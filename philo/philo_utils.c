/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrami <hrami@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 19:00:12 by hrami             #+#    #+#             */
/*   Updated: 2025/04/16 11:46:50 by hrami            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include    "philo.h"

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
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (-1);
		r = r * 10 + (str[i] - '0');
		if (r * s > 2147483647 || r * s < -2147483648)
			return (-1);
		i++;
	}
	return (r * s);
}

int	help_init_rules(int ac, char *av[], t_rules *rules)
{
	struct timeval	tv;

	if (ac == 6)
	{
		rules->must_eat = ft_atoi(av[5]);
		if (rules->must_eat < 0)
		{
			printf("ERROR\n");
			return (0);
		}
	}
	else
		rules->must_eat = -1;
	gettimeofday(&tv, NULL);
	rules->start_time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	rules->someone_died = 0;
	return (1);
}

int	init_rules(int ac, char *av[], t_rules *rules)
{
	if (ac != 5 && ac != 6)
	{
		printf("ERROR : enter exact argument\n");
		return (0);
	}
	rules->nb_philo = ft_atoi(av[1]);
	rules->time_to_die = ft_atoi(av[2]);
	rules->time_to_eat = ft_atoi(av[3]);
	rules->time_to_sleep = ft_atoi(av[4]);
	if (rules->nb_philo < 0 || rules->time_to_die < 0
		|| rules->time_to_eat < 0 || rules->time_to_sleep < 0)
	{
		printf("ERROR\n");
		return (0);
	}
	if (!help_init_rules(ac, av, rules))
		return (0);
	return (1);
}

long long	timestamp(t_rules *rules)
{
	struct timeval	tv;

	(void)rules;
	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000 + tv.tv_usec / 1000));
}
