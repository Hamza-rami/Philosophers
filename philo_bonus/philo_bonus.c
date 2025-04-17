# include "philo_bonus.h"

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

int	init_semaphores(t_rules *rules)
{
	unlink_semaphores();
	rules->forks = sem_open("/forks", O_CREAT, 0644, rules->nb_philo);
	rules->print_lock = sem_open("/print", O_CREAT, 0644, 1);
	rules->dead_lock = sem_open("/dead_lock", O_CREAT, 0644, 1);
	rules->meal_check = sem_open("/meal_lock", O_CREAT, 0644, 1);
	if (rules->forks == SEM_FAILED || rules->print_lock == SEM_FAILED
		|| rules->dead_lock == SEM_FAILED || rules->meal_check== SEM_FAILED)
	{
		printf("ERROR: failed to open semaphores\n");
		return (0);
	}
	return (1);
}


void	unlink_semaphores(void)
{
	sem_unlink("/forks");
	sem_unlink("/print");
	sem_unlink("/dead_lock");
	sem_unlink("/meal_lock");
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
	int	i;

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
			philo_routine(&rules->philos[i]);
			exit(0);
		}
		i++;
	}
	return (1);
}

void	philo_routine(t_philo *philo)
{
	while (1)
	{
		sem_wait(philo->rules->forks);
		print("has taken a fork");
		sem_wait(philo->rules->forks);
		print("has taken a fork");
		sem_wait(philo->rules->meal_check);
		philo->last_meal = timestamp(philo->rules);
		sem_post(philo->rules->meal_check);
		print("is eating");
		sleep(philo->rules->time_to_eat);
		philo->n_eat++;
		sem_post(philo->rules->forks);
		sem_post(philo->rules->forks);
		print("is sleeping");
		sleep(philo->rules->time_to_sleep);
		print("is thinking");
		if (philo->rules->must_eat != -1 && philo->n_eat >= philo->rules->must_eat)
			exit(0);
	}
}

void *monitor(void *arg)
{
    t_rules *rules;
    int full;
    int i;
    long long current_time;

	rules = (t_rules *)arg;
	while (1)
    {
        full = 0;
        i = 0;
	}


}
