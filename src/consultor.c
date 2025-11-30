#include "../include/banco.h"

void	sair_banco_consultor(t_transacao *transacao)
{
	t_dados		*dados;
	t_controle	*controle;

	dados = transacao->dados;
	controle = &dados->controle;

	pthread_mutex_lock(&controle->mut_consultores);
	controle->consultores_ativos--;
	if (controle->consultores_ativos == 0)
	{
		sem_post(controle->semaforo_justica); // Libera para escritores
		pthread_mutex_unlock(&controle->mut_operador);
	}
	pthread_mutex_unlock(&controle->mut_consultores);
}

int	entrar_banco_consultor(t_transacao *transacao)
{
	t_dados		*dados;
	t_controle	*controle;

	dados = transacao->dados;
	controle = &dados->controle;

	// Semáforo para justiça (evita starvation de escritores)
	sem_wait(controle->semaforo_justica);
	sem_post(controle->semaforo_justica);

	pthread_mutex_lock(&controle->mut_banco);
	pthread_mutex_lock(&controle->mut_consultores);
	controle->consultores_ativos++;
	if (controle->consultores_ativos == 1)
		pthread_mutex_lock(&controle->mut_operador);
	pthread_mutex_unlock(&controle->mut_consultores);
	pthread_mutex_unlock(&controle->mut_banco);

	return (0);
}

int	consultar_saldo(t_transacao *transacao)
{
	t_dados	*dados;

	dados = transacao->dados;
	
	if (entrar_banco_consultor(transacao) != 0)
		return (1);

	set_estado_transacao(transacao, CONSULTANDO);
	imprimir_msg(dados, transacao->id, CONSULTANDO_SALDO, CONSULTOR);
	transacao->ultimo_tempo_acesso = get_tempo();
	
	ft_dormir(dados->tempo_consulta);
	
	pthread_mutex_lock(&transacao->mut_operacoes);
	transacao->nb_operacoes_concluidas++;
	pthread_mutex_unlock(&transacao->mut_operacoes);
	
	sair_banco_consultor(transacao);
	return (0);
}

void	*rotina_consultor(void *transacao_p)
{
	t_transacao	*transacao;

	transacao = (t_transacao *)transacao_p;
	transacao->ultimo_tempo_acesso = get_tempo();

	while (get_sistema_ativo(transacao->dados) && transacao->estado != BLOQUEADA)
	{
		if (consultar_saldo(transacao) != 0)
			break;
			
		if (transacao->estado == BLOQUEADA || !get_sistema_ativo(transacao->dados))
			break;

		set_estado_transacao(transacao, AGUARDANDO);
		imprimir_msg(transacao->dados, transacao->id, AGUARDANDO_ACESSO, CONSULTOR);
		ft_dormir(transacao->dados->tempo_espera);

		if (transacao->estado == BLOQUEADA || !get_sistema_ativo(transacao->dados))
			break;

		set_estado_transacao(transacao, ANALISANDO);
		imprimir_msg(transacao->dados, transacao->id, ANALISANDO_EXTRATO, CONSULTOR);
	}
	return (NULL);
}