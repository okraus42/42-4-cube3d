/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_shaders.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 17:40:11 by plouda            #+#    #+#             */
/*   Updated: 2024/01/23 12:34:04 by plouda           ###   ########.fr       */
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

void	sphere_shader(t_rayfinder *rf, t_vect3f intersection, void *object_ptr, t_master *master)
{
	t_sphere	*sphere;
	t_shader	shader;

	sphere = (t_sphere *)object_ptr;
	/* if (sphere->mode == HIGHLIGHT)
	{
		rf->clr = 0x00FF00FF;
		return ;
	} */
	shader.light_intensity = rf->light_intensity;
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

	rf->shadowray.direction = shader.light_dir;
	rf->shadowray.origin = add_vect3f(intersection, scale_vect3f(1e-4, shader.hit_normal));
	shader.light_dist = point_distance(rf->shadowray.origin, array_to_vect(master->rt->light->coords));
	rf->t_near = (double)INT_MAX;
	if (shader.light_dist < 1e-6)
		shader.light_dist = 0.00001;
	
	if (shader.light_intensity != 0)
	{
		shader.falloff = MAX(pow(shader.light_dist / shader.light_intensity, 2), 1);
		shader.diffuse_ratio = dot_product(shader.hit_normal, shader.light_dir);
		clampf(0, 1, &shader.diffuse_ratio);
		get_clr_components_t(shader.rgb_diffuse_arr, sphere->rgb_light, \
			shader.diffuse_ratio, master->rt->light->brightness, shader.falloff);
		shader.specular_ratio = master->options->glossiness * pow(MAX(dot_product(shader.view_dir, shader.reflect_vect), 0), master->options->spec_highlight_size);
		clampf(0, 1, &shader.specular_ratio);
		get_clr_components_t(shader.rgb_specular_arr, master->rt->light->rgb, \
			shader.specular_ratio, master->rt->light->brightness, shader.falloff);
		shader.rgb_specular = get_clr_int(shader.rgb_specular_arr);
		shader.rgb_diffuse = get_clr_int(shader.rgb_diffuse_arr);
		shader.rgb_ambient = get_clr_int(sphere->rgb_ambient);
	}
	else
	{
		shader.rgb_ambient = get_clr_int(sphere->rgb_ambient);
		shader.rgb_specular = shader.rgb_ambient;
		shader.rgb_diffuse = shader.rgb_ambient;
	}
	trace_shadow_t(master, rf, shader);
}

void	plane_shader(t_rayfinder *rf, t_vect3f intersection, void *object_ptr, t_master *master)
{
	t_plane		*plane;
	t_shader	shader;
	
	plane = (t_plane *)object_ptr;
	/* if (plane->mode == HIGHLIGHT)
	{
		rf->clr = 0x00FF00FF;
		return ;
	} */
	shader.light_intensity = rf->light_intensity;
	shader.hit_normal = *plane->normal;
	shader.light_dir = subtract_vect3f(array_to_vect(master->rt->light->coords), intersection);
	shader.view_dir = subtract_vect3f(array_to_vect(master->rt->camera->coords), intersection);
	normalize(&shader.view_dir);
	normalize(&shader.hit_normal);
	normalize(&shader.light_dir);

	shader.diffuse_ratio = dot_product(shader.hit_normal, shader.light_dir);
	if (shader.diffuse_ratio < 0)
	{
		shader.diffuse_ratio = shader.diffuse_ratio * -1;
		shader.hit_normal = invert_vect3f(shader.hit_normal); // because shadowray's origin will be shadowed by the plane itself otherwise
	}
	shader.incident_light = shader.light_dir;
	shader.dot_reflect = MAX(dot_product(shader.hit_normal, shader.incident_light), 0);
	shader.reflect_vect.x = 2 * shader.dot_reflect * shader.hit_normal.x - shader.incident_light.x;
	shader.reflect_vect.y = 2 * shader.dot_reflect * shader.hit_normal.y - shader.incident_light.y;
	shader.reflect_vect.z = 2 * shader.dot_reflect * shader.hit_normal.z - shader.incident_light.z;
	normalize(&shader.reflect_vect);

	rf->shadowray.direction = shader.light_dir;
	rf->shadowray.origin = add_vect3f(intersection, scale_vect3f(1e-4, shader.hit_normal));
	shader.light_dist = point_distance(rf->shadowray.origin, array_to_vect(master->rt->light->coords));
	rf->t_near = (double)INT_MAX;
	if (shader.light_dist < 1e-6)
		shader.light_dist = 0.00001;
	
	if (shader.light_intensity != 0)
	{
		shader.falloff = MAX(pow(shader.light_dist / shader.light_intensity, 2), 1);
		//clampf(0, 1, &shader.diffuse_ratio);
		// this is where diffuse ratio condition used to be
		get_clr_components_t(shader.rgb_diffuse_arr, plane->rgb_light, \
			shader.diffuse_ratio, master->rt->light->brightness, shader.falloff);
		shader.specular_ratio = master->options->glossiness * pow(MAX(dot_product(shader.view_dir, shader.reflect_vect), 0), master->options->spec_highlight_size);
		clampf(0, 1, &shader.specular_ratio);
		get_clr_components_t(shader.rgb_specular_arr, master->rt->light->rgb, \
			shader.specular_ratio, master->rt->light->brightness, shader.falloff);
		shader.rgb_specular = get_clr_int(shader.rgb_specular_arr);
		shader.rgb_diffuse = get_clr_int(shader.rgb_diffuse_arr);
		shader.rgb_ambient = get_clr_int(plane->rgb_ambient);
	}
	else
	{
		shader.rgb_ambient = get_clr_int(plane->rgb_ambient);
		shader.rgb_specular = shader.rgb_ambient;
		shader.rgb_diffuse = shader.rgb_ambient;
	}
	trace_shadow_t(master, rf, shader);
}

