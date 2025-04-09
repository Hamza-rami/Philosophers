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
	else
	    rules->must_eat = -1;
}

int main(int argc, char const *argv[])
{
	pthread_mutex_t fork;
	pthread_mutex_init(&fork, NULL);
	pthread_mutex_lock(&fork);
	return 0;
}
