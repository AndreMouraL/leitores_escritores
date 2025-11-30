#ifndef BANCO_H
# define BANCO_H

# include <unistd.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <stdint.h>
# include <stdbool.h>
# include <semaphore.h>

# define ENTRADA_INVALIDA 1
# define ERRO_ALOCACAO 2
# define ERRO_THREAD 3
# define ERRO_SEMAFORO 4

# define CONSULTANDO_SALDO "consultando saldo"
# define REALIZANDO_TRANSF "realizando transferencia"
# define ANALISANDO_EXTRATO "analisando extrato"
# define AGUARDANDO_ACESSO "aguardando acesso"
# define CONTA_BLOQUEADA "conta bloqueada"

typedef enum e_tipo_transacao
{
	CONSULTOR = 0,      // Leitor - Consulta saldo
	OPERADOR = 1        // Escritor - Faz transferência
}	t_tipo;

typedef enum e_estado_transacao
{
	CONSULTANDO = 0,
	TRANSFERINDO = 1,
	ANALISANDO = 2,
	AGUARDANDO = 3,
	BLOQUEADA = 4
}	t_estado;

struct	s_dados;

typedef struct s_transacao
{
	int				id;
	t_tipo			tipo;
	t_estado		estado;
	struct s_dados	*dados;
	pthread_mutex_t	mut_estado;
	pthread_mutex_t	mut_operacoes;
	int				nb_operacoes_concluidas;
	uint64_t		ultimo_tempo_acesso;
} t_transacao;

typedef struct s_controle_banco
{
	int				consultores_ativos;
	int				operador_ativo;
	pthread_mutex_t	mut_consultores;
	pthread_mutex_t	mut_operador;
	pthread_mutex_t	mut_banco;
	sem_t			*semaforo_justica;  // Para evitar starvation
} t_controle;

typedef struct s_dados
{
	int				nb_consultores;
	int				nb_operadores;
	int				nb_consultas_total;
	int				nb_transferencias_total;
	bool			sistema_ativo;
	uint64_t		tempo_consulta;
	uint64_t		tempo_transferencia;
	uint64_t		tempo_espera;
	uint64_t		tempo_maximo;
	uint64_t		tempo_inicio;
	pthread_mutex_t	mut_impressao;
	pthread_mutex_t	mut_sistema;
	pthread_mutex_t	mut_tempo_inicio;
	t_controle		controle;
	pthread_t		monitor_ativos;
	pthread_t		monitor_concluidos;
	pthread_t		*threads_consultores;
	pthread_t		*threads_operadores;
	t_transacao		*consultores;
	t_transacao		*operadores;
} t_dados;

// main.c
int				sistema_bancario(int argc, char **argv);

// init_data.c
int				inicializar_dados(t_dados *dados, int argc, char **argv);
int				inicializar_transacoes(t_dados *dados);

// controle.c
int				inicializar_controle(t_controle *controle, int total_threads);
void			liberar_controle(t_controle *controle);

// consultor.c
void			*rotina_consultor(void *transacao_p);
int				consultar_saldo(t_transacao *transacao);
int				entrar_banco_consultor(t_transacao *transacao);
void			sair_banco_consultor(t_transacao *transacao);

// transferencia.c
void			*rotina_operador(void *transacao_p);
int				realizar_transferencia(t_transacao *transacao);
int				entrar_banco_operador(t_transacao *transacao);
void			sair_banco_operador(t_transacao *transacao);

// monitor.c
void			*monitor_ativos_rotina(void *dados_p);
void			*monitor_concluidos_rotina(void *dados_p);
bool			transacao_travada(t_transacao *transacao);

// tempo.c
uint64_t		get_tempo(void);
void			ft_dormir(uint64_t tempo);

// utils.c
void			liberar_dados(t_dados *dados);
void			imprimir_msg(t_dados *dados, int id, char *msg, t_tipo tipo);
bool			opcao_nb_operacoes(t_dados *dados);
bool			get_sistema_ativo(t_dados *dados);
void			set_sistema_ativo(t_dados *dados, bool ativo);
void			set_estado_transacao(t_transacao *transacao, t_estado estado);
uint64_t		get_tempo_inicio(t_dados *dados);

// parse.c
int				verificar_entrada(int argc, char **argv);
int				ft_atoi(char *str);
void			imprimir_instrucoes(void);

#endif