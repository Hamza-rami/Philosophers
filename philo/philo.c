/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrami <hrami@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 19:11:55 by hrami             #+#    #+#             */
/*   Updated: 2025/04/26 10:40:19 by hrami            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	help_philo_routine(t_philo *philo)
{
	if (!check_if_died(philo))
		return (0);
	if (!take_fork(philo))
		return (0);
	pthread_mutex_lock(&philo->rules->eat_mutex);
	philo->last_meal = timestamp(philo->rules);
	philo->n_eat++;
	if (!print_status(philo, "is eating"))
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		return (0);
	}
	pthread_mutex_unlock(&philo->rules->eat_mutex);
	return (1);
}

void	*philo_routine(void *arg)
{
	t_philo			*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(100);
	while (1)
	{
		if (philo->rules->must_eat != -1
			&& philo->n_eat >= philo->rules->must_eat)
			break ;
		if (!help_philo_routine(philo))
			return (NULL);
		ft_usleep(philo->rules->time_to_eat, philo->rules);
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		if (!print_status(philo, "is sleeping"))
			return (NULL);
		if (philo->rules->time_to_sleep > philo->rules->time_to_die)
			ft_usleep(philo->rules->time_to_die, philo->rules);
		else
			ft_usleep(philo->rules->time_to_sleep, philo->rules);
		if (!print_status(philo, "is thinking"))
			return (NULL);
	}
	return (NULL);
}

int	start_simulation(t_rules *rules)
{
	int			i;
	pthread_t	monitor_thread;

	i = 0;
	while (i < rules->nb_philo)
	{
		if (pthread_create(&rules->philos[i].thread, NULL,
				philo_routine, &rules->philos[i]) != 0)
			return (printf("Error creating thread for philosopher %d\n",
					i + 1), 0);
		i++;
	}
	if (pthread_create(&monitor_thread, NULL, monitor, rules) != 0)
		return (printf("Error creating monitor thread\n"), 0);
	i = -1;
	while (++i < rules->nb_philo)
		pthread_join(rules->philos[i].thread, NULL);
	pthread_join(monitor_thread, NULL);
	return (destroy_and_free(rules));
}

int	handle_1(t_rules *rules)
{
	long long	time;

	time = timestamp(rules) - rules->start_time;
	printf("%lld %d has take a fork\n", time, 1);
	usleep(rules->time_to_die * 1000);
	printf("%lld %d died\n", timestamp(rules) - rules->start_time, 1);
	return (0);
}

int	main(int ac, char *av[])
{
	t_rules		rules;

	if (!(init_rules(ac, av, &rules)))
		return (1);
	if (rules.nb_philo == 1)
		return (handle_1(&rules));
	if (!(init_forks_and_philos(&rules)))
		return (1);
	if (!start_simulation(&rules))
		return (1);
	return (0);
}
