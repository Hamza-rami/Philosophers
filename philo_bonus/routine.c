/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrami <hrami@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:46:32 by hrami             #+#    #+#             */
/*   Updated: 2025/04/24 15:07:59 by hrami            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	philo_routine(t_philo *philo)
{
	if (philo->id % 2 == 0)
		usleep(50);
	while (1)
	{
		sem_wait(philo->rules->forks);
		sem_wait(philo->rules->forks);
		print("has taken a fork", philo);
		print("has taken a fork", philo);
		sem_wait(philo->rules->meal_check);
		philo->last_meal = timestamp(philo->rules);
		sem_post(philo->rules->meal_check);
		print("is eating", philo);
		ft_usleep(philo->rules->time_to_eat, philo->rules);
		sem_post(philo->rules->forks);
		sem_post(philo->rules->forks);
		philo->n_eat++;
		print("is sleeping", philo);
		ft_usleep(philo->rules->time_to_sleep, philo->rules);
		print("is thinking", philo);
		if (philo->rules->must_eat != -1 && philo->n_eat
			>= philo->rules->must_eat)
			return (cleanup(philo->rules), exit(1));
	}
}

void	ft_usleep(long time, t_rules *rules)
{
	long	start;

	start = timestamp(rules);
	while ((timestamp(rules) - start) < time)
		usleep(500);
}
