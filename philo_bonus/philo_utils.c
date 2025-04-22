/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrami <hrami@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:40:48 by hrami             #+#    #+#             */
/*   Updated: 2025/04/21 14:41:51 by hrami            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

long long	timestamp(t_rules *rules)
{
	struct timeval	tv;

	(void)rules;
	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000 + tv.tv_usec / 1000));
}

void	print(char *str, t_philo *philo)
{
	long long	time;

	sem_wait(philo->rules->print_lock);
	time = timestamp(philo->rules);
	sem_wait(philo->rules->dead_lock);
	if (!philo->rules->someone_died)
		printf("%lld %d %s\n", time - philo->rules->start_time, philo->id, str);
	sem_post(philo->rules->dead_lock);
	sem_post(philo->rules->print_lock);
}

void	kill_all(t_rules *rules)
{
	int	i;

	i = 0;
	while (i < rules->nb_philo)
	{
		kill(rules->pids[i], SIGKILL);
		i++;
	}
}

void	wait_processes(t_rules *rules)
{
	int	i;
	int	status;

	i = 0;
	while (i < rules->nb_philo)
	{
		waitpid(-1, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) == 1)
		{
			kill_all(rules);
			break ;
		}
		i++;
	}
}

void	cleanup(t_rules *rules)
{
	sem_close(rules->forks);
	sem_close(rules->print_lock);
	sem_close(rules->dead_lock);
	sem_close(rules->meal_check);
	sem_unlink("/forks");
	sem_unlink("/print");
	sem_unlink("/dead_lock");
	sem_unlink("/meal_lock");
	free(rules->pids);
	free(rules->philos);
}
