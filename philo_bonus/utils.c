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
		waitpid(rules->pids[i], &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) == 1)
		{
			kill_all(rules);
			break;
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

static void	ft_skip(char const *str, int *s, int *i)
{
	while (str[*i] == ' ' || (str[*i] >= 9 && str[*i] <= 13))
		(*i)++;
	if ((str[*i] == '+' || str[*i] == '-'))
	{
		if (str[*i] == '-')
			*s *= -1;
		(*i)++;
	}
}

int	ft_atoi(char *str)
{
	int		i;
	int		s;
	long	r;

	i = 0;
	s = 1;
	r = 0;
	ft_skip(str, &s, &i);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (-1);
		r = r * 10 + (str[i] - '0');
		if (r * s > 2147483647 || r * s < -2147483648)
			return (-1);
		i++;
	}
	return (r * s);
}