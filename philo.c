#include "philo.h"

int init_rules(int ac, char *av[], t_rules *rules)
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
	if (rules->nb_philo < 0 || rules->time_to_die < 0 || rules->time_to_eat < 0 || rules->time_to_sleep < 0)
	{
		printf("ERROR\n");
		return (1);
	}
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
	struct timeval tv;
	gettimeofday(&tv, NULL);
	rules->start_time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	rules->someone_died = 0;
	rules->tottal_eat = 0;
	return (1) ;
}

long long timestamp(t_rules *rules)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000 + tv.tv_usec / 1000) - rules->start_time);
}

int	init_forks_and_philos(t_rules *rules)
{
	int i;

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
	time = timestamp(philo->rules);
	pthread_mutex_unlock(&philo->rules->if_died);
	printf("%lld ms %d %s\n", time, philo->id, msg);
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

void	*philo_routine(void *arg)
{
	t_philo *philo = (t_philo *)arg;
	pthread_mutex_t *first = NULL;
	pthread_mutex_t *second = NULL;

	while (1)
	{
		if (philo->rules->must_eat != -1 && philo->n_eat >= philo->rules->must_eat)
			break ;
		if (!check_if_died(philo))
			return (NULL);
		if (!take_fork(philo, &first, &second))
			return (0);
		pthread_mutex_lock(&philo->rules->eat_mutex);
		philo->last_meal = timestamp(philo->rules);
		philo->n_eat++;
		pthread_mutex_unlock(&philo->rules->eat_mutex);
		if (!print_status(philo, "is eating"))
		{
			pthread_mutex_unlock(second);
			pthread_mutex_unlock(first);
			return (NULL);
		}
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

void *monitor(void *arg)
{
	t_rules *rules = (t_rules *)arg;
	int i;
	long long current_time;
	int	full;

	while (!rules->someone_died)
	{
		i = 0;
		full = 0;
		while (i < rules->nb_philo)
		{
			current_time = timestamp(rules);
			pthread_mutex_lock(&rules->eat_mutex);
			if (current_time - rules->philos[i].last_meal > rules->time_to_die
				&& rules->philos[i].n_eat < rules->must_eat)				
			{
				pthread_mutex_unlock(&rules->eat_mutex);
				pthread_mutex_lock(&rules->print_lock);
				printf("%lld %d died\n", current_time, rules->philos[i].id);
				pthread_mutex_unlock(&rules->print_lock);
				pthread_mutex_lock(&rules->if_died);
				rules->someone_died = 1;
				pthread_mutex_unlock(&rules->if_died);
				return (NULL);
			}
			if (rules->philos[i].n_eat >= rules->must_eat)
				full++;
			pthread_mutex_unlock(&rules->eat_mutex);
			i++;
		}      
		if (full == rules->nb_philo)
			break ;
		usleep(1000);
	}
	return (NULL);
}


int main(int ac, char *av[])
{
	t_rules rules;
	int i;

	if (!(init_rules(ac, av, &rules)))
		return (1);
	if (!(init_forks_and_philos(&rules)))
		return (1);
	pthread_t monitor_thread;
	if (pthread_create(&monitor_thread, NULL, monitor, &rules) != 0)
		return (printf("Error creating monitor thread\n"), 1);
	i = 0;
	while (i < rules.nb_philo)
	{
		if (pthread_create(&rules.philos[i].thread, NULL, philo_routine, &rules.philos[i]) != 0)
			return (printf("Error creating thread for philosopher %d\n", i + 1), 1);
		i++;
	}
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


