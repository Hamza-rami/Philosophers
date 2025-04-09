#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>


typedef struct s_philo
{
    int             id;
    int             meals_eaten;
    long long       last_meal;
    pthread_t       thread;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    struct s_rules  *rules;
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
    pthread_mutex_t *forks;
    pthread_mutex_t print_lock;
    t_philo         *philos;
} t_rules;


#endif