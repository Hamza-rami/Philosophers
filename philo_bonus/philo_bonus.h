#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <semaphore.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>


struct	s_rules;
struct	s_philo;

typedef struct s_philo
{
	int				id;
	int				n_eat;
	long long		last_meal;
	pid_t			pid;
	struct s_rules	*rules;
}	t_philo;


typedef struct s_rules
{
	int			nb_philo;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	int			must_eat;
	long long	start_time;
	int			someone_died;
	t_philo		*philos;
	pid_t		*pids;
	sem_t		*forks;
	sem_t		*print_lock;
	sem_t		*dead_lock;
	sem_t		*meal_check;
}	t_rules;

long long	timestamp(t_rules *rules);

#endif