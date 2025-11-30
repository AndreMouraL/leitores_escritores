#include "../include/biblioteca.h"

int	inicializar_biblioteca(t_biblioteca *biblioteca)
{
	biblioteca->leitores_ativos = 0;
	biblioteca->escritor_ativo = 0;
	
	if (pthread_mutex_init(&biblioteca->mut_leitores, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&biblioteca->mut_escritor, NULL) != 0)
	{
		pthread_mutex_destroy(&biblioteca->mut_leitores);
		return (1);
	}
	if (pthread_mutex_init(&biblioteca->mut_biblioteca, NULL) != 0)
	{
		pthread_mutex_destroy(&biblioteca->mut_leitores);
		pthread_mutex_destroy(&biblioteca->mut_escritor);
		return (1);
	}
	
	return (0);
}

void	liberar_biblioteca(t_biblioteca *biblioteca)
{
	pthread_mutex_destroy(&biblioteca->mut_leitores);
	pthread_mutex_destroy(&biblioteca->mut_escritor);
	pthread_mutex_destroy(&biblioteca->mut_biblioteca);
}