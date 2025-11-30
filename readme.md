📚 Sistema de Leitores e Escritores - Biblioteca Concorrente
📋 Sobre o Projeto
Este projeto implementa uma solução para o Problema dos Leitores e Escritores adaptado para um sistema de biblioteca concorrente. É uma adaptação do conceito de filósofos jantantes, onde múltiplos leitores e escritores competem por acesso a recursos compartilhados.

O sistema garante que:

✅ Múltiplos leitores podem acessar o recurso simultaneamente

✅ Apenas um escritor pode acessar o recurso por vez

✅ Escritores têm exclusividade total durante o acesso

✅ Prevenção de deadlocks com monitoramento automático

✅ Controle de starvation com timeouts configuráveis

🏗️ Estrutura do Projeto
text
leitores_escritores/
├── include/
│   └── biblioteca.h          # Headers e definições
├── src/
│   ├── main.c               # Função principal
│   ├── init_data.c          # Inicialização de dados
│   ├── leitor.c             # Lógica dos leitores
│   ├── escritor.c           # Lógica dos escritores
│   ├── biblioteca.c         # Controle da biblioteca
│   ├── monitor.c            # Monitoramento do sistema
│   ├── tempo.c              # Funções de tempo
│   ├── utils.c              # Utilitários
│   └── parse.c              # Parsing de argumentos
├── scripts/
│   ├── compilar.sh          # Script de compilação
│   └── converter.ps1        # Conversor para visualizador
├── Makefile                 # Build automation
└── README.md               # Este arquivo
🚀 Como Compilar e Executar
Compilação
bash
make                    # Compilar o projeto
make clean             # Limpar arquivos objeto
make re               # Recompilar completamente
Execução
bash
./biblioteca nb_leitores nb_escritores tempo_maximo tempo_leitura tempo_escrita tempo_espera [numero_operacoes]
Exemplo
bash
./biblioteca 3 2 410 200 100 200 5
📊 Parâmetros do Programa
Parâmetro	Descrição	Valores
nb_leitores	Número de threads leitoras	1-20
nb_escritores	Número de threads escritoras	1-20
tempo_maximo	Tempo máximo sem acesso (ms)	60+
tempo_leitura	Duração da leitura (ms)	60+
tempo_escrita	Duração da escrita (ms)	60+
tempo_espera	Tempo entre operações (ms)	60+
numero_operacoes	Nº de operações por thread	0+ (opcional)
🎯 Funcionalidades Implementadas
🔒 Mecanismos de Sincronização
Mutexes para proteção de variáveis compartilhadas

Variáveis de condição para coordenação entre threads

Semáforos implícitos no controle de acesso

Prioridade para leitores (implementação clássica)

👁️ Monitoramento
Detecção de deadlocks automática

Timeout configurável para prevenção de starvation

Logging em tempo real de todas as operações

Finalização graciosa quando metas são atingidas

⚡ Performance
Threads concorrentes para máxima utilização de CPU

Sleeps eficientes entre operações

Alocação dinâmica de recursos

Cleanup completo ao finalizar

🔧 Tecnologias Utilizadas
Linguagem: C

Threading: pthreads

Sincronização: mutexes, variáveis de condição

Tempo: gettimeofday, usleep

Build: Makefile

📈 Exemplo de Saída
text
15 LEITOR 1 esta lendo
18 LEITOR 2 esta lendo
20 ESCRITOR 1 esta esperando
215 LEITOR 1 esta esperando
218 LEITOR 2 esta esperando
220 ESCRITOR 1 esta escrevendo
420 LEITOR 3 esta lendo
625 ESCRITOR 1 esta pensando
🎮 Visualização
Para visualizar a execução graficamente:

Execute o programa e salve a saída:

bash
./biblioteca 3 2 410 200 100 200 5 > saida.txt
Converta para o formato do visualizador:

bash
./scripts/converter.ps1 -InputFile saida.txt
Acesse Philosophers Visualizer e cole o conteúdo de visualizer_output.txt

🐛 Solução de Problemas
Erros Comuns
"Entrada inválida": Verifique se todos os parâmetros são números positivos

"Falha na alocação": Sistema sem memória suficiente

Threads travadas: Aumente o tempo_maximo para evitar timeouts prematuros

Debug
Compile com make DEBUG=1 para logs detalhados

Use make run para teste com parâmetros padrão

Verifique permissões de execução nos scripts

📚 Conceitos Aplicados
Exclusão mútua com mutexes

Sincronização entre threads

Condições de corrida e prevenção

Deadlocks e starvation

Programação concorrente em C

Gerenciamento de recursos compartilhados

👥 Autor
Desenvolvido como adaptação do problema dos filósofos jantantes para estudo de sistemas concorrentes.

Nota: Este projeto é educativo e demonstra conceitos avançados de programação concorrente e sincronização em sistemas operacionais.

