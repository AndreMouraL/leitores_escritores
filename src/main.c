#include "../include/biblioteca.h"

int	executar_threads(t_dados *dados)
{
	int	i;

	dados->tempo_inicio = get_tempo();
	
	// Criar threads leitores
	i = -1;
	while (++i < dados->nb_leitores)
	{
		if (pthread_create(&dados->threads_leitores[i], NULL,
				&rotina_leitor, &dados->leitores[i]))
			return (1);
	}
	
	// Criar threads escritores
	i = -1;
	while (++i < dados->nb_escritores)
	{
		if (pthread_create(&dados->threads_escritores[i], NULL,
				&rotina_escritor, &dados->escritores[i]))
			return (1);
	}
	
	// Criar monitores
	if (pthread_create(&dados->monitor_todos_ativos, NULL,
			&todos_ativos_rotina, dados))
		return (1);
		
	if (opcao_nb_operacoes(dados) && pthread_create(&dados->monitor_todos_concluidos, NULL,
			&todos_concluidos_rotina, dados))
		return (1);
		
	return (0);
}

int	unir_threads(t_dados *dados)
{
	int	i;

	if (pthread_join(dados->monitor_todos_ativos, NULL))
		return (1);
		
	if (opcao_nb_operacoes(dados) && pthread_join(dados->monitor_todos_concluidos, NULL))
		return (1);
	
	i = -1;
	while (++i < dados->nb_leitores)
	{
		if (pthread_join(dados->threads_leitores[i], NULL))
			return (1);
	}
	
	i = -1;
	while (++i < dados->nb_escritores)
	{
		if (pthread_join(dados->threads_escritores[i], NULL))
			return (1);
	}
	
	return (0);
}

int	sistema_biblioteca(int argc, char **argv)
{
	t_dados dados;

	if (inicializar_dados(&dados, argc, argv) != 0)
		return (ERRO_ALOCACAO);
		
	inicializar_threads(&dados);
	inicializar_biblioteca(&dados.biblioteca);
	
	if (executar_threads(&dados) != 0)
		return (ERRO_THREAD);
		
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
	
	if (sistema_biblioteca(argc, argv) != 0)
		return (ERRO_ALOCACAO);
		
	return (0);
}