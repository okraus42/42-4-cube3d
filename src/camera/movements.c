/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movements.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 18:20:05 by plouda            #+#    #+#             */
/*   Updated: 2024/03/05 11:23:44 by plouda           ###   ########.fr       */
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
