/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrami <hrami@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 19:56:58 by hrami             #+#    #+#             */
/*   Updated: 2025/04/15 19:58:51 by hrami            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_forks_and_philos(t_rules *rules)
{
	int	i;

	rules->forks = malloc(sizeof(pthread_mutex_t) * rules->nb_philo);
	if (!rules->forks)
		return (0);
	rules->philos = malloc(sizeof(t_philo) * rules->nb_philo);
	if (!rules->philos)
		return (0);
	i = 0;
	while (i < rules->nb_philo)
	{
		rules->philos[i].id = i + 1;
		rules->philos[i].left_fork = &rules->forks[i];
		rules->philos[i].right_fork = &rules->forks[(i + 1) % rules->nb_philo];
		rules->philos[i].last_meal = timestamp(rules);
		rules->philos[i].rules = rules;
		rules->philos[i].n_eat = 0;
		i++;
	}
	pthread_mutex_init(&rules->print_lock, NULL);
	pthread_mutex_init(&rules->meals_lock, NULL);
	pthread_mutex_init(&rules->eat_mutex, NULL);
	pthread_mutex_init(&rules->if_died, NULL);
	pthread_mutex_init(&rules->died, NULL);
	return (1);
}

int	print_status(t_philo *philo, const char *msg)
{
	long long	time;

	pthread_mutex_lock(&philo->rules->print_lock);
	pthread_mutex_lock(&philo->rules->if_died);
	if (philo->rules->someone_died)
	{
		pthread_mutex_unlock(&philo->rules->if_died);
		pthread_mutex_unlock(&philo->rules->print_lock);
		return (0);
	}
	time = timestamp(philo->rules) - philo->rules->start_time;
	pthread_mutex_unlock(&philo->rules->if_died);
	printf("%lld %d %s\n", time, philo->id, msg);
	pthread_mutex_unlock(&philo->rules->print_lock);
	return (1);
}

int	check_if_died(t_philo *philo)
{
	pthread_mutex_lock(&philo->rules->if_died);
	if (philo->rules->someone_died)
	{
		pthread_mutex_unlock(&philo->rules->if_died);
		return (0);
	}
	pthread_mutex_unlock(&philo->rules->if_died);
	return (1);
}

int	take_fork(t_philo *philo, pthread_mutex_t **first, pthread_mutex_t **second)
{
	if (philo->left_fork < philo->right_fork)
	{
		*first = philo->left_fork;
		*second = philo->right_fork;
	}
	else
	{
		*first = philo->right_fork;
		*second = philo->left_fork;
	}
	pthread_mutex_lock(*first);
	pthread_mutex_lock(*second);
	if (!print_status(philo, "has taken a fork"))
	{
		pthread_mutex_unlock(*second);
		pthread_mutex_unlock(*first);
		return (0);
	}
	if (!print_status(philo, "has taken a fork"))
	{
		pthread_mutex_unlock(*second);
		pthread_mutex_unlock(*first);
		return (0);
	}
	return (1);
}
