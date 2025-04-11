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
	{
	    rules->must_eat = ft_atoi(av[5]);
		if (rules->must_eat < 0)
		{
			printf("ERlknROR\n");
			exit(1);	
		}
	}
	else
	    rules->must_eat = -1;
	struct timeval tv;
	gettimeofday(&tv, NULL);
	rules->start_time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    rules->is_dead = 0;
    pthread_mutex_init(&rules->death_lock, NULL);
}

long long timestamp(t_rules *rules)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000 + tv.tv_usec / 1000) - rules->start_time);
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
        rules->philos[i].last_meal = timestamp(rules);
		rules->philos[i].left_fork = &rules->forks[i];
		rules->philos[i].right_fork = &rules->forks[(i + 1) % rules->nb_philo];
		rules->philos[i].rules = rules;
		i++;
	}
    pthread_mutex_init(&rules->philos[i].meal_lock, NULL);
	pthread_mutex_init(&rules->print_lock, NULL);
}

void *philo_routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;

    while (1)
    {
        pthread_mutex_lock(&philo->rules->death_lock);
        if (philo->rules->is_dead)
        {
            pthread_mutex_unlock(&philo->rules->death_lock);
            break;
        }
        pthread_mutex_unlock(&philo->rules->death_lock);
        pthread_mutex_lock(&philo->rules->print_lock);
        printf("%lld %d is thinking\n", timestamp(philo->rules), philo->id);
        pthread_mutex_unlock(&philo->rules->print_lock);
        pthread_mutex_lock(philo->left_fork);
        pthread_mutex_lock(philo->right_fork);
        long long current_time = timestamp(philo->rules);
        pthread_mutex_lock(&philo->meal_lock);
        philo->last_meal = current_time;
        pthread_mutex_unlock(&philo->meal_lock);
        pthread_mutex_lock(&philo->rules->print_lock);
        printf("%lld %d is eating\n", current_time, philo->id);
        pthread_mutex_unlock(&philo->rules->print_lock);
        pthread_mutex_unlock(&philo->rules->print_lock);
        usleep(philo->rules->time_to_eat * 1000);
        philo->meals_eaten++;
        pthread_mutex_unlock(philo->left_fork);
        pthread_mutex_unlock(philo->right_fork);
        pthread_mutex_lock(&philo->rules->print_lock);
        printf("%lld %d is sleeping\n", timestamp(philo->rules), philo->id);
        pthread_mutex_unlock(&philo->rules->print_lock);
        usleep(philo->rules->time_to_sleep * 1000);
    }
    return (NULL);
}

void *monitor_death(void *arg)
{
    t_rules *rules = (t_rules *)arg;
    int i;
    long long time;

    while (1)
    {
        i = 0;
        while (i < rules->nb_philo)
        {
            pthread_mutex_lock(&rules->philos[i].meal_lock);
            time = timestamp(rules) - rules->philos[i].last_meal;
            pthread_mutex_unlock(&rules->philos[i].meal_lock);
            if (time > rules->time_to_die)
            {
                pthread_mutex_lock(&rules->print_lock);
                printf("%lld %d died\n", timestamp(rules), rules->philos[i].id);
                rules->is_dead = 1;
                pthread_mutex_unlock(&rules->print_lock);
                return (NULL);
            }
            i++;
        }
        usleep(1000);
    }
    return (NULL);
}

int main(int ac, char *av[])
{
    t_rules rules;
    int i;

    init_rules(ac, av, &rules);
    init_forks_and_philos(&rules);
    i = 0;
    while (i < rules.nb_philo)
    {
        if (pthread_create(&rules.philos[i].thread, NULL, philo_routine, &rules.philos[i]) != 0)
        {
            printf("Error creating thread for philosopher %d\n", i + 1);
            exit(1);
        }
        i++;
    }
    pthread_t monitor;
    pthread_create(&monitor, NULL, monitor_death, &rules);
    i = 0;
    while (i < rules.nb_philo)
    {
        if (pthread_join(rules.philos[i].thread, NULL) != 0)
        {
            printf("Error joining thread for philosopher %d\n", i + 1);
            exit(1);
        }
        i++;
    }
    i = 0;
    while (i < rules.nb_philo)
    {
        pthread_mutex_destroy(&rules.forks[i]);
        i++;
    }
    pthread_mutex_destroy(&rules.print_lock);
    pthread_mutex_destroy(&rules.philos[i].meal_lock);
    free(rules.forks);
    free(rules.philos);
    return 0;
}

