/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shader_utils_sphere.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 15:56:55 by okraus            #+#    #+#             */
/*   Updated: 2024/03/05 18:55:39 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	set_sphere_texture(t_shader *shader,
	t_sphere *sphere, t_vect3f intersection)
{
	t_map	m;

	m.p.x = intersection.x - sphere->coords[X];
	m.p.y = intersection.y - sphere->coords[Y];
	m.p.z = intersection.z - sphere->coords[Z];
	rotate_vect(&m.p, sphere->q);
	m.s[0] = sphere->texture->tx_main->width;
	m.s[1] = sphere->texture->tx_main->height;
	get_sphere_uv(m.p, &m.u, &m.v, m.s);
	m.w = m.s[0] * 4;
	shader->rgb_object[R] = sphere->texture->tx_main->pixels[(m.v
			* m.w) + (m.u * 4)];
	shader->rgb_object[G] = sphere->texture->tx_main->pixels[(m.v
			* m.w) + (m.u * 4) + 1];
	shader->rgb_object[B] = sphere->texture->tx_main->pixels[(m.v
			* m.w) + (m.u * 4) + 2];
}

void	set_sphere_checkerboard(t_shader *shader,
	t_sphere *sphere, t_vect3f intersection)
{
	t_map	m;

	m.p.x = intersection.x - sphere->coords[X];
	m.p.y = intersection.y - sphere->coords[Y];
	m.p.z = intersection.z - sphere->coords[Z];
	rotate_vect(&m.p, sphere->q);
	m.s[0] = (int)sphere->checkerboard->magnitude;
	m.s[1] = (int)sphere->checkerboard->magnitude;
	get_sphere_uv(m.p, &m.u, &m.v, m.s);
	if (((m.u & 1) && (m.v & 1)) || (!(m.u & 1) && !(m.v & 1)))
	{
		shader->rgb_object[R] = sphere->checkerboard->rgb1[R];
		shader->rgb_object[G] = sphere->checkerboard->rgb1[G];
		shader->rgb_object[B] = sphere->checkerboard->rgb1[B];
	}
	else
	{
		shader->rgb_object[R] = sphere->checkerboard->rgb2[R];
		shader->rgb_object[G] = sphere->checkerboard->rgb2[G];
		shader->rgb_object[B] = sphere->checkerboard->rgb2[B];
	}
}

void	set_sphere_rgb(t_shader *shader,
	t_sphere *sphere, t_vect3f intersection)
{
	if (sphere->texture && sphere->texture->tx_main)
	{
		set_sphere_texture(shader, sphere, intersection);
	}
	else if (sphere->checkerboard)
	{
		set_sphere_checkerboard(shader, sphere, intersection);
	}
	else
	{
		shader->rgb_object[R] = sphere->rgb[R];
		shader->rgb_object[G] = sphere->rgb[G];
		shader->rgb_object[B] = sphere->rgb[B];
	}
}

void	set_sphere_normal2(t_shader *shader, t_sphere *sphere, t_map m)
{
	m.invq = get_inverse_quat(sphere->q);
	rotate_vect(&m.p, sphere->q);
	rotate_vect(&(shader->hit_normal), sphere->q);
	m.tanq = get_sphere_tan_quat(shader->hit_normal);
	rotate_vect(&(shader->hit_normal), m.tanq);
	m.s[0] = sphere->vector_map->vm_main->width;
	m.s[1] = sphere->vector_map->vm_main->height;
	get_sphere_uv(m.p, &m.u, &m.v, m.s);
	m.w = m.s[0] * 4;
	m.newnormal = get_new_normal(sphere->vector_map->vm_main->pixels[(m.v
				* m.w) + (m.u * 4)],
			sphere->vector_map->vm_main->pixels[(m.v
				* m.w) + (m.u * 4) + 1],
			sphere->vector_map->vm_main->pixels[(m.v
				* m.w) + (m.u * 4) + 2]);
	m.invt = get_inverse_quat(m.tanq);
	rotate_vect(&m.newnormal, m.invt);
	rotate_vect(&m.newnormal, m.invq);
	shader->hit_normal.x = m.newnormal.x;
	shader->hit_normal.y = m.newnormal.y;
	shader->hit_normal.z = m.newnormal.z;
}

void	set_sphere_normal(t_shader *shader,
	t_sphere *sphere, t_vect3f intersection)
{
	t_map	m;

	if (sphere->vector_map)
	{
		if (sphere->vector_map->vm_main)
		{
			m.p.x = intersection.x - sphere->coords[X];
			m.p.y = intersection.y - sphere->coords[Y];
			m.p.z = intersection.z - sphere->coords[Z];
			set_sphere_normal2(shader, sphere, m);
		}
	}
}
