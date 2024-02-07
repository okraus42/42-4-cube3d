/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_shaders.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 17:40:11 by plouda            #+#    #+#             */
/*   Updated: 2024/02/07 11:33:56 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	get_clr_components_t(int *light, int *rgb, double ratio, double bright, double falloff)
{
	light[R] = ((double)rgb[R] * bright / falloff) * ratio;
	light[G] = ((double)rgb[G] * bright / falloff) * ratio;
	light[B] = ((double)rgb[B] * bright / falloff) * ratio;
}

void	get_comb_clr(int *rgb_comb, int *rgb_light, int *rgb_spec)
{

	rgb_comb[R] = ((int)((rgb_light[R] + 1) * (rgb_spec[R] + 1)) - 1) >> 8;
	rgb_comb[G] = ((int)((rgb_light[G] + 1) * (rgb_spec[G] + 1)) - 1) >> 8;
	rgb_comb[B] = ((int)((rgb_light[B] + 1) * (rgb_spec[B] + 1)) - 1) >> 8;
}

void	trace_shadow_t(t_master *m, t_rayfinder *rf, t_shader shader)
{
	if (find_intersections(m, rf->shadowray, rf, SHADOW))
	{
		rf->shadow_inter = get_intersection(rf->shadowray.origin, \
		rf->shadowray.direction, rf->t_near);
		rf->inter_dist = point_distance(rf->shadow_inter, rf->shadowray.origin);
		if (rf->inter_dist < shader.light_dist)
			rf->clr = shader.rgb_ambient;
		else
		{
			rf->clr = ft_max_clr(shader.rgb_ambient, shader.rgb_diffuse);
			rf->clr = ft_max_clr(rf->clr, shader.rgb_specular);
		}
	}
	else
	{
		rf->clr = ft_max_clr(shader.rgb_ambient, shader.rgb_diffuse);
		rf->clr = ft_max_clr(rf->clr, shader.rgb_specular);
	}
}


void	trace_shadow(t_master *master, t_rayfinder *rf, t_vect3f intersection, t_shader *shader)
{
	rf->shadowray.direction = shader->light_dir;
	rf->shadowray.origin = add_vect3f(intersection, scale_vect3f(1e-4, shader->hit_normal));
	shader->light_dist = point_distance(rf->shadowray.origin, array_to_vect(master->rt->light->coords));
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

void	phong_illumination(t_shader *shader, t_rt *rt, int *object_unit_rgb)
{
	shader->ia = rt->ambient->rgb[R] / 255. * rt->ambient->ratio * object_unit_rgb[R] / 255. ;
	shader->id = object_unit_rgb[R] / 255. * rt->light->brightness * rt->light->rgb[R] / 255. ;
	shader->is = rt->light->rgb[R] / 255. * rt->light->brightness;
	shader->illumination[R] = shader->ia + shader->diffuse_ratio * shader->id + shader->specular_ratio * shader->is;

	shader->ia = rt->ambient->rgb[G] / 255. * rt->ambient->ratio * object_unit_rgb[G] / 255. ;
	shader->id = object_unit_rgb[G] / 255. * rt->light->brightness * rt->light->rgb[G] / 255. ;
	shader->is = rt->light->rgb[G] / 255. * rt->light->brightness;
	shader->illumination[G] = shader->ia + shader->diffuse_ratio * shader->id + shader->specular_ratio * shader->is;

	shader->ia = rt->ambient->rgb[B] / 255. * rt->ambient->ratio * object_unit_rgb[B] / 255. ;
	shader->id =object_unit_rgb[B] / 255. * rt->light->brightness * rt->light->rgb[B] / 255. ;
	shader->is = rt->light->rgb[B] / 255. * rt->light->brightness;
	shader->illumination[B] = shader->ia + shader->diffuse_ratio * shader->id + shader->specular_ratio * shader->is;

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

void	diff_and_spec_ratios(t_shader *shader, t_options options)
{
	shader->diffuse_ratio = dot_product(shader->hit_normal, shader->light_dir);
	//clampf(0, 1, &shader.diffuse_ratio);
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
		shader->specular_ratio = options.glossiness * pow(MAX(dot_product(shader->view_dir, shader->reflect_vect), 0), options.spec_highlight_size);
	}
}

void	sphere_shader(t_rayfinder *rf, t_vect3f intersection, void *object_ptr, t_master *master)
{
	t_sphere	*sphere;
	t_shader	shader;

	sphere = (t_sphere *)object_ptr;
	shader.light_intensity = rf->light_intensity;
	shader.hit_normal = subtract_vect3f(intersection, array_to_vect(sphere->coords));
	shader.light_dir = subtract_vect3f(array_to_vect(master->rt->light->coords), intersection);
	shader.view_dir = subtract_vect3f(array_to_vect(master->rt->camera->coords), intersection);
	normalize(&shader.view_dir);
	normalize(&shader.hit_normal);
	normalize(&shader.light_dir);

	diff_and_spec_ratios(&shader, *master->options);
	trace_shadow(master, rf, intersection, &shader);
	phong_illumination(&shader, master->rt, sphere->rgb);
	shader.pix_color[R] = 255 * shader.illumination[R];
	shader.pix_color[G] = 255 * shader.illumination[G];
	shader.pix_color[B] = 255 * shader.illumination[B];
	rf->clr = get_clr_int(shader.pix_color);
}

void	plane_shader(t_rayfinder *rf, t_vect3f intersection, void *object_ptr, t_master *master)
{
	t_plane		*plane;
	t_shader	shader;
	
	plane = (t_plane *)object_ptr;
	shader.light_intensity = rf->light_intensity;
	shader.hit_normal = *plane->normal;
	shader.light_dir = subtract_vect3f(array_to_vect(master->rt->light->coords), intersection);
	shader.view_dir = subtract_vect3f(array_to_vect(master->rt->camera->coords), intersection);
	normalize(&shader.view_dir);
	normalize(&shader.hit_normal);
	normalize(&shader.light_dir);

	diff_and_spec_ratios(&shader, *master->options);
	trace_shadow(master, rf, intersection, &shader);
	phong_illumination(&shader, master->rt, plane->rgb);
	shader.pix_color[R] = 255 * shader.illumination[R];
	shader.pix_color[G] = 255 * shader.illumination[G];
	shader.pix_color[B] = 255 * shader.illumination[B];
	rf->clr = get_clr_int(shader.pix_color);
}

void	cylinder_shader(t_rayfinder *rf, t_vect3f intersection, void *object_ptr, t_master *master, t_ray ray)
{
	t_cylinder	*cylinder;
	t_shader	shader;
	
	cylinder = (t_cylinder *)object_ptr;
	shader.light_intensity = rf->light_intensity;
	shader.hit_normal = get_hit_normal(rf, ray, intersection, *cylinder);
	shader.light_dir = subtract_vect3f(array_to_vect(master->rt->light->coords), intersection);
	shader.view_dir = subtract_vect3f(array_to_vect(master->rt->camera->coords), intersection);
	normalize(&shader.view_dir);
	normalize(&shader.hit_normal);
	normalize(&shader.light_dir);

	diff_and_spec_ratios(&shader, *master->options);
	trace_shadow(master, rf, intersection, &shader);
	phong_illumination(&shader, master->rt, cylinder->rgb);
	shader.pix_color[R] = 255 * shader.illumination[R];
	shader.pix_color[G] = 255 * shader.illumination[G];
	shader.pix_color[B] = 255 * shader.illumination[B];
	rf->clr = get_clr_int(shader.pix_color);
}

void	disc_shader(t_rayfinder *rf, t_vect3f intersection, void *object_ptr, t_master *master)
{
	t_disc		*disc;
	t_shader	shader;
	
	disc = (t_disc *)object_ptr;
	shader.light_intensity = rf->light_intensity;
	shader.hit_normal = *disc->normal;
	shader.light_dir = subtract_vect3f(array_to_vect(master->rt->light->coords), intersection);
	shader.view_dir = subtract_vect3f(array_to_vect(master->rt->camera->coords), intersection);
	normalize(&shader.view_dir);
	normalize(&shader.hit_normal);
	normalize(&shader.light_dir);

	diff_and_spec_ratios(&shader, *master->options);
	trace_shadow(master, rf, intersection, &shader);
	phong_illumination(&shader, master->rt, disc->rgb);
	shader.pix_color[R] = 255 * shader.illumination[R];
	shader.pix_color[G] = 255 * shader.illumination[G];
	shader.pix_color[B] = 255 * shader.illumination[B];
	rf->clr = get_clr_int(shader.pix_color);
}

t_vect3f	get_cone_hit_normal(t_vect3f intersection, t_cone cone)
{
	double	dist;
	double	half_angle;
	t_vect3f	axis;
	t_vect3f	a;
	double		d;
	t_vect3f	normal;

	axis = invert_vect3f(*cone.normal);
	half_angle = atan2(cone.diameter / 2, cone.height);
	dist = point_distance(intersection, array_to_vect(cone.pinnacle->coords));
	d = dist * sqrt(1 + pow(half_angle, 2));
	a.x = cone.pinnacle->coords[X] + axis.x * d;
	a.y = cone.pinnacle->coords[Y] + axis.y * d;
	a.z = cone.pinnacle->coords[Z] + axis.z * d;
	normal = subtract_vect3f(intersection, a);
	normalize(&normal);
	return (normal);
}

void	cone_shader(t_rayfinder *rf, t_vect3f intersection, void *object_ptr, t_master *master)
{
	t_cone	*cone;
	t_shader	shader;
	
	cone = (t_cone *)object_ptr;
	shader.light_intensity = rf->light_intensity;
	shader.hit_normal = get_cone_hit_normal(intersection, *cone);
	shader.light_dir = subtract_vect3f(array_to_vect(master->rt->light->coords), intersection);
	shader.view_dir = subtract_vect3f(array_to_vect(master->rt->camera->coords), intersection);
	normalize(&shader.view_dir);
	normalize(&shader.hit_normal);
	normalize(&shader.light_dir);
	
	diff_and_spec_ratios(&shader, *master->options);
	trace_shadow(master, rf, intersection, &shader);
	phong_illumination(&shader, master->rt, cone->rgb);
	shader.pix_color[R] = 255 * shader.illumination[R];
	shader.pix_color[G] = 255 * shader.illumination[G];
	shader.pix_color[B] = 255 * shader.illumination[B];
	rf->clr = get_clr_int(shader.pix_color);
}

void	light_shader(t_rayfinder *rf, void *object_ptr, t_master *master)
{
	t_sphere	*sphere;
	int			rgb_light_arr[3];
	
	sphere = (t_sphere *)object_ptr;
	get_clr_components(rgb_light_arr, sphere->rgb, 1, master->rt->light->brightness);
	rf->clr = get_clr_int(rgb_light_arr);
}
