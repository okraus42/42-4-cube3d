/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manipulate_objects.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 13:21:25 by plouda            #+#    #+#             */
/*   Updated: 2024/03/04 17:08:54 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	set_sphere_vects(t_sphere *sphere)
{
	t_vect3f	tmp;
	t_vect33f	o;

	o.normal = sphere->normal;
	o.right = sphere->right;
	o.up = sphere->up;
	tmp = (t_vect3f){0, 1, 0};
	if (o.normal->x == 0. && o.normal->y == 1. && o.normal->z == 0.)
	{
		*o.right = (t_vect3f){1, 0, 0};
		*o.up = (t_vect3f){0, 0, -1};
	}
	else if (o.normal->x == 0. && o.normal->y == -1 && o.normal->z == 0.)
	{
		*o.right = (t_vect3f){1, 0, 0};
		*o.up = (t_vect3f){0, 0, 1};
	}
	else
	{
		*o.right = cross_product(tmp, *o.normal);
		normalize(o.right);
		*o.up = cross_product(*o.normal, *o.right);
		normalize(o.up);
	}
	sphere->q = get_obj_quat(*(sphere->normal), *(sphere->up));
}

void	set_plane_vects(t_plane *plane)
{
	t_vect3f	tmp;
	t_vect33f	o;

	o.normal = plane->normal;
	o.right = plane->right;
	o.up = plane->up;
	tmp = (t_vect3f){0, 1, 0};
	if (o.normal->x == 0. && o.normal->y == 1. && o.normal->z == 0.)
	{
		*o.right = (t_vect3f){1, 0, 0};
		*o.up = (t_vect3f){0, 0, -1};
	}
	else if (o.normal->x == 0. && o.normal->y == -1 && o.normal->z == 0.)
	{
		*o.right = (t_vect3f){1, 0, 0};
		*o.up = (t_vect3f){0, 0, 1};
	}
	else
	{
		*o.right = cross_product(tmp, *o.normal);
		normalize(o.right);
		*o.up = cross_product(*o.normal, *o.right);
		normalize(o.up);
	}
	plane->q = get_obj_quat(*(plane->normal), *(plane->up));
}

static void	set_cylinder_vects_ptrs(t_cylinder *cylinder, t_vect33f *o)
{
	o->normal = cylinder->normal;
	o->right = cylinder->right;
	o->up = cylinder->up;
}

static void	set_cylinder_cap_inversion(t_cylinder *cylinder)
{
	cylinder->topcap->is_inversed = NORMALDISC;
	cylinder->botcap->is_inversed = INVERSEDISC;
	cylinder->topcap->q = cylinder->q;
	cylinder->botcap->q = cylinder->q;
}

void	set_cylinder_vects(t_cylinder *cylinder)
{
	t_vect3f	tmp;
	t_vect33f	o;

	set_cylinder_vects_ptrs(cylinder, &o);
	tmp = (t_vect3f){0, 1, 0};
	if (o.normal->x == 0. && o.normal->y == 1. && o.normal->z == 0.)
	{
		*o.right = (t_vect3f){1, 0, 0};
		*o.up = (t_vect3f){0, 0, -1};
	}
	else if (o.normal->x == 0. && o.normal->y == -1 && o.normal->z == 0.)
	{
		*o.right = (t_vect3f){1, 0, 0};
		*o.up = (t_vect3f){0, 0, 1};
	}
	else
	{
		*o.right = cross_product(tmp, *o.normal);
		normalize(o.right);
		*o.up = cross_product(*o.normal, *o.right);
		normalize(o.up);
	}
	cylinder->q = get_obj_quat(*(cylinder->normal), *(cylinder->up));
	set_cylinder_cap_inversion(cylinder);
}

void	set_disc_vects(t_disc *disc)
{
	t_vect3f	tmp;
	t_vect33f	o;

	o.normal = disc->normal;
	o.right = disc->right;
	o.up = disc->up;
	tmp = (t_vect3f){0, 1, 0};
	if (o.normal->x == 0. && o.normal->y == 1. && o.normal->z == 0.)
	{
		*o.right = (t_vect3f){1, 0, 0};
		*o.up = (t_vect3f){0, 0, -1};
	}
	else if (o.normal->x == 0. && o.normal->y == -1 && o.normal->z == 0.)
	{
		*o.right = (t_vect3f){1, 0, 0};
		*o.up = (t_vect3f){0, 0, 1};
	}
	else
	{
		*o.right = cross_product(tmp, *o.normal);
		normalize(o.right);
		*o.up = cross_product(*o.normal, *o.right);
		normalize(o.up);
	}
}

static void	set_cone_vects_ptrs(t_cone *cone, t_vect33f *o)
{
	o->normal = cone->normal;
	o->right = cone->right;
	o->up = cone->up;
}

static void	set_cone_cap_inversion(t_cone *cone)
{
	cone->pinnacle->is_inversed = NORMALDISC;
	cone->base->is_inversed = INVERSEDISC;
	cone->base->q = cone->q;
	cone->pinnacle->q = cone->q;
}

void	set_cone_vects(t_cone *cone)
{
	t_vect3f	tmp;
	t_vect33f	o;

	set_cone_vects_ptrs(cone, &o);
	tmp = (t_vect3f){0, 1, 0};
	if (o.normal->x == 0. && o.normal->y == 1. && o.normal->z == 0.)
	{
		*o.right = (t_vect3f){1, 0, 0};
		*o.up = (t_vect3f){0, 0, -1};
	}
	else if (o.normal->x == 0. && o.normal->y == -1 && o.normal->z == 0.)
	{
		*o.right = (t_vect3f){1, 0, 0};
		*o.up = (t_vect3f){0, 0, 1};
	}
	else
	{
		*o.right = cross_product(tmp, *o.normal);
		normalize(o.right);
		*o.up = cross_product(*o.normal, *o.right);
		normalize(o.up);
	}
	cone->q = get_obj_quat(*(cone->normal), *(cone->up));
	set_cone_cap_inversion(cone);
}

//quaternion 
void	qtilt(t_quat *q, double angle)
{
	t_quat	rot;
	t_quat	inv;
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
	t_vect33f	axes;

	axes.normal = sphere->normal;
	axes.right = sphere->right;
	axes.up = sphere->up;
	change_glossiness(&sphere->glossiness, keydata.key);
	change_radius(&sphere->radius, keydata.key);
	move(keydata.key, rt->camera, sphere->coords);
	rotate_o(keydata.key, &axes, rt->camera);
	qrotate_o(keydata.key, &(sphere->q), rt->camera);
}

void	manipulate_plane(t_rt *rt, t_plane *plane, mlx_key_data_t keydata)
{
	t_vect33f	axes;

	axes.normal = plane->normal;
	axes.right = plane->right;
	axes.up = plane->up;
	change_glossiness(&plane->glossiness, keydata.key);
	move(keydata.key, rt->camera, plane->coords);
	rotate_o(keydata.key, &axes, rt->camera);
	qrotate_o(keydata.key, &(plane->q), rt->camera);
}

void	manipulate_cylinder(t_rt *rt,
	t_cylinder *cylinder, mlx_key_data_t keydata)
{
	t_vect33f	axes;

	axes.normal = cylinder->normal;
	axes.right = cylinder->right;
	axes.up = cylinder->up;
	change_glossiness(&cylinder->glossiness, keydata.key);
	change_radius(&cylinder->radius, keydata.key);
	change_height(&cylinder->height, keydata.key);
	move(keydata.key, rt->camera, cylinder->coords);
	rotate_o(keydata.key, &axes, rt->camera);
	qrotate_o(keydata.key, &(cylinder->q), rt->camera);
	get_discs(cylinder);
	cylinder->topcap->q = cylinder->q;
	cylinder->botcap->q = cylinder->q;
}

void	manipulate_cone(t_rt *rt, t_cone *cone, mlx_key_data_t keydata)
{
	t_vect33f	axes;

	axes.normal = cone->normal;
	axes.right = cone->right;
	axes.up = cone->up;
	change_glossiness(&cone->glossiness, keydata.key);
	change_radius(&cone->radius, keydata.key);
	change_height(&cone->height, keydata.key);
	cone->half_angle = atan2(cone->radius, cone->height);
	cone->dist_term = sqrt(1 + pow(cone->half_angle, 2));
	move(keydata.key, rt->camera, cone->coords);
	rotate_o(keydata.key, &axes, rt->camera);
	qrotate_o(keydata.key, &(cone->q), rt->camera);
	*cone->axis = invert_vect3f(*cone->normal);
	get_cone_discs(cone);
	cone->base->q = cone->q;
	cone->pinnacle->q = cone->q;
}

void	manipulate_highlighted_sphere_plane(t_rt *rt, mlx_key_data_t keydata)
{
	int	i;

	i = 0;
	while (i < rt->n_spheres)
	{
		if (rt->spheres[i]->mode == HIGHLIGHT)
			manipulate_sphere(rt, rt->spheres[i], keydata);
		i++;
	}
	i = 0;
	while (i < rt->n_planes)
	{
		if (rt->planes[i]->mode == HIGHLIGHT)
			manipulate_plane(rt, rt->planes[i], keydata);
		i++;
	}
}

void	manipulate_highlighted_object(t_rt *rt, mlx_key_data_t keydata)
{
	int	i;

	manipulate_highlighted_sphere_plane(rt, keydata);
	i = 0;
	while (i < rt->n_cylinders)
	{
		if (rt->cylinders[i]->mode == HIGHLIGHT)
			manipulate_cylinder(rt, rt->cylinders[i], keydata);
		i++;
	}
	i = 0;
	while (i < rt->n_cones)
	{
		if (rt->cones[i]->mode == HIGHLIGHT)
			manipulate_cone(rt, rt->cones[i], keydata);
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
	double	highlight_size;

	highlight_size = master->options->spec_highlight_size;
	if (key == MLX_KEY_H)
	{
		highlight_size /= 2;
		if (highlight_size <= 2)
			highlight_size = 1024;
	}
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
	specular_options(master, keydata.key);
	find_rays(master);
}
