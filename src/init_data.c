#include "../include/biblioteca.h"

int	inicializar_threads(t_dados *dados)
{
	int	i;

	// Inicializar leitores
	i = -1;
	while (++i < dados->nb_leitores)
	{
		dados->leitores[i].dados = dados;
		dados->leitores[i].id = i + 1;
		dados->leitores[i].tipo = LEITOR;
		dados->leitores[i].estado = ESPERANDO;
		dados->leitores[i].nb_leituras_concluidas = 0;
		dados->leitores[i].ultimo_tempo_acesso = get_tempo();
		pthread_mutex_init(&dados->leitores[i].mut_estado, NULL);
		pthread_mutex_init(&dados->leitores[i].mut_leituras_concluidas, NULL);
	}

	// Inicializar escritores
	i = -1;
	while (++i < dados->nb_escritores)
	{
		dados->escritores[i].dados = dados;
		dados->escritores[i].id = i + 1;
		dados->escritores[i].tipo = ESCRITOR;
		dados->escritores[i].estado = ESPERANDO;
		dados->escritores[i].nb_leituras_concluidas = 0;
		dados->escritores[i].ultimo_tempo_acesso = get_tempo();
		pthread_mutex_init(&dados->escritores[i].mut_estado, NULL);
		pthread_mutex_init(&dados->escritores[i].mut_leituras_concluidas, NULL);
	}

	return (0);
}

int	alocar_dados(t_dados *dados)
{
	// Alocar leitores
	dados->leitores = malloc(sizeof(t_thread) * dados->nb_leitores);
	if (dados->leitores == NULL)
		return (ERRO_ALOCACAO);

	dados->threads_leitores = malloc(sizeof(pthread_t) * dados->nb_leitores);
	if (dados->threads_leitores == NULL)
		return (free(dados->leitores), ERRO_ALOCACAO);

	// Alocar escritores
	dados->escritores = malloc(sizeof(t_thread) * dados->nb_escritores);
	if (dados->escritores == NULL)
		return (free(dados->leitores), free(dados->threads_leitores), ERRO_ALOCACAO);

	dados->threads_escritores = malloc(sizeof(pthread_t) * dados->nb_escritores);
	if (dados->threads_escritores == NULL)
		return (free(dados->leitores), free(dados->threads_leitores), free(dados->escritores), ERRO_ALOCACAO);

	return (0);
}

int	inicializar_dados(t_dados *dados, int argc, char **argv)
{
	dados->nb_leitores = ft_atoi(argv[1]);
	dados->nb_escritores = ft_atoi(argv[2]);
	dados->tempo_maximo = (uint64_t)ft_atoi(argv[3]);
	dados->tempo_leitura = (uint64_t)ft_atoi(argv[4]);
	dados->tempo_escrita = (uint64_t)ft_atoi(argv[5]);
	dados->tempo_espera = (uint64_t)ft_atoi(argv[6]);
	dados->nb_leituras_total = -1;
	dados->nb_escritas_total = -1;
	dados->manter_execucao = true;

	if (argc == 8)
	{
		dados->nb_leituras_total = ft_atoi(argv[7]);
		dados->nb_escritas_total = ft_atoi(argv[7]);
	}

	pthread_mutex_init(&dados->mut_impressao, NULL);
	pthread_mutex_init(&dados->mut_manter_exec, NULL);
	pthread_mutex_init(&dados->mut_tempo_inicio, NULL);

	return (alocar_dados(dados));
}