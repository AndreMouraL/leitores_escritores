#include "../include/biblioteca.h"

void	sair_biblioteca_leitor(t_thread *thread)
{
	t_dados		*dados;
	t_biblioteca	*biblioteca;

	dados = thread->dados;
	biblioteca = &dados->biblioteca;

	pthread_mutex_lock(&biblioteca->mut_leitores);
	biblioteca->leitores_ativos--;
	if (biblioteca->leitores_ativos == 0)
		pthread_mutex_unlock(&biblioteca->mut_escritor);
	pthread_mutex_unlock(&biblioteca->mut_leitores);
}

int	entrar_biblioteca_leitor(t_thread *thread)
{
	t_dados		*dados;
	t_biblioteca	*biblioteca;

	dados = thread->dados;
	biblioteca = &dados->biblioteca;

	pthread_mutex_lock(&biblioteca->mut_biblioteca);
	pthread_mutex_lock(&biblioteca->mut_leitores);
	biblioteca->leitores_ativos++;
	if (biblioteca->leitores_ativos == 1)
		pthread_mutex_lock(&biblioteca->mut_escritor);
	pthread_mutex_unlock(&biblioteca->mut_leitores);
	pthread_mutex_unlock(&biblioteca->mut_biblioteca);

	return (0);
}

int	ler(t_thread *thread)
{
	t_dados	*dados;

	dados = thread->dados;
	
	if (entrar_biblioteca_leitor(thread) != 0)
		return (1);

	set_estado_thread(thread, LENDO);
	imprimir_msg(dados, thread->id, LER, LEITOR);
	thread->ultimo_tempo_acesso = get_tempo();
	
	ft_dormir(dados->tempo_leitura);
	
	pthread_mutex_lock(&thread->mut_leituras_concluidas);
	thread->nb_leituras_concluidas++;
	pthread_mutex_unlock(&thread->mut_leituras_concluidas);
	
	sair_biblioteca_leitor(thread);
	return (0);
}

void	*rotina_leitor(void *thread_p)
{
	t_thread	*thread;

	thread = (t_thread *)thread_p;
	thread->ultimo_tempo_acesso = get_tempo();

	if (thread->id % 2 == 0)
		ft_dormir(thread->dados->tempo_leitura - 10);

	while (get_manter_execucao(thread->dados) && thread->estado != BLOQUEADO)
	{
		if (ler(thread) != 0)
			break;
			
		if (thread->estado == BLOQUEADO || !get_manter_execucao(thread->dados))
			break;

		set_estado_thread(thread, ESPERANDO);
		imprimir_msg(thread->dados, thread->id, ESPERAR, LEITOR);
		ft_dormir(thread->dados->tempo_espera);

		if (thread->estado == BLOQUEADO || !get_manter_execucao(thread->dados))
			break;

		set_estado_thread(thread, PENSANDO);
		imprimir_msg(thread->dados, thread->id, PENSAR, LEITOR);
	}
	return (NULL);
}