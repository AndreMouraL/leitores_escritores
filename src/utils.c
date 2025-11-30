#include "../include/biblioteca.h"

uint64_t	get_tempo_inicio(t_dados *dados)
{
	uint64_t	tempo;

	pthread_mutex_lock(&dados->mut_tempo_inicio);
	tempo = dados->tempo_inicio;
	pthread_mutex_unlock(&dados->mut_tempo_inicio);
	return (tempo);
}

bool	opcao_nb_operacoes(t_dados *dados)
{
	if (dados->nb_leituras_total > 0 && dados->nb_escritas_total > 0)
		return (true);
	return (false);
}

bool	get_manter_execucao(t_dados *dados)
{
	bool	manter_execucao;

	pthread_mutex_lock(&dados->mut_manter_exec);
	manter_execucao = dados->manter_execucao;
	pthread_mutex_unlock(&dados->mut_manter_exec);
	return (manter_execucao);
}

void	set_manter_execucao(t_dados *dados, bool definir_para)
{
	pthread_mutex_lock(&dados->mut_manter_exec);
	dados->manter_execucao = definir_para;
	pthread_mutex_unlock(&dados->mut_manter_exec);
}

void	set_estado_thread(t_thread *thread, t_estado estado)
{
	pthread_mutex_lock(&thread->mut_estado);
	if (thread->estado != BLOQUEADO)
		thread->estado = estado;
	pthread_mutex_unlock(&thread->mut_estado);
}

void	imprimir_msg(t_dados *dados, int id, char *msg, t_tipo tipo)
{
	uint64_t	tempo;
	char		*tipo_str;

	tempo = get_tempo() - get_tempo_inicio(dados);
	tipo_str = (tipo == LEITOR) ? "LEITOR" : "ESCRITOR";

	pthread_mutex_lock(&dados->mut_impressao);
	if (get_manter_execucao(dados))
		printf("%llu %s %d %s\n", tempo, tipo_str, id, msg);
	pthread_mutex_unlock(&dados->mut_impressao);
}

void	liberar_dados(t_dados *dados)
{
	int	i;

	// Liberar leitores
	i = -1;
	while (++i < dados->nb_leitores)
	{
		pthread_mutex_destroy(&dados->leitores[i].mut_estado);
		pthread_mutex_destroy(&dados->leitores[i].mut_leituras_concluidas);
	}

	// Liberar escritores
	i = -1;
	while (++i < dados->nb_escritores)
	{
		pthread_mutex_destroy(&dados->escritores[i].mut_estado);
		pthread_mutex_destroy(&dados->escritores[i].mut_leituras_concluidas);
	}

	// Liberar biblioteca
	liberar_biblioteca(&dados->biblioteca);

	// Liberar mutexes principais
	pthread_mutex_destroy(&dados->mut_impressao);
	pthread_mutex_destroy(&dados->mut_manter_exec);
	pthread_mutex_destroy(&dados->mut_tempo_inicio);

	// Liberar memória
	free(dados->threads_leitores);
	free(dados->threads_escritores);
	free(dados->leitores);
	free(dados->escritores);
}