 ##Sistema Bancário - Leitores e Escritores
 ##Sistema de Transações Bancárias Concorrentes

## Sobre o Projeto
Este projeto implementa uma solução para o Problema dos Leitores e Escritores adaptado para um sistema bancário concorrente. É uma adaptação prática onde múltiplos consultores (leitores) e operadores (escritores) competem por acesso a contas bancárias compartilhadas.

O sistema garente que:

✅ Múltiplas consultas de saldo podem ocorrer simultaneamente
✅ Transferências têm acesso exclusivo às contas
✅ Prevenção de inconsistências em transações financeiras
✅ Monitoramento automático contra deadlocks
✅ Controle de starvation com timeouts configuráveis

🏗️ Estrutura do Projeto
banco_leitores_escritores/
├── include/
│   └── banco.h              # Headers para sistema bancário
├── src/
│   ├── main.c               # Função principal
│   ├── init_data.c          # Inicialização de dados
│   ├── consultor.c          # Leitores - Consultas de saldo
│   ├── transferencia.c      # Escritores - Transferências
│   ├── controle.c           # Controle do banco (mutex/semáforos)
│   ├── monitor.c            # Monitoramento do sistema
│   ├── tempo.c              # Funções de tempo
│   ├── utils.c              # Utilitários
│   └── parse.c              # Parsing de argumentos
├── scripts/
│   ├── compilar.bat         # Script de compilação Windows
│   └── converter.ps1        # Conversor para visualizador
├── Makefile                 # Build automation
└── README.md               # Documentação

Como Compilar e Executar
make                    # Compilar o projeto
make clean             # Limpar arquivos objeto
make re               # Recompilar completamente

Execução
./banco_le_esc.exe nb_consultores nb_operadores tempo_maximo tempo_consulta tempo_transferencia tempo_espera [numero_operacoes]
Exemplo Prático
./banco_le_esc.exe 3 2 410 200 100 200 5

📊 Parâmetros do Sistema Bancário
Parâmetro	Descrição	Valores
nb_consultores	Número de consultores de saldo	1-10
nb_operadores	Número de operadores de transferência	1-10
tempo_maximo	Tempo máximo sem acesso (ms)	60+
tempo_consulta	Duração da consulta de saldo (ms)	60+
tempo_transferencia	Duração da transferência (ms)	60+
tempo_espera	Tempo entre operações (ms)	60+
numero_operacoes	Nº de operações por transação	0+ (opcional)

🎯 Funcionalidades Implementadas
🔒 Mecanismos de Sincronização Bancária
Mutexes para proteção de dados financeiros

Semáforos para controle de acesso justo

Acesso simultâneo para consultas de saldo

Acesso exclusivo para transferências

Prevenção de starvation entre consultores e operadores

👁️ Monitoramento Financeiro
Detecção de transações travadas

Timeout configurável para prevenção de deadlocks

Logging em tempo real de todas as operações

Finalização graciosa quando metas são atingidas

⚡ Performance Bancária
Threads concorrentes para máxima eficiência

Sleeps otimizados entre operações

Alocação dinâmica de recursos

Cleanup completo ao finalizar

🔧 Tecnologias Utilizadas
Linguagem: C

Threading: pthreads

Sincronização: mutexes, semáforos

Tempo: gettimeofday, usleep

Build: Makefile

Plataforma: Windows (compatível com Linux)

📈 Exemplo de Saída do Sistema
text
15 CONSULTOR 1 consultando saldo
18 CONSULTOR 2 consultando saldo
20 OPERADOR 1 aguardando acesso
215 CONSULTOR 1 aguardando acesso
218 CONSULTOR 2 aguardando acesso
220 OPERADOR 1 realizando transferencia
420 CONSULTOR 3 consultando saldo
625 OPERADOR 1 analisando extrato
🎮 Visualização das Transações
Para visualizar a execução graficamente:

Execute o programa e salve a saída:

bash
./banco_le_esc.exe 3 2 410 200 100 200 5 > saida.txt
Converta para o formato do visualizador:

bash
.\scripts\converter.ps1 -InputFile saida.txt
Acesse Philosophers Visualizer e cole o conteúdo de visualizer_output.txt

🐛 Solução de Problemas
Erros Comuns
"Entrada inválida": Verifique se todos os parâmetros são números positivos

"Falha na alocação": Sistema sem memória suficiente

"Transações travadas": Aumente o tempo_maximo para evitar timeouts prematuros

Debug
Compile com make DEBUG=1 para logs detalhados

Use make run para teste com parâmetros padrão

Verifique permissões de execução nos scripts

📚 Conceitos de Sistemas Bancários Aplicados
Exclusão mútua em transações financeiras

Consistência de dados em operações concorrentes

Controle de concorrência em sistemas bancários

Prevenção de condições de corrida em transferências

Gerenciamento de deadlocks em transações

Justiça no acesso entre consultas e transferências

👥 Contexto do Projeto
Desenvolvido como adaptação do problema clássico dos leitores e escritores para um sistema bancário real, demonstrando conceitos avançados de:

Programação concorrente em sistemas financeiros

Sincronização em transações bancárias

Controle de acesso a recursos compartilhados

Monitoramento de sistemas em tempo real

Nota: Este projeto é educativo e demonstra conceitos avançados de programação concorrente aplicados a sistemas bancários, garantindo a segurança e consistência das transações financeiras.


Desenvolvido por Andre Moura na disciplina de tempo real UFMA, o uso indevido sem mensão acarretarar em sações judiciais.
