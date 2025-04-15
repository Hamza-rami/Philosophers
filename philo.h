#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdbool.h>
# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_philo
{
	int             id;
	long long       last_meal;
	pthread_t       thread;
	pthread_mutex_t *left_fork;
	pthread_mutex_t *right_fork;
	struct s_rules  *rules;
	int				n_eat;
} t_philo;

typedef struct s_rules
{
	int             nb_philo;
	int             time_to_die;
	int             time_to_eat;
	int             time_to_sleep;
	int             must_eat;
	int             someone_died;
	long long       start_time;
	int             tottal_eat;
	pthread_mutex_t meals_lock;
	pthread_mutex_t *forks;
	pthread_mutex_t print_lock;
	pthread_mutex_t eat_mutex;
	pthread_mutex_t if_died;
	pthread_mutex_t died;
	t_philo         *philos;
} t_rules;




int	ft_atoi(char *str);

#endif