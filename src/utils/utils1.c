/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 12:20:14 by plouda            #+#    #+#             */
/*   Updated: 2023/12/17 17:56:08 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

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
