/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movements.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 18:20:05 by plouda            #+#    #+#             */
/*   Updated: 2024/01/10 12:43:35 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	move(keys_t key, t_camera *camera, double *coords)
{
	if (key == MLX_KEY_RIGHT)
		move_right(camera, coords);
	if (key == MLX_KEY_LEFT)
		move_left(camera, coords);
	if (key == MLX_KEY_UP)
		move_forward(camera, coords);
	if (key == MLX_KEY_DOWN)
		move_backward(camera, coords);
	if (key == MLX_KEY_PAGE_UP)
		move_up(camera, coords);
	if (key == MLX_KEY_PAGE_DOWN)
		move_down(camera, coords);
}

void	move_right(t_camera *camera, double *coords)
{
	coords[X] += (camera->right->x * 1);
	coords[Y] += (camera->right->y * 1);
	coords[Z] += (camera->right->z * 1);
}

void	move_left(t_camera *camera, double *coords)
{
	coords[X] -= (camera->right->x * 1);
	coords[Y] -= (camera->right->y * 1);
	coords[Z] -= (camera->right->z * 1);
}

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
