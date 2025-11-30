#include "../include/banco.h"

bool	transacao_travada(t_transacao *transacao)
{
	bool	resultado;
	t_dados	*dados;

	dados = transacao->dados;
	resultado = false;

	if (get_tempo() - transacao->ultimo_tempo_acesso > dados->tempo_maximo &&
		transacao->estado != CONSULTANDO && transacao->estado != TRANSFERINDO)
	{
		set_estado_transacao(transacao, BLOQUEADA);
		resultado = true;
	}
	return (resultado);
}

bool	todos_consultores_concluidos(t_dados *dados)
{
	int	i;

	i = -1;
	while (++i < dados->nb_consultores)
	{
		pthread_mutex_lock(&dados->consultores[i].mut_operacoes);
		if (dados->consultores[i].nb_operacoes_concluidas < dados->nb_consultas_total)
		{
			pthread_mutex_unlock(&dados->consultores[i].mut_operacoes);
			return (false);
		}
		pthread_mutex_unlock(&dados->consultores[i].mut_operacoes);
	}
	return (true);
}

bool	todos_operadores_concluidos(t_dados *dados)
{
	int	i;

	i = -1;
	while (++i < dados->nb_operadores)
	{
		pthread_mutex_lock(&dados->operadores[i].mut_operacoes);
		if (dados->operadores[i].nb_operacoes_concluidas < dados->nb_transferencias_total)
		{
			pthread_mutex_unlock(&dados->operadores[i].mut_operacoes);
			return (false);
		}
		pthread_mutex_unlock(&dados->operadores[i].mut_operacoes);
	}
	return (true);
}

void	notificar_todas_transacoes(t_dados *dados)
{
	int	i;

	i = -1;
	while (++i < dados->nb_consultores)
		set_estado_transacao(&dados->consultores[i], BLOQUEADA);

	i = -1;
	while (++i < dados->nb_operadores)
		set_estado_transacao(&dados->operadores[i], BLOQUEADA);
}

void	*monitor_ativos_rotina(void *dados_p)
{
	int			i;
	t_dados		*dados;

	dados = (t_dados *)dados_p;

	while (get_sistema_ativo(dados))
	{
		// Verificar consultores
		i = -1;
		while (++i < dados->nb_consultores && get_sistema_ativo(dados))
		{
			if (transacao_travada(&dados->consultores[i]))
			{
				imprimir_msg(dados, dados->consultores[i].id, CONTA_BLOQUEADA, CONSULTOR);
				set_sistema_ativo(dados, false);
				notificar_todas_transacoes(dados);
				break;
			}
		}

		// Verificar operadores
		i = -1;
		while (++i < dados->nb_operadores && get_sistema_ativo(dados))
		{
			if (transacao_travada(&dados->operadores[i]))
			{
				imprimir_msg(dados, dados->operadores[i].id, CONTA_BLOQUEADA, OPERADOR);
				set_sistema_ativo(dados, false);
				notificar_todas_transacoes(dados);
				break;
			}
		}
		usleep(1000);
	}
	return (NULL);
}

void	*monitor_concluidos_rotina(void *dados_p)
{
	t_dados	*dados;

	dados = (t_dados *)dados_p;

	while (get_sistema_ativo(dados))
	{
		if (todos_consultores_concluidos(dados) && todos_operadores_concluidos(dados))
		{
			set_sistema_ativo(dados, false);
			notificar_todas_transacoes(dados);
			break;
		}
		usleep(1000);
	}
	return (NULL);
}