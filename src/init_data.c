#include "../include/banco.h"

int	inicializar_transacoes(t_dados *dados)
{
	int	i;

	// Inicializar consultores
	i = -1;
	while (++i < dados->nb_consultores)
	{
		dados->consultores[i].dados = dados;
		dados->consultores[i].id = i + 1;
		dados->consultores[i].tipo = CONSULTOR;
		dados->consultores[i].estado = AGUARDANDO;
		dados->consultores[i].nb_operacoes_concluidas = 0;
		dados->consultores[i].ultimo_tempo_acesso = get_tempo();
		pthread_mutex_init(&dados->consultores[i].mut_estado, NULL);
		pthread_mutex_init(&dados->consultores[i].mut_operacoes, NULL);
	}

	// Inicializar operadores
	i = -1;
	while (++i < dados->nb_operadores)
	{
		dados->operadores[i].dados = dados;
		dados->operadores[i].id = i + 1;
		dados->operadores[i].tipo = OPERADOR;
		dados->operadores[i].estado = AGUARDANDO;
		dados->operadores[i].nb_operacoes_concluidas = 0;
		dados->operadores[i].ultimo_tempo_acesso = get_tempo();
		pthread_mutex_init(&dados->operadores[i].mut_estado, NULL);
		pthread_mutex_init(&dados->operadores[i].mut_operacoes, NULL);
	}

	return (0);
}

int	alocar_dados(t_dados *dados)
{
	// Alocar consultores
	dados->consultores = malloc(sizeof(t_transacao) * dados->nb_consultores);
	if (dados->consultores == NULL)
		return (ERRO_ALOCACAO);

	dados->threads_consultores = malloc(sizeof(pthread_t) * dados->nb_consultores);
	if (dados->threads_consultores == NULL)
		return (free(dados->consultores), ERRO_ALOCACAO);

	// Alocar operadores
	dados->operadores = malloc(sizeof(t_transacao) * dados->nb_operadores);
	if (dados->operadores == NULL)
		return (free(dados->consultores), free(dados->threads_consultores), ERRO_ALOCACAO);

	dados->threads_operadores = malloc(sizeof(pthread_t) * dados->nb_operadores);
	if (dados->threads_operadores == NULL)
		return (free(dados->consultores), free(dados->threads_consultores), 
				free(dados->operadores), ERRO_ALOCACAO);

	return (0);
}

int	inicializar_dados(t_dados *dados, int argc, char **argv)
{
	dados->nb_consultores = ft_atoi(argv[1]);
	dados->nb_operadores = ft_atoi(argv[2]);
	dados->tempo_maximo = (uint64_t)ft_atoi(argv[3]);
	dados->tempo_consulta = (uint64_t)ft_atoi(argv[4]);
	dados->tempo_transferencia = (uint64_t)ft_atoi(argv[5]);
	dados->tempo_espera = (uint64_t)ft_atoi(argv[6]);
	dados->nb_consultas_total = -1;
	dados->nb_transferencias_total = -1;
	dados->sistema_ativo = true;

	if (argc == 8)
	{
		dados->nb_consultas_total = ft_atoi(argv[7]);
		dados->nb_transferencias_total = ft_atoi(argv[7]);
	}

	pthread_mutex_init(&dados->mut_impressao, NULL);
	pthread_mutex_init(&dados->mut_sistema, NULL);
	pthread_mutex_init(&dados->mut_tempo_inicio, NULL);

	return (alocar_dados(dados));
}