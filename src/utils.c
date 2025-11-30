#include "../include/banco.h"

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
	if (dados->nb_consultas_total > 0 && dados->nb_transferencias_total > 0)
		return (true);
	return (false);
}

bool	get_sistema_ativo(t_dados *dados)
{
	bool	sistema_ativo;

	pthread_mutex_lock(&dados->mut_sistema);
	sistema_ativo = dados->sistema_ativo;
	pthread_mutex_unlock(&dados->mut_sistema);
	return (sistema_ativo);
}

void	set_sistema_ativo(t_dados *dados, bool ativo)
{
	pthread_mutex_lock(&dados->mut_sistema);
	dados->sistema_ativo = ativo;
	pthread_mutex_unlock(&dados->mut_sistema);
}

void	set_estado_transacao(t_transacao *transacao, t_estado estado)
{
	pthread_mutex_lock(&transacao->mut_estado);
	if (transacao->estado != BLOQUEADA)
		transacao->estado = estado;
	pthread_mutex_unlock(&transacao->mut_estado);
}

void	imprimir_msg(t_dados *dados, int id, char *msg, t_tipo tipo)
{
	uint64_t	tempo;
	char		*tipo_str;

	tempo = get_tempo() - get_tempo_inicio(dados);
	tipo_str = (tipo == CONSULTOR) ? "CONSULTOR" : "OPERADOR";

	pthread_mutex_lock(&dados->mut_impressao);
	if (get_sistema_ativo(dados))
		printf("%llu %s %d %s\n", tempo, tipo_str, id, msg);
	pthread_mutex_unlock(&dados->mut_impressao);
}

void	liberar_dados(t_dados *dados)
{
	int	i;

	// Liberar consultores
	i = -1;
	while (++i < dados->nb_consultores)
	{
		pthread_mutex_destroy(&dados->consultores[i].mut_estado);
		pthread_mutex_destroy(&dados->consultores[i].mut_operacoes);
	}

	// Liberar operadores
	i = -1;
	while (++i < dados->nb_operadores)
	{
		pthread_mutex_destroy(&dados->operadores[i].mut_estado);
		pthread_mutex_destroy(&dados->operadores[i].mut_operacoes);
	}

	// Liberar controle (mutex + semáforos)
	liberar_controle(&dados->controle);

	// Liberar mutexes principais
	pthread_mutex_destroy(&dados->mut_impressao);
	pthread_mutex_destroy(&dados->mut_sistema);
	pthread_mutex_destroy(&dados->mut_tempo_inicio);

	// Liberar memória
	free(dados->threads_consultores);
	free(dados->threads_operadores);
	free(dados->consultores);
	free(dados->operadores);
}