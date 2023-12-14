/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 12:20:14 by plouda            #+#    #+#             */
/*   Updated: 2023/12/14 18:04:53 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minirt.h"

void	ft_swapf(double *a, double *b)
{
	double	temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

t_vect3f	array_to_vect(double *array)
{
	t_vect3f	vect;

	vect.x = array[X];
	vect.y = array[Y];
	vect.z = array[Z];
	return (vect);
}
