/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shader_utils_cone.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 15:57:04 by okraus            #+#    #+#             */
/*   Updated: 2024/03/13 17:40:08 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	set_cone_texture(t_shader *shader,
	t_cone *cone, t_vect3f intersection)
{
	t_map	m;

	m.p.x = intersection.x - cone->coords[X];
	m.p.y = intersection.y - cone->coords[Y];
	m.p.z = intersection.z - cone->coords[Z];
	rotate_vect(&m.p, cone->q);
	m.s[0] = cone->texture->tx_main->width;
	m.s[1] = cone->texture->tx_main->height;
	m.s[2] = cone->height;
	get_cone_uv(m.p, &m.u, &m.v, m.s);
	m.w = m.s[0] * 4;
	shader->rgb_object[R] = cone->texture->tx_main->pixels[(m.v
			* m.w) + (m.u * 4)];
	shader->rgb_object[G] = cone->texture->tx_main->pixels[(m.v
			* m.w) + (m.u * 4) + 1];
	shader->rgb_object[B] = cone->texture->tx_main->pixels[(m.v
			* m.w) + (m.u * 4) + 2];
}

void	set_cone_checkerboard(t_shader *shader,
	t_cone *cone, t_vect3f intersection)
{
	t_map	m;

	m.p.x = intersection.x - cone->coords[X];
	m.p.y = intersection.y - cone->coords[Y];
	m.p.z = intersection.z - cone->coords[Z];
	m.s[0] = (int)cone->checkerboard->magnitude;
	m.s[1] = (int)(0.5 + cone->checkerboard->magnitude
			* sqrt(cone->height * cone->height + cone->radius
				* cone->radius) / (M_PI * cone->radius * 2));
	m.s[2] = cone->height;
	rotate_vect(&m.p, cone->q);
	get_cone_uv(m.p, &m.u, &m.v, m.s);
	if (((m.u & 1) && (m.v & 1)) || (!(m.u & 1) && !(m.v & 1)))
	{
		shader->rgb_object[R] = cone->checkerboard->rgb1[R];
		shader->rgb_object[G] = cone->checkerboard->rgb1[G];
		shader->rgb_object[B] = cone->checkerboard->rgb1[B];
	}
	else
	{
		shader->rgb_object[R] = cone->checkerboard->rgb2[R];
		shader->rgb_object[G] = cone->checkerboard->rgb2[G];
		shader->rgb_object[B] = cone->checkerboard->rgb2[B];
	}
}

void	set_cone_rgb(t_shader *shader, t_cone *cone, t_vect3f intersection)
{
	if (cone->texture && cone->texture->tx_main)
	{
		set_cone_texture(shader, cone, intersection);
	}
	else if (cone->checkerboard)
	{
		set_cone_checkerboard(shader, cone, intersection);
	}
	else
	{
		shader->rgb_object[R] = cone->rgb[R];
		shader->rgb_object[G] = cone->rgb[G];
		shader->rgb_object[B] = cone->rgb[B];
	}
}

void	set_cone_normal2(t_shader *shader, t_cone *cone, t_map m)
{
	m.invq = get_inverse_quat(cone->q);
	rotate_vect(&m.p, cone->q);
	m.s[0] = cone->vector_map->vm_main->width;
	m.s[1] = cone->vector_map->vm_main->height;
	m.s[2] = cone->height;
	get_cone_uv(m.p, &m.u, &m.v, m.s);
	m.w = m.s[0] * 4;
	m.newnormal = get_new_normal(cone->vector_map->vm_main->pixels[(m.v
				* m.w) + (m.u * 4)],
			cone->vector_map->vm_main->pixels[(m.v * m.w)
			+ (m.u * 4) + 1],
			cone->vector_map->vm_main->pixels[(m.v * m.w)
			+ (m.u * 4) + 2]);
	m.invt = get_inverse_quat(m.tanq);
	rotate_vect(&m.newnormal, m.invt);
	rotate_vect(&m.newnormal, m.invq);
	shader->hit_normal.x = m.newnormal.x;
	shader->hit_normal.y = m.newnormal.y;
	shader->hit_normal.z = m.newnormal.z;
}

void	set_cone_normal(t_shader *shader, t_cone *cone, t_vect3f intersection)
{
	t_map	m;

	if (cone->vector_map)
	{
		if (cone->vector_map->vm_main)
		{
			m.p.x = intersection.x - cone->coords[X];
			m.p.y = intersection.y - cone->coords[Y];
			m.p.z = intersection.z - cone->coords[Z];
			rotate_vect(&(shader->hit_normal), cone->q);
			m.tanq = get_cone_tan_quat(shader->hit_normal, cone, intersection);
			set_cone_normal2(shader, cone, m);
		}
	}
}
