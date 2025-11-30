#include "../include/banco.h"

void	sair_banco_operador(t_transacao *transacao)
{
	t_dados		*dados;
	t_controle	*controle;

	dados = transacao->dados;
	controle = &dados->controle;

	pthread_mutex_unlock(&controle->mut_operador);
	controle->operador_ativo = 0;
	sem_post(controle->semaforo_justica); // Libera semáforo
}

int	entrar_banco_operador(t_transacao *transacao)
{
	t_dados		*dados;
	t_controle	*controle;

	dados = transacao->dados;
	controle = &dados->controle;

	// Semáforo para justiça - operador pega todos os slots
	int total_threads = dados->nb_consultores + dados->nb_operadores;
	for (int i = 0; i < total_threads; i++)
		sem_wait(controle->semaforo_justica);

	pthread_mutex_lock(&controle->mut_banco);
	pthread_mutex_lock(&controle->mut_operador);
	controle->operador_ativo = 1;
	pthread_mutex_unlock(&controle->mut_banco);

	return (0);
}

int	realizar_transferencia(t_transacao *transacao)
{
	t_dados	*dados;

	dados = transacao->dados;
	
	if (entrar_banco_operador(transacao) != 0)
		return (1);

	set_estado_transacao(transacao, TRANSFERINDO);
	imprimir_msg(dados, transacao->id, REALIZANDO_TRANSF, OPERADOR);
	transacao->ultimo_tempo_acesso = get_tempo();
	
	ft_dormir(dados->tempo_transferencia);
	
	pthread_mutex_lock(&transacao->mut_operacoes);
	transacao->nb_operacoes_concluidas++;
	pthread_mutex_unlock(&transacao->mut_operacoes);
	
	sair_banco_operador(transacao);
	return (0);
}

void	*rotina_operador(void *transacao_p)
{
	t_transacao	*transacao;

	transacao = (t_transacao *)transacao_p;
	transacao->ultimo_tempo_acesso = get_tempo();

	while (get_sistema_ativo(transacao->dados) && transacao->estado != BLOQUEADA)
	{
		if (realizar_transferencia(transacao) != 0)
			break;
			
		if (transacao->estado == BLOQUEADA || !get_sistema_ativo(transacao->dados))
			break;

		set_estado_transacao(transacao, AGUARDANDO);
		imprimir_msg(transacao->dados, transacao->id, AGUARDANDO_ACESSO, OPERADOR);
		ft_dormir(transacao->dados->tempo_espera);

		if (transacao->estado == BLOQUEADA || !get_sistema_ativo(transacao->dados))
			break;

		set_estado_transacao(transacao, ANALISANDO);
		imprimir_msg(transacao->dados, transacao->id, ANALISANDO_EXTRATO, OPERADOR);
	}
	return (NULL);
}