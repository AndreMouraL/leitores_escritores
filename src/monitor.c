#include "../include/biblioteca.h"

bool	thread_morreu(t_thread *thread)
{
	bool	resultado;
	t_dados	*dados;

	dados = thread->dados;
	resultado = false;

	if (get_tempo() - thread->ultimo_tempo_acesso > dados->tempo_maximo &&
		thread->estado != LENDO && thread->estado != ESCREVENDO)
	{
		set_estado_thread(thread, BLOQUEADO);
		resultado = true;
	}
	return (resultado);
}

bool	todos_leitores_concluidos(t_dados *dados)
{
	int	i;

	i = -1;
	while (++i < dados->nb_leitores)
	{
		pthread_mutex_lock(&dados->leitores[i].mut_leituras_concluidas);
		if (dados->leitores[i].nb_leituras_concluidas < dados->nb_leituras_total)
		{
			pthread_mutex_unlock(&dados->leitores[i].mut_leituras_concluidas);
			return (false);
		}
		pthread_mutex_unlock(&dados->leitores[i].mut_leituras_concluidas);
	}
	return (true);
}

bool	todos_escritores_concluidos(t_dados *dados)
{
	int	i;

	i = -1;
	while (++i < dados->nb_escritores)
	{
		pthread_mutex_lock(&dados->escritores[i].mut_leituras_concluidas);
		if (dados->escritores[i].nb_leituras_concluidas < dados->nb_escritas_total)
		{
			pthread_mutex_unlock(&dados->escritores[i].mut_leituras_concluidas);
			return (false);
		}
		pthread_mutex_unlock(&dados->escritores[i].mut_leituras_concluidas);
	}
	return (true);
}

void	notificar_todas_threads(t_dados *dados)
{
	int	i;

	i = -1;
	while (++i < dados->nb_leitores)
		set_estado_thread(&dados->leitores[i], BLOQUEADO);

	i = -1;
	while (++i < dados->nb_escritores)
		set_estado_thread(&dados->escritores[i], BLOQUEADO);
}

void	*todos_ativos_rotina(void *dados_p)
{
	int			i;
	t_dados		*dados;

	dados = (t_dados *)dados_p;

	while (get_manter_execucao(dados))
	{
		// Verificar leitores
		i = -1;
		while (++i < dados->nb_leitores && get_manter_execucao(dados))
		{
			if (thread_morreu(&dados->leitores[i]))
			{
				imprimir_msg(dados, dados->leitores[i].id, "sistema travou", LEITOR);
				set_manter_execucao(dados, false);
				notificar_todas_threads(dados);
				break;
			}
		}

		// Verificar escritores
		i = -1;
		while (++i < dados->nb_escritores && get_manter_execucao(dados))
		{
			if (thread_morreu(&dados->escritores[i]))
			{
				imprimir_msg(dados, dados->escritores[i].id, "sistema travou", ESCRITOR);
				set_manter_execucao(dados, false);
				notificar_todas_threads(dados);
				break;
			}
		}
		usleep(1000);
	}
	return (NULL);
}

void	*todos_concluidos_rotina(void *dados_p)
{
	t_dados	*dados;

	dados = (t_dados *)dados_p;

	while (get_manter_execucao(dados))
	{
		if (todos_leitores_concluidos(dados) && todos_escritores_concluidos(dados))
		{
			set_manter_execucao(dados, false);
			notificar_todas_threads(dados);
			break;
		}
		usleep(1000);
	}
	return (NULL);
}