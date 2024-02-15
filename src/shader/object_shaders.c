/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_shaders.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 17:40:11 by plouda            #+#    #+#             */
/*   Updated: 2024/02/15 15:32:18 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	sphere_shader(t_rayfinder *rf, t_vect3f intersection, void *object_ptr, t_master *master)
{
	t_sphere	*sphere;
	t_shader	shader;
	int			i;

	sphere = (t_sphere *)object_ptr;
	i = 0;
	set_sphere_rgb(&shader, sphere, intersection);
	set_ambient_intensity(&shader, master->rt->ambient->rgb, master->rt->ambient->ratio); // ambient light as default
	shader.light_intensity = rf->light_intensity; // unused at the moment
	shader.hit_normal = subtract_vect3f(intersection, array_to_vect(sphere->coords)); // specific to object
	shader.view_dir = subtract_vect3f(array_to_vect(master->rt->camera->coords), intersection);
	normalize(&shader.view_dir);
	normalize(&shader.hit_normal);
	while (i < master->rt->n_lights)
	{
		shader.light_dir = subtract_vect3f(array_to_vect(master->rt->light_spheres[i]->coords), intersection);
		normalize(&shader.light_dir);
		diff_and_spec_ratios(&shader, *master->options);
		trace_shadow(master, rf, intersection, &shader, master->rt->light_spheres[i]->coords);
		phong_illumination(&shader, master->rt->light_spheres[i]);
		i++;
	}
	shader.pix_color[R] = 255 * shader.illumination[R];
	shader.pix_color[G] = 255 * shader.illumination[G];
	shader.pix_color[B] = 255 * shader.illumination[B];
	rf->clr = get_clr_int(shader.pix_color);
}

void	plane_shader(t_rayfinder *rf, t_vect3f intersection, void *object_ptr, t_master *master)
{
	t_plane		*plane;
	t_shader	shader;
	int			i;
	
	plane = (t_plane *)object_ptr;
	i = 0;
	set_plane_rgb(&shader, plane, intersection);
	set_ambient_intensity(&shader, master->rt->ambient->rgb, master->rt->ambient->ratio); // ambient light as default
	shader.light_intensity = rf->light_intensity;
	shader.hit_normal = *plane->normal;
	shader.view_dir = subtract_vect3f(array_to_vect(master->rt->camera->coords), intersection);
	normalize(&shader.view_dir);
	normalize(&shader.hit_normal);
	while (i < master->rt->n_lights)
	{
		shader.light_dir = subtract_vect3f(array_to_vect(master->rt->light_spheres[i]->coords), intersection);
		normalize(&shader.light_dir);
		diff_and_spec_ratios(&shader, *master->options);
		trace_shadow(master, rf, intersection, &shader, master->rt->light_spheres[i]->coords);
		phong_illumination(&shader, master->rt->light_spheres[i]);
		i++;
	}
	shader.pix_color[R] = 255 * shader.illumination[R];
	shader.pix_color[G] = 255 * shader.illumination[G];
	shader.pix_color[B] = 255 * shader.illumination[B];
	rf->clr = get_clr_int(shader.pix_color);
}

void	cylinder_shader(t_rayfinder *rf, t_vect3f intersection, void *object_ptr, t_master *master, t_ray ray)
{
	t_cylinder	*cylinder;
	t_shader	shader;
	int			i;
	
	cylinder = (t_cylinder *)object_ptr;
	i = 0;
	set_cylinder_rgb(&shader, cylinder, intersection);
	set_ambient_intensity(&shader, master->rt->ambient->rgb, master->rt->ambient->ratio); // ambient light as default
	shader.light_intensity = rf->light_intensity;
	shader.hit_normal = get_cylinder_hit_normal(rf, ray, intersection, *cylinder);
	shader.view_dir = subtract_vect3f(array_to_vect(master->rt->camera->coords), intersection);
	normalize(&shader.view_dir);
	normalize(&shader.hit_normal);
	while (i < master->rt->n_lights)
	{
		shader.light_dir = subtract_vect3f(array_to_vect(master->rt->light_spheres[i]->coords), intersection);
		normalize(&shader.light_dir);
		diff_and_spec_ratios(&shader, *master->options);
		trace_shadow(master, rf, intersection, &shader, master->rt->light_spheres[i]->coords);
		phong_illumination(&shader, master->rt->light_spheres[i]);
		i++;
	}
	shader.pix_color[R] = 255 * shader.illumination[R];
	shader.pix_color[G] = 255 * shader.illumination[G];
	shader.pix_color[B] = 255 * shader.illumination[B];
	rf->clr = get_clr_int(shader.pix_color);
}

void	disc_shader(t_rayfinder *rf, t_vect3f intersection, void *object_ptr, t_master *master)
{
	t_disc		*disc;
	t_shader	shader;
	int			i;
	
	disc = (t_disc *)object_ptr;
	i = 0;
	set_disc_rgb(&shader, disc, intersection);
	set_ambient_intensity(&shader, master->rt->ambient->rgb, master->rt->ambient->ratio); // ambient light as default
	shader.light_intensity = rf->light_intensity;
	shader.hit_normal = *disc->normal;
	shader.view_dir = subtract_vect3f(array_to_vect(master->rt->camera->coords), intersection);
	normalize(&shader.view_dir);
	normalize(&shader.hit_normal);
	while (i < master->rt->n_lights)
	{
		shader.light_dir = subtract_vect3f(array_to_vect(master->rt->light_spheres[i]->coords), intersection);
		normalize(&shader.light_dir);
		diff_and_spec_ratios(&shader, *master->options);
		trace_shadow(master, rf, intersection, &shader, master->rt->light_spheres[i]->coords);
		phong_illumination(&shader, master->rt->light_spheres[i]);
		i++;
	}
	shader.pix_color[R] = 255 * shader.illumination[R];
	shader.pix_color[G] = 255 * shader.illumination[G];
	shader.pix_color[B] = 255 * shader.illumination[B];
	rf->clr = get_clr_int(shader.pix_color);
}

void	cone_shader(t_rayfinder *rf, t_vect3f intersection, void *object_ptr, t_master *master)
{
	t_cone	*cone;
	t_shader	shader;
	int			i;
	
	cone = (t_cone *)object_ptr;
	i = 0;
	
	set_cone_rgb(&shader, cone, intersection);
	set_ambient_intensity(&shader, master->rt->ambient->rgb, master->rt->ambient->ratio); // ambient light as default
	shader.light_intensity = rf->light_intensity;
	shader.hit_normal = get_cone_hit_normal(intersection, *cone);
	shader.view_dir = subtract_vect3f(array_to_vect(master->rt->camera->coords), intersection);
	normalize(&shader.view_dir);
	normalize(&shader.hit_normal);
	while (i < master->rt->n_lights)
	{
		shader.light_dir = subtract_vect3f(array_to_vect(master->rt->light_spheres[i]->coords), intersection);
		normalize(&shader.light_dir);
		diff_and_spec_ratios(&shader, *master->options);
		trace_shadow(master, rf, intersection, &shader, master->rt->light_spheres[i]->coords);
		phong_illumination(&shader, master->rt->light_spheres[i]);
		i++;
	}
	shader.pix_color[R] = 255 * shader.illumination[R];
	shader.pix_color[G] = 255 * shader.illumination[G];
	shader.pix_color[B] = 255 * shader.illumination[B];
	rf->clr = get_clr_int(shader.pix_color);
}

void	light_shader(t_rayfinder *rf, void *object_ptr, t_master *master)
{
	t_sphere	*sphere;
	int			rgb_light_arr[3];
	
	(void)master;
	sphere = (t_sphere *)object_ptr;
	get_clr_components(rgb_light_arr, sphere->rgb, 1, sphere->brightness);
	rf->clr = get_clr_int(rgb_light_arr);
}
