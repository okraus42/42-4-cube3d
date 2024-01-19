/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_shaders.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 17:40:11 by plouda            #+#    #+#             */
/*   Updated: 2024/01/19 17:59:05 by plouda           ###   ########.fr       */
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

void	trace_shadow_t(t_master *m, t_rayfinder *rf, uint32_t amb, uint32_t light, uint32_t spec)
{
	if (find_intersections(m, rf->shadowray, rf, SHADOW))
	{
		rf->shadow_inter = get_intersection(rf->shadowray.origin, \
		rf->shadowray.direction, rf->t_near);
		rf->inter_dist = point_distance(rf->shadow_inter, rf->shadowray.origin);
		if (rf->inter_dist < rf->light_dist)
			rf->clr = amb;
		else
		{
			rf->clr = ft_max_clr(amb, light);
			rf->clr = ft_max_clr(rf->clr, spec);
		}
	}
	else
	{
		rf->clr = ft_max_clr(amb, light);
		rf->clr = ft_max_clr(rf->clr, spec);
	}
}

/* t_shader	init_shader_struct(void)
{
	t_shader	shader;

}
 */
void	sphere_shader(t_rayfinder *rf, t_vect3f intersection, void *object_ptr, t_master *master)
{
	t_sphere	*sphere;
	t_shader	shader;
	int			rgb_diffuse_arr[3];
	int			rgb_specular_arr[3];
	uint32_t	rgb_diffuse;
	uint32_t	rgb_ambient;
	uint32_t	rgb_specular;
	t_vect3f	incident_light;
	t_vect3f	reflect_vect;
	t_vect3f	view_dir;
	double		dot_reflect;
	double		falloff;

	sphere = (t_sphere *)object_ptr;
	/* if (sphere->mode == HIGHLIGHT)
	{
		rf->clr = 0x00FF00FF;
		return ;
	} */
	shader.hit_normal = subtract_vect3f(intersection, array_to_vect(sphere->coords));
	shader.light_dir = subtract_vect3f(array_to_vect(master->rt->light->coords), intersection);
	shader.view_dir = subtract_vect3f(array_to_vect(master->rt->camera->coords), intersection);
	normalize(&shader.view_dir);
	normalize(&shader.hit_normal);
	normalize(&shader.light_dir);
	shader.incident_light = shader.light_dir;
	shader.dot_reflect = MAX(dot_product(shader.hit_normal, shader.incident_light), 0);
	shader.reflect_vect.x = 2 * shader.dot_reflect * shader.hit_normal.x - shader.incident_light.x;
	shader.reflect_vect.y = 2 * shader.dot_reflect * shader.hit_normal.y - shader.incident_light.y;
	shader.reflect_vect.z = 2 * shader.dot_reflect * shader.hit_normal.z - shader.incident_light.z;
	normalize(&shader.reflect_vect);
	
	rf->shadowray.direction = rf->light_dir;
	rf->shadowray.origin = add_vect3f(intersection, scale_vect3f(1e-4, rf->hit_normal));
	rf->light_dist = point_distance(rf->shadowray.origin, array_to_vect(master->rt->light->coords));
	rf->t_near = (double)INT_MAX;
	if (rf->light_dist < 1e-6)
		rf->light_dist = 0.00001;
	
	if (rf->light_intensity != 0)
	{
		falloff = MAX(pow(rf->light_dist / rf->light_intensity, 2), 1);
			rf->light_ratio = dot_product(rf->hit_normal, rf->light_dir);
		clampf(0, 1, &rf->light_ratio);
		get_clr_components_t(rgb_diffuse_arr, sphere->rgb_light, \
			rf->light_ratio, master->rt->light->brightness, falloff);
		rf->light_ratio = master->options->glossiness * pow(MAX(dot_product(view_dir, reflect_vect), 0), master->options->spec_highlight_size);
		clampf(0, 1, &rf->light_ratio);
		get_clr_components_t(rgb_specular_arr, master->rt->light->rgb, \
			rf->light_ratio, master->rt->light->brightness, falloff);
		rgb_specular = get_clr_int(rgb_specular_arr);
		rgb_diffuse = get_clr_int(rgb_diffuse_arr);
		rgb_ambient = get_clr_int(sphere->rgb_ambient);
	}
	else
	{
		rgb_ambient = get_clr_int(sphere->rgb_ambient);
		rgb_specular = rgb_ambient;
		rgb_diffuse = rgb_ambient;
	}
	trace_shadow_t(master, rf, rgb_ambient, rgb_diffuse, rgb_specular);
}

