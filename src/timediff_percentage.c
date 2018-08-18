/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timediff_percentage.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlutt <rlutt@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 14:36:16 by rlutt             #+#    #+#             */
/*   Updated: 2018/08/17 16:54:33 by rlutt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ping.h"

long double			time_diff_ms(struct timeval *then, struct timeval *now)
{
	long double x;

	x = (double)(then->tv_usec - now->tv_usec) / 1000.0L +
			(double)(then->tv_sec - now->tv_sec) * 1000.0L;
	return (x);
}

long double			time_diff_sec(struct timeval *then, struct timeval *now)
{
	return ((now->tv_sec + (1.0 / 1000000) * now->tv_usec) -
			(then->tv_sec + (1.0 / 1000000) * then->tv_usec));
}

float				get_percentage(size_t a, size_t b)
{
	size_t			diff;

	diff = a - b;
	if (diff == 0)
		return (0.00f);
	else
		return (((float)diff / a) * 100);
}
