#include "../include/biblioteca.h"

int	inicializar_biblioteca(t_biblioteca *biblioteca)
{
	biblioteca->leitores_ativos = 0;
	biblioteca->escritor_ativo = 0;
	pthread_mutex_init(&biblioteca->mut_leitores, NULL);
	pthread_mutex_init(&biblioteca->mut_escritor, NULL);
	pthread_mutex_init(&biblioteca->mut_biblioteca, NULL);
	return (0);
}

void	liberar_biblioteca(t_biblioteca *biblioteca)
{
	pthread_mutex_destroy(&biblioteca->mut_leitores);
	pthread_mutex_destroy(&biblioteca->mut_escritor);
	pthread_mutex_destroy(&biblioteca->mut_biblioteca);
}