/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrami <hrami@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:32:00 by hrami             #+#    #+#             */
/*   Updated: 2025/04/23 10:04:03 by hrami            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	unlink_semaphores(void)
{
	sem_unlink("/forks");
	sem_unlink("/print");
	sem_unlink("/dead_lock");
	sem_unlink("/meal_lock");
}

int	init_semaphores(t_rules *rules)
{
	unlink_semaphores();
	rules->forks = sem_open("/forks", O_CREAT, 0644, rules->nb_philo);
	rules->print_lock = sem_open("/print", O_CREAT, 0644, 1);
	rules->dead_lock = sem_open("/dead_lock", O_CREAT, 0644, 1);
	rules->meal_check = sem_open("/meal_lock", O_CREAT, 0644, 1);
	if (rules->forks == SEM_FAILED || rules->print_lock == SEM_FAILED
		|| rules->dead_lock == SEM_FAILED || rules->meal_check == SEM_FAILED)
	{
		printf("ERROR: failed to open semaphores\n");
		return (0);
	}
	return (1);
}

int	init_philos(t_rules *rules)
{
	int	i;

	rules->philos = malloc(sizeof(t_philo) * rules->nb_philo);
	if (!rules->philos)
		return (0);
	i = 0;
	while (i < rules->nb_philo)
	{
		rules->philos[i].id = i + 1;
		rules->philos[i].last_meal = rules->start_time;
		rules->philos[i].n_eat = 0;
		rules->philos[i].rules = rules;
		i++;
	}
	return (1);
}

int	launch_philos(t_rules *rules)
{
	int			i;
	pthread_t	thread;

	rules->pids = malloc(sizeof(pid_t) * rules->nb_philo);
	if (!rules->pids)
		return (0);
	i = 0;
	while (i < rules->nb_philo)
	{
		rules->philos[i].last_meal = timestamp(rules);
		rules->pids[i] = fork();
		if (rules->pids[i] < 0)
			return (0);
		else if (rules->pids[i] == 0)
		{
			pthread_create(&thread, NULL, monitor, &rules->philos[i]);
			philo_routine(&rules->philos[i]);
			pthread_join(thread, NULL);
			cleanup(rules);
			exit(0);
		}
		i++;
	}
	return (1);
}

int	main(int ac, char **av)
{
	t_rules	rules;

	if (!init_rules(ac, av, &rules))
		return (1);
	if (!init_semaphores(&rules))
		return (1);
	if (!init_philos(&rules))
		return (1);
	if (!launch_philos(&rules))
		return (1);
	wait_processes(&rules);
	cleanup(&rules);
	return (0);
}
