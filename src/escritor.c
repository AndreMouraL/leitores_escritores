#include "../include/biblioteca.h"

void	sair_biblioteca_escritor(t_thread *thread)
{
	t_dados		*dados;
	t_biblioteca	*biblioteca;

	dados = thread->dados;
	biblioteca = &dados->biblioteca;

	pthread_mutex_unlock(&biblioteca->mut_escritor);
	biblioteca->escritor_ativo = 0;
}

int	entrar_biblioteca_escritor(t_thread *thread)
{
	t_dados		*dados;
	t_biblioteca	*biblioteca;

	dados = thread->dados;
	biblioteca = &dados->biblioteca;

	pthread_mutex_lock(&biblioteca->mut_biblioteca);
	pthread_mutex_lock(&biblioteca->mut_escritor);
	biblioteca->escritor_ativo = 1;
	pthread_mutex_unlock(&biblioteca->mut_biblioteca);

	return (0);
}

int	escrever(t_thread *thread)
{
	t_dados	*dados;

	dados = thread->dados;
	
	if (entrar_biblioteca_escritor(thread) != 0)
		return (1);

	set_estado_thread(thread, ESCREVENDO);
	imprimir_msg(dados, thread->id, ESCREVER, ESCRITOR);
	thread->ultimo_tempo_acesso = get_tempo();
	
	ft_dormir(dados->tempo_escrita);
	
	sair_biblioteca_escritor(thread);
	return (0);
}

void	*rotina_escritor(void *thread_p)
{
	t_thread	*thread;

	thread = (t_thread *)thread_p;
	thread->ultimo_tempo_acesso = get_tempo();

	if (thread->id % 2 == 1)
		ft_dormir(thread->dados->tempo_escrita - 10);

	while (get_manter_execucao(thread->dados) && thread->estado != BLOQUEADO)
	{
		if (escrever(thread) != 0)
			break;
			
		if (thread->estado == BLOQUEADO || !get_manter_execucao(thread->dados))
			break;

		set_estado_thread(thread, ESPERANDO);
		imprimir_msg(thread->dados, thread->id, ESPERAR, ESCRITOR);
		ft_dormir(thread->dados->tempo_espera);

		if (thread->estado == BLOQUEADO || !get_manter_execucao(thread->dados))
			break;

		set_estado_thread(thread, PENSANDO);
		imprimir_msg(thread->dados, thread->id, PENSAR, ESCRITOR);
	}
	return (NULL);
}