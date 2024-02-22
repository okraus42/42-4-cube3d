/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phong_illumination.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 11:37:57 by plouda            #+#    #+#             */
/*   Updated: 2024/02/22 15:08:12 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	trace_shadow(t_master *master, t_rayfinder *rf, t_vect3f intersection, t_shader *shader, double *light_pos)
{
	rf->shadowray.direction = shader->light_dir;
	rf->shadowray.origin = add_vect3f(intersection, scale_vect3f(1e-4, shader->hit_normal));
	shader->light_dist = point_distance(rf->shadowray.origin, array_to_vect(light_pos));
	rf->t_near = (double)INT_MAX;
	if (shader->light_dist < 1e-6)
		shader->light_dist = 1e-6;
	if (find_intersections(master, rf->shadowray, rf, SHADOW))
	{
		rf->shadow_inter = get_intersection(rf->shadowray.origin, \
		rf->shadowray.direction, rf->t_near);
		rf->inter_dist = point_distance(rf->shadow_inter, rf->shadowray.origin);
		if (rf->inter_dist < shader->light_dist)
		{
			shader->diffuse_ratio = 0;
			shader->specular_ratio = 0;
		}
	}
}

void	diff_and_spec_ratios(t_shader *shader, t_options options)
{
	shader->diffuse_ratio = dot_product(shader->hit_normal, shader->light_dir);
	if (!shader->diffuse_ratio)
		shader->specular_ratio = 0;
	else
	{
		get_reflection_vector(shader);
		shader->specular_ratio = shader->obj_glossiness * pow(MAX(dot_product(shader->view_dir, shader->reflect_vect), 0), options.spec_highlight_size);
	}
}

/* void	trace_shadow(t_master *master, t_rayfinder *rf, t_vect3f intersection, t_shader *shader, double *light_pos)
{
	rf->shadowray.direction = shader->light_dir;
	rf->shadowray.origin = add_vect3f(intersection, scale_vect3f(1e-4, shader->hit_normal));
	//rf->shadowray.origin = intersection;
	shader->light_dist = point_distance(rf->shadowray.origin, array_to_vect(light_pos));
	rf->t_near = (double)INT_MAX;
	if (shader->light_dist < 1e-6)
		shader->light_dist = 1e-6;
	if (find_intersections(master, rf->shadowray, rf, SHADOW))
	{
		rf->shadow_inter = get_intersection(rf->shadowray.origin, \
		rf->shadowray.direction, rf->t_near);
		rf->inter_dist = point_distance(rf->shadow_inter, rf->shadowray.origin);
		if (rf->inter_dist < shader->light_dist)
		{
			shader->diffuse_ratio = 0;
			shader->specular_ratio = 0;
		}
	}
} */

void	phong_illumination(t_shader *shader, t_sphere *light)
{
	shader->id = shader->rgb_object[R] / 255. * light->brightness * light->rgb[R] / 255. ;
	shader->is = light->rgb[R] / 255. * light->brightness;
	shader->illumination[R] += (shader->diffuse_ratio * shader->id + shader->specular_ratio * shader->is);

	shader->id = shader->rgb_object[G] / 255. * light->brightness * light->rgb[G] / 255. ;
	shader->is = light->rgb[G] / 255. * light->brightness;
	shader->illumination[G] += (shader->diffuse_ratio * shader->id + shader->specular_ratio * shader->is);

	shader->id =shader->rgb_object[B] / 255. * light->brightness * light->rgb[B] / 255. ;
	shader->is = light->rgb[B] / 255. * light->brightness;
	shader->illumination[B] += (shader->diffuse_ratio * shader->id + shader->specular_ratio * shader->is);

	clampf(0, 1, &shader->illumination[R]);
	clampf(0, 1, &shader->illumination[G]);
	clampf(0, 1, &shader->illumination[B]);
}

void	get_reflection_vector(t_shader *shader)
{
	shader->incident_light = shader->light_dir;
	shader->dot_reflect = MAX(dot_product(shader->hit_normal, shader->incident_light), 0);
	shader->reflect_vect.x = 2 * shader->dot_reflect * shader->hit_normal.x - shader->incident_light.x;
	shader->reflect_vect.y = 2 * shader->dot_reflect * shader->hit_normal.y - shader->incident_light.y;
	shader->reflect_vect.z = 2 * shader->dot_reflect * shader->hit_normal.z - shader->incident_light.z;
	normalize(&shader->reflect_vect);
}
/* 
void	diff_and_spec_ratios(t_shader *shader, t_options options)
{
	shader->diffuse_ratio = dot_product(shader->hit_normal, shader->light_dir);
	// clampf(0, 1, &shader->diffuse_ratio);
	if (shader->diffuse_ratio < 0)
	{
		shader->diffuse_ratio = shader->diffuse_ratio * -1;
		shader->hit_normal = invert_vect3f(shader->hit_normal);
	}
	if (!shader->diffuse_ratio)
		shader->specular_ratio = 0;
	else
	{
		get_reflection_vector(shader);
		shader->specular_ratio = shader->obj_glossiness * pow(MAX(dot_product(shader->view_dir, shader->reflect_vect), 0), options.spec_highlight_size);
	}
} */