/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movements.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 18:20:05 by plouda            #+#    #+#             */
/*   Updated: 2023/12/14 18:20:34 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minirt.h"

void	move_right(t_camera *camera)
{
	camera->coords[X] += (camera->right->x * 3);
	camera->coords[Y] += (camera->right->y * 3);
	camera->coords[Z] += (camera->right->z * 3);
}

void	move_left(t_camera *camera)
{
	camera->coords[X] -= (camera->right->x * 3);
	camera->coords[Y] -= (camera->right->y * 3);
	camera->coords[Z] -= (camera->right->z * 3);
}

void	move_forward(t_camera *camera)
{
	camera->coords[X] -= (camera->normal->x * 3);
	camera->coords[Y] -= (camera->normal->y * 3);
	camera->coords[Z] -= (camera->normal->z * 3);
}

void	move_backward(t_camera *camera)
{
	camera->coords[X] += (camera->normal->x * 3);
	camera->coords[Y] += (camera->normal->y * 3);
	camera->coords[Z] += (camera->normal->z * 3);
}

void	move_up(t_camera *camera)
{
	camera->coords[X] += (camera->up->x * 3);
	camera->coords[Y] += (camera->up->y * 3);
	camera->coords[Z] += (camera->up->z * 3);
}

void	move_down(t_camera *camera)
{
	camera->coords[X] -= (camera->up->x * 3);
	camera->coords[Y] -= (camera->up->y * 3);
	camera->coords[Z] -= (camera->up->z * 3);
}