void	cylinder_shader(t_rayfinder *rf, t_vect3f intersection, void *object_ptr, t_master *master, t_ray ray)
{
	t_cylinder	*cylinder;
	t_shader	shader;
	
	cylinder = (t_cylinder *)object_ptr;
	/* if (cylinder->mode == HIGHLIGHT)
	{
		rf->clr = 0x00FF00FF;
		return ;
	} */
	shader.light_intensity = rf->light_intensity;
	shader.hit_normal = get_hit_normal(rf, ray, intersection, *cylinder);
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

	rf->shadowray.direction = shader.light_dir;
	rf->shadowray.origin = add_vect3f(intersection, scale_vect3f(1e-4, shader.hit_normal));
	shader.light_dist = point_distance(rf->shadowray.origin, array_to_vect(master->rt->light->coords));
	rf->t_near = (double)INT_MAX;
	if (shader.light_dist < 1e-6)
		shader.light_dist = 0.00001;

	if (shader.light_intensity != 0)
	{
		shader.falloff = MAX(pow(shader.light_dist / shader.light_intensity, 2), 1);
		shader.diffuse_ratio = dot_product(shader.hit_normal, shader.light_dir);
		clampf(0, 1, &shader.diffuse_ratio);
		get_clr_components_t(shader.rgb_diffuse_arr, cylinder->rgb_light, \
			shader.diffuse_ratio, master->rt->light->brightness, shader.falloff);
		shader.specular_ratio = master->options->glossiness * pow(MAX(dot_product(shader.view_dir, shader.reflect_vect), 0), master->options->spec_highlight_size);
		clampf(0, 1, &shader.specular_ratio);
		get_clr_components_t(shader.rgb_specular_arr, master->rt->light->rgb, \
			shader.specular_ratio, master->rt->light->brightness, shader.falloff);
		shader.rgb_specular = get_clr_int(shader.rgb_specular_arr);
		shader.rgb_diffuse = get_clr_int(shader.rgb_diffuse_arr);
		shader.rgb_ambient = get_clr_int(cylinder->rgb_ambient);
	}
	else
	{
		shader.rgb_ambient = get_clr_int(cylinder->rgb_ambient);
		shader.rgb_specular = shader.rgb_ambient;
		shader.rgb_diffuse = shader.rgb_ambient;
	}
	trace_shadow_t(master, rf, shader);
}

void	disc_shader(t_rayfinder *rf, t_vect3f intersection, void *object_ptr, t_master *master)
{
	t_disc		*disc;
	t_shader	shader;
	
	disc = (t_disc *)object_ptr;
	/* if (disc->mode == HIGHLIGHT || disc->mode == DEFAULT)
	{
		rf->clr = 0x00FF00FF;
		return ;
	} */
	shader.light_intensity = rf->light_intensity;
	shader.hit_normal = *disc->normal;
	shader.light_dir = subtract_vect3f(array_to_vect(master->rt->light->coords), intersection);
	shader.view_dir = subtract_vect3f(array_to_vect(master->rt->camera->coords), intersection);
	normalize(&shader.view_dir);
	normalize(&shader.hit_normal);
	normalize(&shader.light_dir);

	shader.diffuse_ratio = dot_product(shader.hit_normal, shader.light_dir);
	if (shader.diffuse_ratio < 0)
	{
		shader.diffuse_ratio = shader.diffuse_ratio * -1;
		shader.hit_normal = invert_vect3f(shader.hit_normal); // because shadowray's origin will be shadowed by the plane itself otherwise
	}

	shader.incident_light = shader.light_dir;
	shader.dot_reflect = MAX(dot_product(shader.hit_normal, shader.incident_light), 0);
	shader.reflect_vect.x = 2 * shader.dot_reflect * shader.hit_normal.x - shader.incident_light.x;
	shader.reflect_vect.y = 2 * shader.dot_reflect * shader.hit_normal.y - shader.incident_light.y;
	shader.reflect_vect.z = 2 * shader.dot_reflect * shader.hit_normal.z - shader.incident_light.z;
	normalize(&shader.reflect_vect);

	rf->shadowray.direction = shader.light_dir;
	rf->shadowray.origin = add_vect3f(intersection, scale_vect3f(1e-4, shader.hit_normal));
	shader.light_dist = point_distance(rf->shadowray.origin, array_to_vect(master->rt->light->coords));
	rf->t_near = (double)INT_MAX;
	if (shader.light_dist < 1e-6)
		shader.light_dist = 0.00001;

	if (shader.light_intensity != 0)
	{
		shader.falloff = MAX(pow(shader.light_dist / shader.light_intensity, 2), 1);
		/* shader.diffuse_ratio = dot_product(shader.hit_normal, shader.light_dir);
		if (shader.diffuse_ratio < 0)
		{
			shader.diffuse_ratio = shader.diffuse_ratio * -1;
			shader.hit_normal = invert_vect3f(shader.hit_normal); // because shadowray's origin will be shadowed by the plane itself otherwise
		} */
		//clampf(0, 1, &shader.diffuse_ratio);
		get_clr_components_t(shader.rgb_diffuse_arr, disc->rgb_light, \
			shader.diffuse_ratio, master->rt->light->brightness, shader.falloff);
		shader.specular_ratio = master->options->glossiness * pow(MAX(dot_product(shader.view_dir, shader.reflect_vect), 0), master->options->spec_highlight_size);
		clampf(0, 1, &shader.specular_ratio);
		get_clr_components_t(shader.rgb_specular_arr, master->rt->light->rgb, \
			shader.specular_ratio, master->rt->light->brightness, shader.falloff);
		shader.rgb_specular = get_clr_int(shader.rgb_specular_arr);
		shader.rgb_diffuse = get_clr_int(shader.rgb_diffuse_arr);
		shader.rgb_ambient = get_clr_int(disc->rgb_ambient);
	}
	else
	{
		shader.rgb_ambient = get_clr_int(disc->rgb_ambient);
		shader.rgb_specular = shader.rgb_ambient;
		shader.rgb_diffuse = shader.rgb_ambient;
	}
	trace_shadow_t(master, rf, shader);
}

