/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrami <hrami@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 19:11:55 by hrami             #+#    #+#             */
/*   Updated: 2025/04/15 19:57:44 by hrami            ###   ########.fr       */
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

bool	not_full(t_philo *philo, t_rules *rules)
{
	if (rules->must_eat == -1)
		return (true);
	if (philo->n_eat < rules->must_eat)
		return (true);
	return (false);
}

void	*monitor(void *arg)
{
	t_rules		*rules;
	int			i;
	long long	current_time;
	int			full;

	rules = (t_rules *)arg;
	while (1)
	{
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
				pthread_mutex_lock(&rules->print_lock);
				printf("%lld %d died\n", current_time - rules->start_time,
					rules->philos[i].id);
				pthread_mutex_unlock(&rules->print_lock);
				pthread_mutex_lock(&rules->if_died);
				rules->someone_died = 1;
				pthread_mutex_unlock(&rules->if_died);
				return (NULL);
			}
			if (rules->must_eat != -1 && rules->philos[i].n_eat
				>= rules->must_eat)
				full++;
			pthread_mutex_unlock(&rules->eat_mutex);
			i++;
		}
		if (full == rules->nb_philo)
			break ;
	}
	return (NULL);
}

int	main(int ac, char *av[])
{
	t_rules		rules;
	int			i;
	pthread_t	monitor_thread;

	if (!(init_rules(ac, av, &rules)))
		return (1);
	if (!(init_forks_and_philos(&rules)))
		return (1);
	i = 0;
	while (i < rules.nb_philo)
	{
		if (pthread_create(&rules.philos[i].thread, NULL,
				philo_routine, &rules.philos[i]) != 0)
			return (printf("Error creating thread for philosopher %d\n",
					i + 1), 1);
		i++;
	}
	if (pthread_create(&monitor_thread, NULL, monitor, &rules) != 0)
		return (printf("Error creating monitor thread\n"), 1);
	i = 0;
	while (i < rules.nb_philo)
		pthread_join(rules.philos[i++].thread, NULL);
	pthread_join(monitor_thread, NULL);
	i = 0;
	while (i < rules.nb_philo)
		pthread_mutex_destroy(&rules.forks[i++]);
	pthread_mutex_destroy(&rules.print_lock);
	pthread_mutex_destroy(&rules.eat_mutex);
	pthread_mutex_destroy(&rules.meals_lock);
	pthread_mutex_destroy(&rules.if_died);
	pthread_mutex_destroy(&rules.died);
	free(rules.forks);
	free(rules.philos);
	return (0);
}
