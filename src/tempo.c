#include "../include/banco.h"

uint64_t	get_tempo(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (0);
	return ((tv.tv_sec * (uint64_t)1000) + (tv.tv_usec / 1000));
}

void	ft_dormir(uint64_t tempo_sono)
{
	uint64_t	inicio;

	inicio = get_tempo();
	while ((get_tempo() - inicio) < tempo_sono)
		usleep(500);
}