t_vect3f	get_cone_hit_normal(t_rayfinder *rf, t_ray ray, t_vect3f intersection, t_cone cone)
{
	t_vect3f	hit_normal;
	t_vect3f	centered;
	t_vect3f	axis;
	double		half_angle;
	double			m; // a scalar that determines the closest point on the axis to the hit point

	axis = invert_vect3f(*cone.normal);
	half_angle = atan2(cone.diameter / 2, cone.height);
	centered = subtract_center(ray.origin, cone.coords);
	m = (dot_product(ray.direction, axis) * rf->t_near) + dot_product(centered, axis);
	hit_normal.x = intersection.x - cone.coords[X] - ((1 + half_angle * half_angle) * axis.x * m);
	hit_normal.y = intersection.y - cone.coords[Y] - ((1 + half_angle * half_angle) * axis.y * m);
	hit_normal.z = intersection.z - cone.coords[Z] - ((1 + half_angle * half_angle) * axis.z * m) * m;
	normalize(&hit_normal);
	return (hit_normal);
}

// wrong normal calculation
void	cone_shader(t_rayfinder *rf, t_vect3f intersection, void *object_ptr, t_master *master, t_ray ray)
{
	t_cone	*cone;
	t_shader	shader;
	
	cone = (t_cone *)object_ptr;
	/* if (cone->mode == HIGHLIGHT || cone->mode == DEFAULT)
	{
		rf->clr = 0x00FF00FF;
		return ;
	} */
	shader.light_intensity = rf->light_intensity;
	shader.hit_normal = get_cone_hit_normal(rf, ray, intersection, *cone);
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

	rf->shadowray.direction = shader.light_dir;
	rf->shadowray.origin = add_vect3f(intersection, scale_vect3f(1e-4, shader.hit_normal));
	shader.light_dist = point_distance(rf->shadowray.origin, array_to_vect(master->rt->light->coords));
	rf->t_near = (double)INT_MAX;
	if (shader.light_dist < 1e-6)
		shader.light_dist = 0.00001;

	if (shader.light_intensity != 0)
	{
		shader.falloff = MAX(pow(shader.light_dist / shader.light_intensity, 2), 1);
		shader.diffuse_ratio = dot_product(shader.hit_normal, shader.light_dir);
		clampf(0, 1, &shader.diffuse_ratio);
		get_clr_components_t(shader.rgb_diffuse_arr, cone->rgb_light, \
			shader.diffuse_ratio, master->rt->light->brightness, shader.falloff);
		shader.specular_ratio = master->options->glossiness * pow(MAX(dot_product(shader.view_dir, shader.reflect_vect), 0), master->options->spec_highlight_size);
		clampf(0, 1, &shader.specular_ratio);
		get_clr_components_t(shader.rgb_specular_arr, master->rt->light->rgb, \
			shader.specular_ratio, master->rt->light->brightness, shader.falloff);
		shader.rgb_specular = get_clr_int(shader.rgb_specular_arr);
		shader.rgb_diffuse = get_clr_int(shader.rgb_diffuse_arr);
		shader.rgb_ambient = get_clr_int(cone->rgb_ambient);
	}
	else
	{
		shader.rgb_ambient = get_clr_int(cone->rgb_ambient);
		shader.rgb_specular = shader.rgb_ambient;
		shader.rgb_diffuse = shader.rgb_ambient;
	}
	trace_shadow_t(master, rf, shader);
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
