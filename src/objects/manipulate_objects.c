/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manipulate_objects.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 13:21:25 by plouda            #+#    #+#             */
/*   Updated: 2024/02/28 15:59:50 by okraus           ###   ########.fr       */
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
	sphere->q = get_obj_quat(*(sphere->normal), *(sphere->up));
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
	plane->q = get_obj_quat(*(plane->normal), *(plane->up));
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
	cylinder->q = get_obj_quat(*(cylinder->normal), *(cylinder->up));
	cylinder->topcap->is_inversed = NORMALDISC;
	cylinder->botcap->is_inversed = INVERSEDISC;
	cylinder->topcap->q = cylinder->q;
	cylinder->botcap->q = cylinder->q;
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
	// cylinder->pinn->type = NORMALDISC;
	// cylinder->botcap->type = INVERSEDISC;
	
	cone->q = get_obj_quat(*(cone->normal), *(cone->up));
	printf("%f\n", cone->q.q2);
	cone->pinnacle->is_inversed = NORMALDISC;
	cone->base->is_inversed = INVERSEDISC;
	cone->base->q = cone->q;
	cone->pinnacle->q = cone->q;
}


//quaternion 
void	qtilt(t_quat *q, double angle)
{
t_quat	rot;
	t_quat	inv;
	//t_quat	res1;
	t_quat	res2;

	if (q->q0 > 0.9999 || q->q0 < -0.9999)
	{
		q->q0 = 0.9999;
		q->q1 = 0.01;
		q->q2 = 0;
		q->q3 = 0;
	}
	normalize_quat(q);
	rot.q0 = cos(rad(angle) / 2);
	rot.q1 = 1 * sin(rad(angle) / 2);
	rot.q2 = 0;
	rot.q3 = 0;
	normalize_quat(&rot);
	inv = get_inverse_quat(rot);
	//res1 = mult_quat(rot, *q);
	res2 = mult_quat(*q, inv);
	q->q0 = res2.q0;
	q->q1 = res2.q1;
	q->q2 = res2.q2;
	q->q3 = res2.q3;
	if (q->q0 < 0.001 && q->q0 > 0.001)
	{
		q->q0 = 0;
		q->q1 = 1;
		q->q2 = 0;
		q->q3 = 0;
	}
}

// rotation along up/y-axis
void	qpan(t_quat *q, double angle)
{	
	t_quat	rot;
	t_quat	inv;
	//t_quat	res1;
	t_quat	res2;

	if (q->q0 > 0.9999 || q->q0 < -0.9999)
	{
		q->q0 = 0.9999;
		q->q1 = 0;
		q->q2 = 0.01;
		q->q3 = 0;
	}
	normalize_quat(q);
	rot.q0 = cos(rad(angle) / 2);
	rot.q1 = 0;
	rot.q2 = 1 * sin(rad(angle) / 2);
	rot.q3 = 0;
	normalize_quat(&rot);
	inv = get_inverse_quat(rot);
	//res1 = mult_quat(rot, *q);
	res2 = mult_quat(*q, inv);
	q->q0 = res2.q0;
	q->q1 = res2.q1;
	q->q2 = res2.q2;
	q->q3 = res2.q3;
	if (q->q0 < 0.001 && q->q0 > 0.001)
	{
		q->q0 = 0;
		q->q1 = 0;
		q->q2 = 1;
		q->q3 = 0;
	}
}

// rotation along forward/z-axis
void	qcant(t_quat *q, double angle)
{
	t_quat	rot;
	t_quat	inv;
	//t_quat	res1;
	t_quat	res2;

	if (q->q0 > 0.9999 || q->q0 < -0.9999)
	{
		q->q0 = 0.9999;
		q->q1 = 0;
		q->q2 = 0;
		q->q3 = 0.01;
	}
	normalize_quat(q);
	rot.q0 = cos(rad(angle) / 2);
	rot.q1 = 0;
	rot.q2 = 0;
	rot.q3 = 1 * sin(rad(angle) / 2);
	normalize_quat(&rot);
	inv = get_inverse_quat(rot);
	//res1 = mult_quat(rot, *q);
	res2 = mult_quat(*q, inv);
	q->q0 = res2.q0;
	q->q1 = res2.q1;
	q->q2 = res2.q2;
	q->q3 = res2.q3;
	if (q->q0 < 0.001 && q->q0 > 0.001)
	{
		q->q0 = 0;
		q->q1 = 0;
		q->q2 = 0;
		q->q3 = 1;
	}
}

