/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrami <hrami@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 18:55:06 by hrami             #+#    #+#             */
/*   Updated: 2025/04/24 15:48:37 by hrami            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdbool.h>
# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>
# include <sys/time.h>

struct	s_rules;
struct	s_philo;

typedef struct s_philo
{
	int				id;
	long long		last_meal;
	int				n_eat;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	struct s_rules	*rules;
}	t_philo;

typedef struct s_rules
{
	int				nb_philo;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat;
	int				someone_died;
	long long		time_to_die;
	long long		start_time;
	pthread_mutex_t	meals_lock;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	eat_mutex;
	pthread_mutex_t	if_died;
	pthread_mutex_t	died;
	t_philo			*philos;
}	t_rules;

int			ft_atoi(char *str);
int			init_rules(int ac, char *av[], t_rules *rules);
long long	timestamp(t_rules *rules);
int			init_forks_and_philos(t_rules *rules);
int			print_status(t_philo *philo, const char *msg);
int			check_if_died(t_philo *philo);
int			take_fork(t_philo *philo);
int			destroy_and_free(t_rules *rules);
void		*monitor(void *arg);
void		ft_usleep(long time, t_rules *rules);

#endif