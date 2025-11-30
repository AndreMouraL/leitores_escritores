#ifndef BIBLIOTECA_H
# define BIBLIOTECA_H

# include <unistd.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <stdint.h>
# include <stdbool.h>

# define ENTRADA_INVALIDA 1
# define ERRO_ALOCACAO 2
# define ERRO_THREAD 3

# define LER "esta lendo"
# define ESCREVER "esta escrevendo"
# define PENSAR "esta pensando"
# define ESPERAR "esta esperando"
# define ACESSO_NEGADO "acesso negado"

typedef enum e_tipo
{
	LEITOR = 0,
	ESCRITOR = 1
}	t_tipo;

typedef enum e_estado
{
	LENDO = 0,
	ESCREVENDO = 1,
	PENSANDO = 2,
	ESPERANDO = 3,
	BLOQUEADO = 4
}	t_estado;

struct s_dados;

typedef struct s_thread
{
	int				id;
	t_tipo			tipo;
	t_estado		estado;
	struct s_dados	*dados;
	pthread_mutex_t	mut_estado;
	pthread_mutex_t	mut_leituras_concluidas;
	int				nb_leituras_concluidas;
	uint64_t		ultimo_tempo_acesso;
} t_thread;

typedef struct s_biblioteca
{
	int				leitores_ativos;
	int				escritor_ativo;
	pthread_mutex_t	mut_leitores;
	pthread_mutex_t	mut_escritor;
	pthread_mutex_t	mut_biblioteca;
} t_biblioteca;

typedef struct s_dados
{
	int				nb_leitores;
	int				nb_escritores;
	int				nb_leituras_total;
	int				nb_escritas_total;
	bool			manter_execucao;
	uint64_t		tempo_leitura;
	uint64_t		tempo_escrita;
	uint64_t		tempo_espera;
	uint64_t		tempo_maximo;
	uint64_t		tempo_inicio;
	pthread_mutex_t	mut_impressao;
	pthread_mutex_t	mut_manter_exec;
	pthread_mutex_t	mut_tempo_inicio;
	t_biblioteca	biblioteca;
	pthread_t		monitor_todos_ativos;
	pthread_t		monitor_todos_concluidos;
	pthread_t		*threads_leitores;
	pthread_t		*threads_escritores;
	t_thread		*leitores;
	t_thread		*escritores;
} t_dados;

// main.c
int				sistema_biblioteca(int argc, char **argv);

// init_data.c
int				inicializar_dados(t_dados *dados, int argc, char **argv);
int				inicializar_threads(t_dados *dados);

// biblioteca.c
int				inicializar_biblioteca(t_biblioteca *biblioteca);
void			liberar_biblioteca(t_biblioteca *biblioteca);

// leitor.c
void			*rotina_leitor(void *thread_p);
int				ler(t_thread *thread);
int				entrar_biblioteca_leitor(t_thread *thread);
void			sair_biblioteca_leitor(t_thread *thread);

// escritor.c
void			*rotina_escritor(void *thread_p);
int				escrever(t_thread *thread);
int				entrar_biblioteca_escritor(t_thread *thread);
void			sair_biblioteca_escritor(t_thread *thread);

// monitor.c
void			*todos_ativos_rotina(void *dados_p);
void			*todos_concluidos_rotina(void *dados_p);
bool			thread_morreu(t_thread *thread);
bool			todos_leitores_concluidos(t_dados *dados);
bool			todos_escritores_concluidos(t_dados *dados);
void			notificar_todas_threads(t_dados *dados);

// tempo.c
uint64_t		get_tempo(void);
void			ft_dormir(uint64_t tempo);

// utils.c
void			liberar_dados(t_dados *dados);
void			imprimir_msg(t_dados *dados, int id, char *msg, t_tipo tipo);
bool			opcao_nb_operacoes(t_dados *dados);
bool			get_manter_execucao(t_dados *dados);
void			set_manter_execucao(t_dados *dados, bool definir_para);
void			set_estado_thread(t_thread *thread, t_estado estado);
uint64_t		get_tempo_inicio(t_dados *dados);

// parse.c
int				verificar_entrada(int argc, char **argv);
int				ft_atoi(char *str);
void			imprimir_instrucoes(void);

#endif