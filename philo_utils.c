#include    "philosophers.h"

static void	ft_skip(char const *str, int *s, int *i)
{
	while (str[*i] == ' ' || (str[*i] >= 9 && str[*i] <= 13))
		(*i)++;
	if ((str[*i] == '+' || str[*i] == '-'))
	{
		if (str[*i] == '-')
			*s *= -1;
		(*i)++;
	}
}

int	ft_atoi(char *str)
{
	int		i;
	int		s;
	long	r;

	i = 0;
	s = 1;
	r = 0;
	ft_skip(str, &s, &i);
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		if (r >= (9223372036854775807 / 10))
		{
			if (r > 922337203685477580
				|| (r == 922337203685477580 && str[i] - '0' > 7))
			{
				if (s == 1)
					return (-1);
				return (0);
			}
		}
		r = r * 10 + (str[i] - '0');
		i++;
	}
	if (str[i] != '\0')
	{
		printf("ERROR\n");
		exit(1);
	}
	return (r * s);
}
