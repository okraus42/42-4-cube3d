/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_shaders.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 17:40:11 by plouda            #+#    #+#             */
/*   Updated: 2024/02/27 12:24:17 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	sphere_shader(t_rayfinder *rf, t_vect3f intersection, void *object_ptr, t_master *master)
{
	t_sphere	*sphere;
	t_shader	shader;
	int			i;

	sphere = (t_sphere *)object_ptr;
	shader.obj_glossiness = sphere->glossiness;
	i = 0;
	set_sphere_rgb(&shader, sphere, intersection);
	set_ambient_intensity(&shader, master->rt->ambient->rgb, master->rt->ambient->ratio); // ambient light as default
	shader.hit_normal = subtract_vect3f(intersection, array_to_vect(sphere->coords)); // specific to object
	normalize(&shader.hit_normal);
	set_sphere_normal(&shader, sphere, intersection);
	shader.view_dir = subtract_vect3f(array_to_vect(master->rt->camera->coords), intersection);
	normalize(&shader.view_dir);
	normalize(&shader.hit_normal);
	if (sphere->camera_inside)
		shader.hit_normal = invert_vect3f(shader.hit_normal);
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
	shader.obj_glossiness = plane->glossiness;
	i = 0;
	set_plane_rgb(&shader, plane, intersection);
	set_ambient_intensity(&shader, master->rt->ambient->rgb, master->rt->ambient->ratio); // ambient light as default
	shader.hit_normal = *plane->normal;
	normalize(&shader.hit_normal);
	set_plane_normal(&shader, plane, intersection);
	shader.view_dir = subtract_vect3f(array_to_vect(master->rt->camera->coords), intersection);
	normalize(&shader.view_dir);
	normalize(&shader.hit_normal);
	if (plane->camera_inside)
		shader.hit_normal = invert_vect3f(shader.hit_normal);
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
	shader.obj_glossiness = cylinder->glossiness;
	i = 0;
	set_cylinder_rgb(&shader, cylinder, intersection);
	set_ambient_intensity(&shader, master->rt->ambient->rgb, master->rt->ambient->ratio); // ambient light as default
	shader.hit_normal = get_cylinder_hit_normal(rf, ray, intersection, *cylinder);
	normalize(&shader.hit_normal);
	set_cylinder_normal(&shader, cylinder, intersection);
	shader.view_dir = subtract_vect3f(array_to_vect(master->rt->camera->coords), intersection);
	normalize(&shader.view_dir);
	normalize(&shader.hit_normal);
	if (cylinder->camera_inside)
		shader.hit_normal = invert_vect3f(shader.hit_normal);
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
	shader.obj_glossiness = disc->glossiness;
	i = 0;
	set_disc_rgb(&shader, disc, intersection);
	set_ambient_intensity(&shader, master->rt->ambient->rgb, master->rt->ambient->ratio); // ambient light as default
	shader.hit_normal = *disc->normal;
	normalize(&shader.hit_normal);
	set_disc_normal(&shader, disc, intersection);
	shader.view_dir = subtract_vect3f(array_to_vect(master->rt->camera->coords), intersection);
	normalize(&shader.view_dir);
	normalize(&shader.hit_normal);
	if (disc->camera_inside)
		shader.hit_normal = invert_vect3f(shader.hit_normal);
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
	shader.obj_glossiness = cone->glossiness;
	i = 0;
	set_cone_rgb(&shader, cone, intersection);
	set_ambient_intensity(&shader, master->rt->ambient->rgb, master->rt->ambient->ratio); // ambient light as default
	shader.hit_normal = get_cone_hit_normal(intersection, *cone);
	normalize(&shader.hit_normal);
	set_cone_normal(&shader, cone, intersection);
	shader.view_dir = subtract_vect3f(array_to_vect(master->rt->camera->coords), intersection);
	normalize(&shader.view_dir);
	normalize(&shader.hit_normal);
	if (cone->camera_inside)
		shader.hit_normal = invert_vect3f(shader.hit_normal);
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
