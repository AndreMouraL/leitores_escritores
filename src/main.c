#include "../include/banco.h"

int	executar_threads(t_dados *dados)
{
	int	i;

	dados->tempo_inicio = get_tempo();
	
	// Criar threads consultores
	i = -1;
	while (++i < dados->nb_consultores)
	{
		if (pthread_create(&dados->threads_consultores[i], NULL,
				&rotina_consultor, &dados->consultores[i]))
			return (1);
	}
	
	// Criar threads operadores
	i = -1;
	while (++i < dados->nb_operadores)
	{
		if (pthread_create(&dados->threads_operadores[i], NULL,
				&rotina_operador, &dados->operadores[i]))
			return (1);
	}
	
	// Criar monitores
	if (pthread_create(&dados->monitor_ativos, NULL,
			&monitor_ativos_rotina, dados))
		return (1);
		
	if (opcao_nb_operacoes(dados) && 
		pthread_create(&dados->monitor_concluidos, NULL,
			&monitor_concluidos_rotina, dados))
		return (1);
		
	return (0);
}

int	unir_threads(t_dados *dados)
{
	int	i;

	if (pthread_join(dados->monitor_ativos, NULL))
		return (1);
		
	if (opcao_nb_operacoes(dados) && pthread_join(dados->monitor_concluidos, NULL))
		return (1);
	
	i = -1;
	while (++i < dados->nb_consultores)
	{
		if (pthread_join(dados->threads_consultores[i], NULL))
			return (1);
	}
	
	i = -1;
	while (++i < dados->nb_operadores)
	{
		if (pthread_join(dados->threads_operadores[i], NULL))
			return (1);
	}
	
	return (0);
}

int	sistema_bancario(int argc, char **argv)
{
	t_dados dados;
	int	total_threads;

	if (inicializar_dados(&dados, argc, argv) != 0)
		return (ERRO_ALOCACAO);
		
	inicializar_transacoes(&dados);
	
	total_threads = dados.nb_consultores + dados.nb_operadores;
	if (inicializar_controle(&dados.controle, total_threads) != 0)
		return (liberar_dados(&dados), ERRO_SEMAFORO);
	
	if (executar_threads(&dados) != 0)
		return (liberar_dados(&dados), ERRO_THREAD);
		
	unir_threads(&dados);
	liberar_dados(&dados);
	
	return (0);
}

int	main(int argc, char **argv)
{
	if (verificar_entrada(argc, argv) != 0)
	{
		imprimir_instrucoes();
		return (ENTRADA_INVALIDA);
	}
	
	if (sistema_bancario(argc, argv) != 0)
		return (ERRO_ALOCACAO);
		
	return (0);
}