/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shader_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 10:55:46 by plouda            #+#    #+#             */
/*   Updated: 2024/03/04 20:06:20 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

// https://stackoverflow.com/questions/47932955/
int	point_lies_in_cylinder(t_vect3f point, t_vect3f bot_point,
	t_vect3f top_point, double radius)
{
	t_vect3f	axis;
	t_vect3f	helper_vector;
	t_vect3f	cross_vector;
	double		dist;

	axis = subtract_vect3f(top_point, bot_point);
	helper_vector = subtract_vect3f(point, bot_point);
	cross_vector = cross_product(helper_vector, axis);
	dist = get_vector_length(cross_vector) / get_vector_length(axis);
	return (dist < radius && absf(dist - radius) > PRECISION);
}

int	point_lies_in_cone(t_vect3f point, t_cone cone,
	double radius, double height)
{
	t_vect3f	centered;
	t_vect3f	axis;
	double		cone_dist;
	double		cone_radius;
	double		orth_distance;

	axis = invert_vect3f(*cone.normal);
	centered = subtract_vect3f(point, array_to_vect(cone.pinnacle->coords));
	cone_dist = dot_product(centered, axis);
	cone_radius = cone_dist / height * radius;
	orth_distance = get_vector_length(subtract_vect3f(centered,
				scale_vect3f(cone_dist, axis)));
	return (orth_distance < cone_radius
		&& absf(orth_distance - cone_radius) > PRECISION);
}

void	shader_loop(t_shader *shader, t_master *master,
		t_vect3f intersection, t_rayfinder *rf)
{
	int	i;

	i = 0;
	while (i < master->rt->n_lights)
	{
		shader->light_dir = subtract_vect3f(array_to_vect(
					master->rt->light_spheres[i]->coords), intersection);
		normalize(&shader->light_dir);
		diff_and_spec_ratios(shader, *master->options);
		trace_shadow(master, rf, shader, master->rt->light_spheres[i]->coords);
		phong_illumination(shader, master->rt->light_spheres[i]);
		i++;
	}
}
