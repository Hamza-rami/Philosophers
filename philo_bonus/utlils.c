#include "philo_bonus.h"

long long	timestamp(t_rules *rules)
{
	struct timeval	tv;

	(void)rules;
	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000 + tv.tv_usec / 1000));
}