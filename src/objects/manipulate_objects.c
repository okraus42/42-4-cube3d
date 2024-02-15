/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manipulate_objects.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 13:21:25 by plouda            #+#    #+#             */
/*   Updated: 2024/02/15 11:20:31 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	set_sphere_vects(t_sphere *sphere)
{
	t_vect3f	tmp;
	t_vect3f	*forward;
	t_vect3f	*right;
	t_vect3f	*up;

	forward = sphere->normal;
	right = sphere->right;
	up = sphere->up;
	tmp = (t_vect3f){0, 1, 0};
	if (forward->x == 0. && forward->y == 1. && forward->z == 0.)
	{
		*right = (t_vect3f){1, 0, 0};
		*up = (t_vect3f){0, 0, -1};
	}
	else if (forward->x == 0. && forward->y == -1 && forward->z == 0.)
	{
		*right = (t_vect3f){1, 0, 0};
		*up = (t_vect3f){0, 0, 1};
	}
	else
	{
		*right = cross_product(tmp, *forward);
		normalize(right);
		*up = cross_product(*forward, *right);
		normalize(up);
	}
}

void	set_plane_vects(t_plane *plane)
{
	t_vect3f	tmp;
	t_vect3f	*forward;
	t_vect3f	*right;
	t_vect3f	*up;

	forward = plane->normal;
	right = plane->right;
	up = plane->up;
	tmp = (t_vect3f){0, 1, 0};
	if (forward->x == 0. && forward->y == 1. && forward->z == 0.)
	{
		*right = (t_vect3f){1, 0, 0};
		*up = (t_vect3f){0, 0, -1};
	}
	else if (forward->x == 0. && forward->y == -1 && forward->z == 0.)
	{
		*right = (t_vect3f){1, 0, 0};
		*up = (t_vect3f){0, 0, 1};
	}
	else
	{
		*right = cross_product(tmp, *forward);
		normalize(right);
		*up = cross_product(*forward, *right);
		normalize(up);
	}
}

void	set_cylinder_vects(t_cylinder *cylinder)
{
	t_vect3f	tmp;
	t_vect3f	*forward;
	t_vect3f	*right;
	t_vect3f	*up;

	forward = cylinder->normal;
	right = cylinder->right;
	up = cylinder->up;
	tmp = (t_vect3f){0, 1, 0};
	if (forward->x == 0. && forward->y == 1. && forward->z == 0.)
	{
		*right = (t_vect3f){1, 0, 0};
		*up = (t_vect3f){0, 0, -1};
	}
	else if (forward->x == 0. && forward->y == -1 && forward->z == 0.)
	{
		*right = (t_vect3f){1, 0, 0};
		*up = (t_vect3f){0, 0, 1};
	}
	else
	{
		*right = cross_product(tmp, *forward);
		normalize(right);
		*up = cross_product(*forward, *right);
		normalize(up);
	}
}

void	set_disc_vects(t_disc *disc)
{
	t_vect3f	tmp;
	t_vect3f	*forward;
	t_vect3f	*right;
	t_vect3f	*up;

	forward = disc->normal;
	right = disc->right;
	up = disc->up;
	tmp = (t_vect3f){0, 1, 0};
	if (forward->x == 0. && forward->y == 1. && forward->z == 0.)
	{
		*right = (t_vect3f){1, 0, 0};
		*up = (t_vect3f){0, 0, -1};
	}
	else if (forward->x == 0. && forward->y == -1 && forward->z == 0.)
	{
		*right = (t_vect3f){1, 0, 0};
		*up = (t_vect3f){0, 0, 1};
	}
	else
	{
		*right = cross_product(tmp, *forward);
		normalize(right);
		*up = cross_product(*forward, *right);
		normalize(up);
	}
}

void	set_cone_vects(t_cone *cone)
{
	t_vect3f	tmp;
	t_vect3f	*forward;
	t_vect3f	*right;
	t_vect3f	*up;

	forward = cone->normal;
	right = cone->right;
	up = cone->up;
	tmp = (t_vect3f){0, 1, 0};
	if (forward->x == 0. && forward->y == 1. && forward->z == 0.)
	{
		*right = (t_vect3f){1, 0, 0};
		*up = (t_vect3f){0, 0, -1};
	}
	else if (forward->x == 0. && forward->y == -1 && forward->z == 0.)
	{
		*right = (t_vect3f){1, 0, 0};
		*up = (t_vect3f){0, 0, 1};
	}
	else
	{
		*right = cross_product(tmp, *forward);
		normalize(right);
		*up = cross_product(*forward, *right);
		normalize(up);
	}
}

void	rotate_o(keys_t key, t_vect3f *forward, t_vect3f *right, t_vect3f *up, t_camera *camera)
{
	/* if (key == MLX_KEY_A)
		pan(forward, right, camera->up, 5);
	else if (key == MLX_KEY_D)
		pan(forward, right, camera->up, -5);
	else if (key == MLX_KEY_W)
		tilt(forward, camera->right, up, 5);
	else if (key == MLX_KEY_S)
		tilt(forward, camera->right, up, -5);
	else if (key == MLX_KEY_Q)
		cant(camera->normal, right, up, 5);
	else if (key == MLX_KEY_E)
		cant(camera->normal, right, up, -5);
	normalize(forward);
	normalize(right);
	normalize(up); */
	(void)camera->coords;
	if (key == MLX_KEY_A)
		pan(forward, right, up, 5);
	else if (key == MLX_KEY_D)
		pan(forward, right, up, -5);
	else if (key == MLX_KEY_W)
		tilt(forward, right, up, 5);
	else if (key == MLX_KEY_S)
		tilt(forward, right, up, -5);
	else if (key == MLX_KEY_Q)
		cant(forward, right, up, 5);
	else if (key == MLX_KEY_E)
		cant(forward, right, up, -5);
	normalize(forward);
	normalize(right);
	normalize(up);
}