void	qrotate_o(keys_t key, t_quat *q, t_camera *camera)
{
	(void)camera->coords;
	if (key == MLX_KEY_A)
		qpan(q, 5);
	else if (key == MLX_KEY_D)
		qpan(q, -5);
	else if (key == MLX_KEY_W)
		qtilt(q, 5);
	else if (key == MLX_KEY_S)
		qtilt(q, -5);
	else if (key == MLX_KEY_Q)
		qcant(q, 5);
	else if (key == MLX_KEY_E)
		qcant(q, -5);
	normalize_quat(q);
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

static void	change_glossiness(double *glossiness, keys_t key)
{
	if (key == MLX_KEY_G)
	{
		*glossiness += 0.05; 
		if (*glossiness >= 1.01)
			*glossiness = 0.05;
	}
}

static void	change_radius(double *radius, keys_t key)
{
	if (key == MLX_KEY_1)
		*radius += 0.5;
	else if (key == MLX_KEY_2)
		*radius -= 0.5;
	if (*radius < PRECISION)
		*radius = 0.1;
}

static void	change_height(double *height, keys_t key)
{
	if (key == MLX_KEY_3)
		*height += 1;
	else if (key == MLX_KEY_4)
		*height -= 1;
	if (*height < PRECISION)
		*height = 0.1;
}

void	manipulate_sphere(t_rt *rt, t_sphere *sphere, mlx_key_data_t keydata)
{
	change_glossiness(&sphere->glossiness, keydata.key);
	change_radius(&sphere->radius, keydata.key);
	move(keydata.key, rt->camera, sphere->coords);
	rotate_o(keydata.key, sphere->normal, sphere->right, sphere->up, rt->camera);
	qrotate_o(keydata.key, &(sphere->q), rt->camera);
	//sphere->q = get_obj_quat(*(sphere->normal), *(sphere->up));
	
}

void	manipulate_plane(t_rt *rt, t_plane *plane, mlx_key_data_t keydata)
{
	change_glossiness(&plane->glossiness, keydata.key);
	move(keydata.key, rt->camera, plane->coords);
	rotate_o(keydata.key, plane->normal, plane->right, plane->up, rt->camera);
	qrotate_o(keydata.key, &(plane->q), rt->camera);
	//plane->q = get_obj_quat(*(plane->normal), *(plane->up));
}

void	manipulate_cylinder(t_rt *rt, t_cylinder *cylinder, mlx_key_data_t keydata)
{
	change_glossiness(&cylinder->glossiness, keydata.key);
	change_radius(&cylinder->radius, keydata.key);
	change_height(&cylinder->height, keydata.key);
	move(keydata.key, rt->camera, cylinder->coords);
	rotate_o(keydata.key, cylinder->normal, cylinder->right, cylinder->up, rt->camera);
	qrotate_o(keydata.key, &(cylinder->q), rt->camera);
	get_discs(cylinder);
	//("texture %p, normal %p\n", cylinder->texture, cylinder->vector_map);
	//cylinder->q = get_obj_quat(*(cylinder->normal), *(cylinder->up));
	
	cylinder->topcap->q = cylinder->q;
	cylinder->botcap->q = cylinder->q;
}

void	manipulate_cone(t_rt *rt, t_cone *cone, mlx_key_data_t keydata)
{
	change_glossiness(&cone->glossiness, keydata.key);
	change_radius(&cone->radius, keydata.key);
	change_height(&cone->height, keydata.key);
	cone->half_angle = atan2(cone->radius, cone->height);
	cone->dist_term = sqrt(1 + pow(cone->half_angle, 2));
	move(keydata.key, rt->camera, cone->coords);
	rotate_o(keydata.key, cone->normal, cone->right, cone->up, rt->camera);
	qrotate_o(keydata.key, &(cone->q), rt->camera);
	*cone->axis = invert_vect3f(*cone->normal);
	get_cone_discs(cone);
	//cone->q = get_obj_quat(*(cone->normal), *(cone->up));
	cone->base->q = cone->q;
	cone->pinnacle->q = cone->q;
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

void	manipulate_objects(t_master *master, mlx_key_data_t keydata)
{
	manipulate_highlighted_object(master->rt, keydata);
	find_rays(master);
}

void	specular_options(t_master *master, keys_t key)
{
	//double	glossiness;
	double	highlight_size;

	//glossiness = master->options->glossiness;
	highlight_size = master->options->spec_highlight_size;
	// if (key == MLX_KEY_G)
	// {
	// 	glossiness += 0.05; 
	// 	if (glossiness >= 1.01)
	// 		glossiness = 0.05;
	// }
	if (key == MLX_KEY_H)
	{
		highlight_size /= 2;
		if (highlight_size <= 2)
			highlight_size = 1024;
	}
	//master->options->glossiness = glossiness;
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
	// if (keydata.key == MLX_KEY_COMMA)
	// 	master->options->light_intensity -= 15;
	// else if (keydata.key == MLX_KEY_PERIOD)
	// 	master->options->light_intensity += 15;
	//clamp(0, INT_MAX - 100, &master->options->light_intensity);
	specular_options(master, keydata.key);
	//printf("Light intensity: %d\n", master->options->light_intensity);
	//printf("Glossiness: %.2f\n", master->options->glossiness);
	find_rays(master);
}