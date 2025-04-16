/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrami <hrami@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 19:11:55 by hrami             #+#    #+#             */
/*   Updated: 2025/04/16 10:52:37 by hrami            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	help_philo_routine(t_philo *philo, pthread_mutex_t **first,
	pthread_mutex_t **second)
{
	if (!check_if_died(philo))
		return (0);
	if (!take_fork(philo, first, second))
		return (0);
	pthread_mutex_lock(&philo->rules->eat_mutex);
	philo->last_meal = timestamp(philo->rules);
	philo->n_eat++;
	if (!print_status(philo, "is eating"))
	{
		pthread_mutex_unlock(*second);
		pthread_mutex_unlock(*first);
		return (0);
	}
	pthread_mutex_unlock(&philo->rules->eat_mutex);
	return (1);
}

void	*philo_routine(void *arg)
{
	t_philo			*philo;
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	philo = (t_philo *)arg;
	first = NULL;
	second = NULL;
	while (1)
	{
		if (philo->rules->must_eat != -1
			&& philo->n_eat >= philo->rules->must_eat)
			break ;
		if (!help_philo_routine(philo, &first, &second))
			return (NULL);
		usleep(philo->rules->time_to_eat * 1000);
		pthread_mutex_unlock(second);
		pthread_mutex_unlock(first);
		if (!print_status(philo, "is sleeping"))
			return (NULL);
		usleep(philo->rules->time_to_sleep * 1000);
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

int	main(int ac, char *av[])
{
	t_rules		rules;

	if (!(init_rules(ac, av, &rules)))
		return (1);
	if (!(init_forks_and_philos(&rules)))
		return (1);
	if (!start_simulation(&rules))
		return (1);
	return (0);
}
