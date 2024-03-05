/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movements2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 11:23:21 by plouda            #+#    #+#             */
/*   Updated: 2024/03/05 11:23:48 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	move_forward(t_camera *camera, double *coords)
{
	coords[X] -= (camera->normal->x * 1);
	coords[Y] -= (camera->normal->y * 1);
	coords[Z] -= (camera->normal->z * 1);
}

void	move_backward(t_camera *camera, double *coords)
{
	coords[X] += (camera->normal->x * 1);
	coords[Y] += (camera->normal->y * 1);
	coords[Z] += (camera->normal->z * 1);
}

void	move_up(t_camera *camera, double *coords)
{
	coords[X] += (camera->up->x * 1);
	coords[Y] += (camera->up->y * 1);
	coords[Z] += (camera->up->z * 1);
}

void	move_down(t_camera *camera, double *coords)
{
	coords[X] -= (camera->up->x * 1);
	coords[Y] -= (camera->up->y * 1);
	coords[Z] -= (camera->up->z * 1);
}
