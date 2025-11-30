#include "../include/banco.h"

void	imprimir_instrucoes(void)
{
	printf("\t\tENTRADA INVALIDA!\n\n");
	printf("./banco_le_esc nb_consultores nb_operadores tempo_maximo ");
	printf("tempo_consulta tempo_transferencia tempo_espera ");
	printf("[numero_operacoes]\n");
	printf("Exemplo:\n\n");
	printf("./banco_le_esc 3 2 410 200 100 200 5\n\n");
	printf("nb_consultores: 1-10 (leitores - consultas de saldo)\n");
	printf("nb_operadores: 1-10 (escritores - transferencias)\n");
	printf("tempo_maximo: 60+ (ms)\n");
	printf("tempo_consulta: 60+ (ms)\n");
	printf("tempo_transferencia: 60+ (ms)\n");
	printf("tempo_espera: 60+ (ms)\n");
	printf("numero_operacoes: 0+ (opcional)\n");
}

int	entrada_invalida(int argc, char **argv)
{
	int	i;

	if (argc == 8 && ft_atoi(argv[7]) <= 0)
		return (ENTRADA_INVALIDA);

	if (ft_atoi(argv[1]) < 1 || ft_atoi(argv[1]) > 10)
		return (ENTRADA_INVALIDA);

	if (ft_atoi(argv[2]) < 1 || ft_atoi(argv[2]) > 10)
		return (ENTRADA_INVALIDA);

	i = 2;
	while (++i < 7)
	{
		if (ft_atoi(argv[i]) < 60)
			return (ENTRADA_INVALIDA);
	}
	return (0);
}

int	ft_atoi(char *str)
{
	int		count;
	long	result;
	int		sign;

	count = 0;
	result = 0;
	sign = 1;
	while (str[count] == '\r' || str[count] == '\t' || str[count] == ' '
		|| str[count] == '\f' || str[count] == '\v' || str[count] == '\n')
		count++;
	if (str[count] == '-')
	{
		sign = -1;
		count++;
	}
	else if (str[count] == '+')
		count++;
	if (!(str[count] >= '0' && str[count] <= '9'))
		return (0);
	while (str[count] >= '0' && str[count] <= '9')
		result = result * 10 + (str[count++] - '0');
	return (result * sign);
}

int	entrada_eh_digito(int argc, char **argv)
{
	int	i;
	int	k;

	i = 1;
	while (i < argc)
	{
		k = 0;
		while (argv[i][k] != '\0')
		{
			if (argv[i][k] < '0' || argv[i][k] > '9')
				return (ENTRADA_INVALIDA);
			k++;
		}
		i++;
	}
	return (0);
}

int	verificar_entrada(int argc, char **argv)
{
	if (argc < 7)
		return (ENTRADA_INVALIDA);
	if (argc > 8)
		return (ENTRADA_INVALIDA);
	if (entrada_eh_digito(argc, argv) != 0)
		return (ENTRADA_INVALIDA);
	if (entrada_invalida(argc, argv))
		return (ENTRADA_INVALIDA);
	return (0);
}