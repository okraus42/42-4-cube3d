/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shader_utils_plane.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 15:57:02 by okraus            #+#    #+#             */
/*   Updated: 2024/03/05 18:52:10 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	set_plane_texture(t_shader *shader,
	t_plane *plane, t_vect3f intersection)
{
	t_map	m;

	m.p.x = intersection.x - plane->coords[X];
	m.p.y = intersection.y - plane->coords[Y];
	m.p.z = intersection.z - plane->coords[Z];
	rotate_vect(&m.p, plane->q);
	m.s[0] = plane->texture->tx_main->width;
	m.s[1] = plane->texture->tx_main->height;
	get_plane_uv(m.p, &m.u, &m.v, m.s);
	m.w = m.s[0] * 4;
	shader->rgb_object[R] = plane->texture->tx_main->pixels[(m.v
			* m.w) + (m.u * 4)];
	shader->rgb_object[G] = plane->texture->tx_main->pixels[(m.v
			* m.w) + (m.u * 4) + 1];
	shader->rgb_object[B] = plane->texture->tx_main->pixels[(m.v
			* m.w) + (m.u * 4) + 2];
}

void	set_plane_checkerboard(t_shader *shader,
	t_plane *plane, t_vect3f intersection)
{
	t_map	m;

	m.p.x = intersection.x - plane->coords[X];
	m.p.y = intersection.y - plane->coords[Y];
	m.p.z = intersection.z - plane->coords[Z];
	m.s[0] = (int)plane->checkerboard->magnitude;
	m.s[1] = (int)plane->checkerboard->magnitude;
	rotate_vect(&m.p, plane->q);
	get_plane_uv(m.p, &m.u, &m.v, m.s);
	if (((m.u & 1) && (m.v & 1)) || (!(m.u & 1) && !(m.v & 1)))
	{
		shader->rgb_object[R] = plane->checkerboard->rgb1[R];
		shader->rgb_object[G] = plane->checkerboard->rgb1[G];
		shader->rgb_object[B] = plane->checkerboard->rgb1[B];
	}
	else
	{
		shader->rgb_object[R] = plane->checkerboard->rgb2[R];
		shader->rgb_object[G] = plane->checkerboard->rgb2[G];
		shader->rgb_object[B] = plane->checkerboard->rgb2[B];
	}
}

void	set_plane_rgb(t_shader *shader, t_plane *plane, t_vect3f intersection)
{
	if (plane->texture && plane->texture->tx_main)
	{
		set_plane_texture(shader, plane, intersection);
	}
	else if (plane->checkerboard)
	{
		set_plane_checkerboard(shader, plane, intersection);
	}
	else
	{
		shader->rgb_object[R] = plane->rgb[R];
		shader->rgb_object[G] = plane->rgb[G];
		shader->rgb_object[B] = plane->rgb[B];
	}
}

void	set_plane_normal2(t_shader *shader, t_plane *plane, t_map m)
{
	rotate_vect(&m.p, plane->q);
	m.s[0] = plane->vector_map->vm_main->width;
	m.s[1] = plane->vector_map->vm_main->height;
	get_plane_uv(m.p, &m.u, &m.v, m.s);
	m.w = m.s[0] * 4;
	m.newnormal = get_new_normal(plane->vector_map->vm_main->pixels[(m.v
				* m.w) + (m.u * 4)],
			plane->vector_map->vm_main->pixels[(m.v * m.w)
			+ (m.u * 4) + 1],
			plane->vector_map->vm_main->pixels[(m.v * m.w)
			+ (m.u * 4) + 2]);
	m.invq = get_inverse_quat(plane->q);
	rotate_vect(&m.newnormal, m.invq);
	shader->hit_normal = (t_vect3f){m.newnormal.x,
		m.newnormal.y, m.newnormal.z};
}

void	set_plane_normal(t_shader *shader,
	t_plane *plane, t_vect3f intersection)
{
	t_map	m;

	if (plane->vector_map)
	{
		if (plane->vector_map->vm_main)
		{
			m.p.x = intersection.x - plane->coords[X];
			m.p.y = intersection.y - plane->coords[Y];
			m.p.z = intersection.z - plane->coords[Z];
			set_plane_normal2(shader, plane, m);
		}
	}
}
