/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrami <hrami@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 10:03:03 by hrami             #+#    #+#             */
/*   Updated: 2025/04/24 15:49:00 by hrami            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	not_full(t_philo *philo, t_rules *rules)
{
	if (rules->must_eat == -1)
		return (true);
	if (philo->n_eat < rules->must_eat)
		return (true);
	return (false);
}

void	print_death(t_rules *rules, long long time, int i)
{
	pthread_mutex_lock(&rules->print_lock);
	printf("%lld %d died\n", time - rules->start_time, rules->philos[i].id);
	pthread_mutex_unlock(&rules->print_lock);
	pthread_mutex_lock(&rules->if_died);
	rules->someone_died = 1;
	pthread_mutex_unlock(&rules->if_died);
}

int	check_philos(t_rules *rules)
{
	int			i;
	long long	current_time;
	int			full;

	i = 0;
	full = 0;
	while (i < rules->nb_philo)
	{
		pthread_mutex_lock(&rules->eat_mutex);
		current_time = timestamp(rules);
		if (current_time - rules->philos[i].last_meal > rules->time_to_die
			&& not_full(&rules->philos[i], rules))
		{
			pthread_mutex_unlock(&rules->eat_mutex);
			print_death(rules, current_time, i);
			return (-1);
		}
		if (rules->must_eat != -1 && rules->philos[i].n_eat >= rules->must_eat)
			full++;
		pthread_mutex_unlock(&rules->eat_mutex);
		i++;
	}
	return (full);
}

void	*monitor(void *arg)
{
	t_rules	*rules;
	int		full;

	rules = (t_rules *)arg;
	while (1)
	{
		full = check_philos(rules);
		if (full == -1 || full == rules->nb_philo)
			return (NULL);
	}
	return (NULL);
}

void	ft_usleep(long time, t_rules *rules)
{
	long	start;

	start = timestamp(rules);
	while ((timestamp(rules) - start) < time)
		usleep(500);
}
