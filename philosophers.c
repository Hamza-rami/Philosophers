# include "philosophers.h"

void    init_rules(int ac, char *av[], t_rules *rules)
{
	if (ac != 5 && ac != 6)
	{
		printf("ERROR : enter exact argument\n");
		exit(1);
	}
	rules->nb_philo = ft_atoi(av[1]);
	rules->time_to_die = ft_atoi(av[2]);
	rules->time_to_eat = ft_atoi(av[3]);
	rules->time_to_sleep = ft_atoi(av[4]);
	if (rules->nb_philo < 0 || rules->time_to_die < 0 || rules->time_to_eat < 0 || rules->time_to_sleep < 0)
	{
		printf("ERROR\n");
		exit(1);
	}
	if (ac == 6)
	    rules->must_eat = ft_atoi(av[5]);
	if (rules->must_eat < 0)
	{
		printf("ERROR\n");
		exit(1);	
	}
	else
	    rules->must_eat = -1;
	struct timeval tv;
	gettimeofday(&tv, NULL);
	rules->start_time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void	init_forks_and_philos(t_rules *rules)
{
	int	i;

	rules->forks = malloc(sizeof(pthread_mutex_t) * rules->nb_philo);
	if (!rules->forks)
		exit(1);
	i = 0;
	while (i < rules->nb_philo)
		pthread_mutex_init(&rules->forks[i++], NULL);
	rules->philos = malloc(sizeof(t_philo) * rules->nb_philo);
	if (!rules->philos)
		exit(1);
	i = 0;
	while (i < rules->nb_philo)
	{
		rules->philos[i].id = i + 1;
		rules->philos[i].meals_eaten = 0;
		rules->philos[i].last_meal = 0;
		rules->philos[i].left_fork = &rules->forks[i];
		rules->philos[i].right_fork = &rules->forks[(i + 1) % rules->nb_philo];
		rules->philos[i].rules = rules;
		i++;
	}
	pthread_mutex_init(&rules->print_lock, NULL);
}

void *philo_routine(void *arg)
{
	t_philo *philo = (t_philo *)arg;

	pthread_mutex_lock(philo->left_fork);
	pthread_mutex_lock(philo->right_fork);
	printf("%lld %d is eating\n", timestamp(philo->rules), philo->id);
	usleep(philo->rules->time_to_eat * 1000);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	return (NULL);
}

long long timestamp(t_rules *rules)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000 + tv.tv_usec / 1000) - rules->start_time);
}


