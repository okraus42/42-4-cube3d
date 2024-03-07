/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 11:00:06 by plouda            #+#    #+#             */
/*   Updated: 2024/03/06 16:01:37 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	camera_movements(t_master *master, mlx_key_data_t keydata)
{
	if (master->options->mode == DEFAULT
		&& !keydata.modifier && keydata.action != MLX_RELEASE
		&& (keydata.key == MLX_KEY_RIGHT
			|| keydata.key == MLX_KEY_LEFT
			|| keydata.key == MLX_KEY_UP
			|| keydata.key == MLX_KEY_DOWN
			|| keydata.key == MLX_KEY_MINUS
			|| keydata.key == MLX_KEY_EQUAL
			|| keydata.key == MLX_KEY_PAGE_UP
			|| keydata.key == MLX_KEY_PAGE_DOWN))
		shift_camera(master, keydata);
	else if (master->options->mode == DEFAULT
		&& !keydata.modifier && keydata.action != MLX_RELEASE
		&& (keydata.key == MLX_KEY_A
			|| keydata.key == MLX_KEY_D
			|| keydata.key == MLX_KEY_W
			|| keydata.key == MLX_KEY_S
			|| keydata.key == MLX_KEY_Q
			|| keydata.key == MLX_KEY_E))
		rotate_camera(master, keydata);
}

void	light_movements(t_master *master, mlx_key_data_t keydata)
{
	if ((master->options->mode == LIGHTING)
		&& keydata.modifier == MLX_SHIFT
		&& keydata.key == MLX_KEY_UP
		&& keydata.action != MLX_RELEASE)
		choose_object(master);
	else if (master->options->mode == LIGHTING
		&& keydata.action != MLX_RELEASE
		&& !keydata.modifier
		&& (keydata.key == MLX_KEY_RIGHT || keydata.key == MLX_KEY_LEFT
			|| keydata.key == MLX_KEY_UP || keydata.key == MLX_KEY_DOWN
			|| keydata.key == MLX_KEY_PAGE_UP
			|| keydata.key == MLX_KEY_PAGE_DOWN
			|| keydata.key == MLX_KEY_COMMA || keydata.key == MLX_KEY_PERIOD
			|| keydata.key == MLX_KEY_G || keydata.key == MLX_KEY_H))
		manipulate_light(master, keydata);
	else if (master->options->mode == DEFAULT
		&& keydata.modifier == MLX_CONTROL
		&& keydata.key == MLX_KEY_L
		&& keydata.action != MLX_RELEASE)
	{
		master->options->mode = LIGHTING;
		if (master->rt->n_lights > 0)
			master->rt->light_spheres[0]->mode = HIGHLIGHT;
	}
}

void	object_movements(t_master *master, mlx_key_data_t keydata)
{
	if (keydata.modifier == MLX_CONTROL
		&& keydata.key == MLX_KEY_O && keydata.action != MLX_RELEASE
		&& master->options->mode == DEFAULT)
	{
		master->options->mode = OBJECT_CHOICE;
		choose_object(master);
	}
	else if ((master->options->mode == OBJECT_CHOICE
			|| master->options->mode == HIGHLIGHT)
		&& keydata.modifier == MLX_SHIFT && keydata.key == MLX_KEY_UP
		&& keydata.action != MLX_RELEASE)
		choose_object(master);
	else if (master->options->mode == HIGHLIGHT
		&& keydata.action != MLX_RELEASE && !keydata.modifier
		&& (keydata.key == MLX_KEY_RIGHT || keydata.key == MLX_KEY_LEFT
			|| keydata.key == MLX_KEY_UP || keydata.key == MLX_KEY_DOWN
			|| keydata.key == MLX_KEY_PAGE_UP
			|| keydata.key == MLX_KEY_PAGE_DOWN
			|| keydata.key == MLX_KEY_A || keydata.key == MLX_KEY_D
			|| keydata.key == MLX_KEY_W || keydata.key == MLX_KEY_S
			|| keydata.key == MLX_KEY_Q || keydata.key == MLX_KEY_E
			|| keydata.key == MLX_KEY_G || keydata.key == MLX_KEY_0
			|| keydata.key == MLX_KEY_1 || keydata.key == MLX_KEY_2
			|| keydata.key == MLX_KEY_3 || keydata.key == MLX_KEY_4))
		manipulate_objects(master, keydata);
}

t_rayfinder	trace_singular_object_ray(t_master *master,
	int32_t xpos, int32_t ypos)
{
	t_rayfinder	rf;
	t_ray		*ray;

	ray = malloc(sizeof(t_ray));
	update_camera_matrix(master->rt->camera);
	rf = init_rayfinder(master);
	update_ray_direction(&rf, ray, xpos, ypos);
	find_intersections(master, *ray, &rf, PRIMARY);
	free(ray);
	return (rf);
}
