/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_properties.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 12:19:21 by plouda            #+#    #+#             */
/*   Updated: 2024/03/05 12:26:10 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	rotate_o(keys_t key, t_vect33f *axes, t_camera *camera)
{
	(void)camera->coords;
	if (key == MLX_KEY_A)
		pan(axes->normal, axes->right, axes->up, 5);
	else if (key == MLX_KEY_D)
		pan(axes->normal, axes->right, axes->up, -5);
	else if (key == MLX_KEY_W)
		tilt(axes->normal, axes->right, axes->up, 5);
	else if (key == MLX_KEY_S)
		tilt(axes->normal, axes->right, axes->up, -5);
	else if (key == MLX_KEY_Q)
		cant(axes->normal, axes->right, axes->up, 5);
	else if (key == MLX_KEY_E)
		cant(axes->normal, axes->right, axes->up, -5);
	normalize(axes->normal);
	normalize(axes->right);
	normalize(axes->up);
}

void	change_glossiness(double *glossiness, keys_t key)
{
	if (key == MLX_KEY_G)
	{
		*glossiness += 0.05;
		if (*glossiness >= 1.01)
			*glossiness = 0.05;
	}
}

void	change_radius(double *radius, keys_t key)
{
	if (key == MLX_KEY_1)
		*radius += 0.5;
	else if (key == MLX_KEY_2)
		*radius -= 0.5;
	if (*radius < PRECISION)
		*radius = 0.1;
}

void	change_height(double *height, keys_t key)
{
	if (key == MLX_KEY_3)
		*height += 1;
	else if (key == MLX_KEY_4)
		*height -= 1;
	if (*height < PRECISION)
		*height = 0.1;
}
