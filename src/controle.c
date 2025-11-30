#include "../include/banco.h"

int	inicializar_controle(t_controle *controle, int total_threads)
{
	controle->consultores_ativos = 0;
	controle->operador_ativo = 0;
	
	// Inicializar mutexes
	if (pthread_mutex_init(&controle->mut_consultores, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&controle->mut_operador, NULL) != 0)
	{
		pthread_mutex_destroy(&controle->mut_consultores);
		return (1);
	}
	if (pthread_mutex_init(&controle->mut_banco, NULL) != 0)
	{
		pthread_mutex_destroy(&controle->mut_consultores);
		pthread_mutex_destroy(&controle->mut_operador);
		return (1);
	}
	
	// Inicializar semáforo para justiça (evitar starvation)
	#ifdef _WIN32
	controle->semaforo_justica = malloc(sizeof(sem_t));
	if (controle->semaforo_justica == NULL)
		return (1);
	if (sem_init(controle->semaforo_justica, 0, total_threads) != 0)
		return (1);
	#else
	sem_unlink("/semaforo_justica");
	controle->semaforo_justica = sem_open("/semaforo_justica", O_CREAT, 0644, total_threads);
	if (controle->semaforo_justica == SEM_FAILED)
		return (1);
	#endif
	
	return (0);
}

void	liberar_controle(t_controle *controle)
{
	pthread_mutex_destroy(&controle->mut_consultores);
	pthread_mutex_destroy(&controle->mut_operador);
	pthread_mutex_destroy(&controle->mut_banco);
	
	// Liberar semáforo
	#ifdef _WIN32
	if (controle->semaforo_justica != NULL)
	{
		sem_destroy(controle->semaforo_justica);
		free(controle->semaforo_justica);
	}
	#else
	if (controle->semaforo_justica != NULL)
	{
		sem_close(controle->semaforo_justica);
		sem_unlink("/semaforo_justica");
	}
	#endif
}