/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manipulated_objects.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 12:24:53 by plouda            #+#    #+#             */
/*   Updated: 2024/03/05 12:29:48 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

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
