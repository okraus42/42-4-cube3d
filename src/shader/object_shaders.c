/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_shaders.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 17:40:11 by plouda            #+#    #+#             */
/*   Updated: 2024/01/17 11:58:52 by plouda           ###   ########.fr       */
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

void	sphere_shader(t_rayfinder *rf, t_vect3f intersection, void *object_ptr, t_master *master)
{
	t_sphere	*sphere;
	int			rgb_light_arr[3];
	int			rgb_spec_arr[3];
	//int			rgb_comb_arr[3];
	uint32_t	rgb_light;
	uint32_t	rgb_ambient;
	uint32_t	rgb_spec;
	t_vect3f	incident_light;
	t_vect3f	reflect_vect;
	t_vect3f	view_dir;
	double		dot_reflect;
	//uint32_t	rgb_comb;
	
	rgb_light_arr[0] = 0;
	rgb_light_arr[1] = 0;
	rgb_light_arr[2] = 0;
	sphere = (t_sphere *)object_ptr;
	if (sphere->mode == HIGHLIGHT)
	{
		rf->clr = 0x00FF00FF;
		return ;
	}
	rf->hit_normal = subtract_vect3f(intersection, array_to_vect(sphere->coords));
	rf->light_dir = subtract_vect3f(array_to_vect(master->rt->light->coords), intersection);
	//view_dir = invert_vect3f(intersection);
	view_dir.x = master->rt->camera->coords[X] - intersection.x;
	view_dir.y = master->rt->camera->coords[Y] - intersection.y;
	view_dir.z = master->rt->camera->coords[Z] - intersection.z;
	normalize(&view_dir);
	normalize(&rf->hit_normal);
	normalize(&rf->light_dir);
	incident_light = invert_vect3f(rf->light_dir);
	//incident_light = rf->light_dir;
	dot_reflect = dot_product(rf->hit_normal, incident_light); //  MAX(dot, 0) expands the white to the whole sphere
	//clampf(0, 1, &dot_reflect);
	reflect_vect.x = incident_light.x - 2 * dot_reflect * rf->hit_normal.x;
	reflect_vect.y = incident_light.y - 2 * dot_reflect * rf->hit_normal.y;
	reflect_vect.z = incident_light.z - 2 * dot_reflect * rf->hit_normal.z;
	normalize(&reflect_vect);
	
	rf->light_ratio = dot_product(rf->hit_normal, rf->light_dir);
	clampf(0, 1, &rf->light_ratio);

	//clampf(0, 1, &rf->light_ratio);
	
	rf->shadowray.direction = rf->light_dir;
	rf->shadowray.origin = add_vect3f(intersection, scale_vect3f(1e-4, rf->hit_normal));
	rf->light_dist = point_distance(rf->shadowray.origin, array_to_vect(master->rt->light->coords));
	rf->t_near = (double)INT_MAX;
	if (rf->light_dist < 1e-6)
		rf->light_dist = 0.00001;
	get_clr_components_t(rgb_light_arr, sphere->rgb_light, \
		rf->light_ratio, master->rt->light->brightness, MAX(pow(rf->light_dist / rf->light_intensity, 2), 1));
	rf->light_ratio = pow(MAX(dot_product(view_dir, reflect_vect), 0), 4);
	get_clr_components_t(rgb_spec_arr, master->rt->light->rgb, \
		rf->light_ratio, master->rt->light->brightness, MAX(pow(rf->light_dist / rf->light_intensity, 2), 1)); // needs protection against 0 in light_intensity

	//get_comb_clr(rgb_comb_arr, rgb_light_arr, rgb_spec_arr);

	rgb_spec = get_clr_int(rgb_spec_arr);
	rgb_light = get_clr_int(rgb_light_arr);
	rgb_ambient = get_clr_int(sphere->rgb_ambient);
	//rgb_comb = get_clr_int(rgb_comb_arr);
	trace_shadow_t(master, rf, rgb_ambient, rgb_light, rgb_spec);
}

void	plane_shader(t_rayfinder *rf, t_vect3f intersection, void *object_ptr, t_master *master)
{
	t_plane		*plane;
	int			rgb_light_arr[3];
	uint32_t	rgb_light;
	uint32_t	rgb_ambient;
	
	plane = (t_plane *)object_ptr;
	if (plane->mode == HIGHLIGHT)
	{
		rf->clr = 0x00FF00FF;
		return ;
	}
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
	if (cylinder->mode == HIGHLIGHT)
	{
		rf->clr = 0x00FF00FF;
		return ;
	}
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
	if (disc->mode == HIGHLIGHT)
	{
		rf->clr = 0x00FF00FF;
		return ;
	}
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
	if (sphere->mode == HIGHLIGHT)
	{
		rf->clr = 0x00FF00FF;
		return ;
	}
	get_clr_components(rgb_light_arr, sphere->rgb, 1, master->rt->light->brightness);
	rf->clr = get_clr_int(rgb_light_arr);
}