void	manipulate_sphere(t_rt *rt, t_sphere *sphere, mlx_key_data_t keydata)
{
	move(keydata.key, rt->camera, sphere->coords);
	rotate_o(keydata.key, sphere->normal, sphere->right, sphere->up, rt->camera);
}

void	manipulate_plane(t_rt *rt, t_plane *plane, mlx_key_data_t keydata)
{
	move(keydata.key, rt->camera, plane->coords);
	rotate_o(keydata.key, plane->normal, plane->right, plane->up, rt->camera);
}

void	manipulate_cylinder(t_rt *rt, t_cylinder *cylinder, mlx_key_data_t keydata)
{
	move(keydata.key, rt->camera, cylinder->coords);
	rotate_o(keydata.key, cylinder->normal, cylinder->right, cylinder->up, rt->camera);
	get_discs(cylinder);
}

void	manipulate_cone(t_rt *rt, t_cone *cone, mlx_key_data_t keydata)
{
	move(keydata.key, rt->camera, cone->coords);
	rotate_o(keydata.key, cone->normal, cone->right, cone->up, rt->camera);
	get_cone_discs(cone);
}

void	manipulate_highlighted_object(t_rt *rt, mlx_key_data_t keydata)
{
	int	i;

	i = 0;
	while (i < rt->n_spheres)
	{
		if (rt->spheres[i]->mode == HIGHLIGHT)
		{
			manipulate_sphere(rt, rt->spheres[i], keydata);
			return ;
		}
		i++;
	}
	i = 0;
	while (i < rt->n_planes)
	{
		if (rt->planes[i]->mode == HIGHLIGHT)
		{
			manipulate_plane(rt, rt->planes[i], keydata);
			return ;
		}
		i++;
	}
	i = 0;
	while (i < rt->n_cylinders)
	{
		if (rt->cylinders[i]->mode == HIGHLIGHT)
		{
			manipulate_cylinder(rt, rt->cylinders[i], keydata);
			return ;
		}
		i++;
	}
	i = 0;
	while (i < rt->n_cones)
	{
		if (rt->cones[i]->mode == HIGHLIGHT)
		{
			manipulate_cone(rt, rt->cones[i], keydata);
			return ;
		}
		i++;
	}
}

void	rotate_objects(t_master *master, mlx_key_data_t keydata)
{
	manipulate_highlighted_object(master->rt, keydata);
	find_rays(master);
}

void	specular_options(t_master *master, keys_t key)
{
	double	glossiness;
	double	highlight_size;

	glossiness = master->options->glossiness;
	highlight_size = master->options->spec_highlight_size;
	if (key == MLX_KEY_G)
	{
		glossiness += 0.05; 
		if (glossiness >= 1.01)
			glossiness = 0.05;
	}
	else if (key == MLX_KEY_H)
	{
		highlight_size /= 2;
		if (highlight_size <= 2)
			highlight_size = 1024;
	}
	master->options->glossiness = glossiness;
	master->options->spec_highlight_size = highlight_size;
}

void	manipulate_light(t_master *master, mlx_key_data_t keydata)
{
	int	i;

	i = 0;
	while (i < master->rt->n_lights)
	{
		if (master->rt->light_spheres[i]->mode == HIGHLIGHT)
			break ;
		i++;
	}
	move(keydata.key, master->rt->camera, master->rt->light_spheres[i]->coords);
	if (keydata.key == MLX_KEY_COMMA)
		master->options->light_intensity -= 15;
	else if (keydata.key == MLX_KEY_PERIOD)
		master->options->light_intensity += 15;
	clamp(0, INT_MAX - 100, &master->options->light_intensity);
	specular_options(master, keydata.key);
	printf("Light intensity: %d\n", master->options->light_intensity);
	printf("Glossiness: %.2f\n", master->options->glossiness);
	find_rays(master);
}