void	plane_shader(t_rayfinder *rf, t_vect3f intersection, void *object_ptr, t_master *master)
{
	t_plane		*plane;
	int			rgb_light_arr[3];
	uint32_t	rgb_light;
	uint32_t	rgb_ambient;
	
	plane = (t_plane *)object_ptr;
	/* if (plane->mode == HIGHLIGHT)
	{
		rf->clr = 0x00FF00FF;
		return ;
	} */
	rf->hit_normal = *plane->normal;
	rf->light_dir = subtract_vect3f(array_to_vect(master->rt->light->coords), intersection);
	normalize(&rf->light_dir);
	rf->light_ratio = dot_product(rf->hit_normal, rf->light_dir);
	//clampf(0, 1, &rf->light_ratio);
	if (rf->light_ratio < 0)
	{
		rf->light_ratio = rf->light_ratio * -1;
		rf->hit_normal = invert_vect3f(rf->hit_normal); // because shadowray's origin will be shadowed by the plane itself otherwise
	}

	rf->shadowray.direction = rf->light_dir;
	rf->shadowray.origin = add_vect3f(intersection, scale_vect3f(1e-4, rf->hit_normal));
	rf->light_dist = point_distance(rf->shadowray.origin, array_to_vect(master->rt->light->coords));
	if (rf->light_dist < 1e-6)
		rf->light_dist = 0.00001;
	rf->t_near = (double)INT_MAX;

	get_clr_components_t(rgb_light_arr, plane->rgb_light, \
	rf->light_ratio, master->rt->light->brightness, MAX(pow(rf->light_dist / rf->light_intensity, 2), 1));
	rgb_light = get_clr_int(rgb_light_arr);
	rgb_ambient = get_clr_int(plane->rgb_ambient);
	trace_shadow(master, rf, rgb_ambient, rgb_light);
}

void	cylinder_shader(t_rayfinder *rf, t_vect3f intersection, void *object_ptr, t_master *master, t_ray ray)
{
	t_cylinder	*cylinder;
	int			rgb_light_arr[3];
	uint32_t	rgb_light;
	uint32_t	rgb_ambient;
	
	cylinder = (t_cylinder *)object_ptr;
	/* if (cylinder->mode == HIGHLIGHT)
	{
		rf->clr = 0x00FF00FF;
		return ;
	} */
	rf->hit_normal = get_hit_normal(rf, ray, intersection, *cylinder);
	rf->light_dir = subtract_vect3f(array_to_vect(master->rt->light->coords), intersection);
	normalize(&rf->light_dir);
	rf->light_ratio = dot_product(rf->hit_normal, rf->light_dir);
	clampf(0, 1, &rf->light_ratio);
	
	rf->shadowray.direction = rf->light_dir;
	rf->shadowray.origin = add_vect3f(intersection, scale_vect3f(1e-4, rf->hit_normal));
	rf->light_dist = point_distance(rf->shadowray.origin, array_to_vect(master->rt->light->coords));
	if (rf->light_dist < 1e-6)
		rf->light_dist = 0.00001;
	rf->t_near = (double)INT_MAX;

	get_clr_components_t(rgb_light_arr, cylinder->rgb_light, \
	rf->light_ratio, master->rt->light->brightness, MAX(pow(rf->light_dist / rf->light_intensity, 2), 1));
	rgb_light = get_clr_int(rgb_light_arr);
	rgb_ambient = get_clr_int(cylinder->rgb_ambient);
	trace_shadow(master, rf, rgb_ambient, rgb_light);
}

void	disc_shader(t_rayfinder *rf, t_vect3f intersection, void *object_ptr, t_master *master)
{
	t_disc		*disc;
	int			rgb_light_arr[3];
	uint32_t	rgb_light;
	uint32_t	rgb_ambient;
	
	disc = (t_disc *)object_ptr;
	/* if (disc->mode == HIGHLIGHT)
	{
		rf->clr = 0x00FF00FF;
		return ;
	} */
	rf->hit_normal = *disc->normal;
	rf->light_dir = subtract_vect3f(array_to_vect(master->rt->light->coords), intersection);
	normalize(&rf->light_dir);
	rf->light_ratio = dot_product(rf->hit_normal, rf->light_dir);
	clampf(0, 1, &rf->light_ratio);


	rf->shadowray.direction = rf->light_dir;
	rf->shadowray.origin = add_vect3f(intersection, scale_vect3f(1e-4, rf->hit_normal));
	rf->light_dist = point_distance(rf->shadowray.origin, array_to_vect(master->rt->light->coords));
	if (rf->light_dist < 1e-6)
		rf->light_dist = 0.00001;
	rf->t_near = (double)INT_MAX;

	get_clr_components_t(rgb_light_arr, disc->rgb_light, \
	rf->light_ratio, master->rt->light->brightness, MAX(pow(rf->light_dist / rf->light_intensity, 2), 1));
	rgb_light = get_clr_int(rgb_light_arr);
	rgb_ambient = get_clr_int(disc->rgb_ambient);
	
	trace_shadow(master, rf, rgb_ambient, rgb_light);
}

void	light_shader(t_rayfinder *rf, void *object_ptr, t_master *master)
{
	t_sphere	*sphere;
	int			rgb_light_arr[3];
	
	sphere = (t_sphere *)object_ptr;
	/* if (sphere->mode == HIGHLIGHT)
	{
		rf->clr = 0x00FF00FF;
		return ;
	} */
	get_clr_components(rgb_light_arr, sphere->rgb, 1, master->rt->light->brightness);
	rf->clr = get_clr_int(rgb_light_arr);
}
