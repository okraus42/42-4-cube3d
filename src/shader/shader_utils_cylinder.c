/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shader_utils_cylinder.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 15:57:08 by okraus            #+#    #+#             */
/*   Updated: 2024/03/13 17:14:57 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	set_cylinder_texture(t_shader *shader,
	t_cylinder *cylinder, t_vect3f intersection)
{
	t_map	m;

	m.p.x = intersection.x - cylinder->coords[X];
	m.p.y = intersection.y - cylinder->coords[Y];
	m.p.z = intersection.z - cylinder->coords[Z];
	rotate_vect(&m.p, cylinder->q);
	m.s[0] = cylinder->texture->tx_main->width;
	m.s[1] = cylinder->texture->tx_main->height;
	m.s[2] = cylinder->height;
	get_cylinder_uv(m.p, &m.u, &m.v, m.s);
	m.w = m.s[0] * 4;
	shader->rgb_object[R] = cylinder->texture->tx_main->pixels[(m.v
			* m.w) + (m.u * 4)];
	shader->rgb_object[G] = cylinder->texture->tx_main->pixels[(m.v
			* m.w) + (m.u * 4) + 1];
	shader->rgb_object[B] = cylinder->texture->tx_main->pixels[(m.v
			* m.w) + (m.u * 4) + 2];
}

void	set_cylinder_checkerboard(t_shader *shader,
	t_cylinder *cylinder, t_vect3f intersection)
{
	t_map	m;

	m.p.x = intersection.x - cylinder->coords[X];
	m.p.y = intersection.y - cylinder->coords[Y];
	m.p.z = intersection.z - cylinder->coords[Z];
	m.s[0] = (int)cylinder->checkerboard->magnitude;
	m.s[1] = (int)(0.5 + cylinder->checkerboard->magnitude
			* cylinder->height / (M_PI * cylinder->radius * 2));
	m.s[2] = cylinder->height;
	rotate_vect(&m.p, cylinder->q);
	get_cylinder_uv(m.p, &m.u, &m.v, m.s);
	if (((m.u & 1) && (m.v & 1)) || (!(m.u & 1) && !(m.v & 1)))
	{
		shader->rgb_object[R] = cylinder->checkerboard->rgb1[R];
		shader->rgb_object[G] = cylinder->checkerboard->rgb1[G];
		shader->rgb_object[B] = cylinder->checkerboard->rgb1[B];
	}
	else
	{
		shader->rgb_object[R] = cylinder->checkerboard->rgb2[R];
		shader->rgb_object[G] = cylinder->checkerboard->rgb2[G];
		shader->rgb_object[B] = cylinder->checkerboard->rgb2[B];
	}
}

void	set_cylinder_rgb(t_shader *shader,
	t_cylinder *cylinder, t_vect3f intersection)
{
	if (cylinder->texture && cylinder->texture->tx_main)
	{
		set_cylinder_texture(shader, cylinder, intersection);
	}
	else if (cylinder->checkerboard)
	{
		set_cylinder_checkerboard(shader, cylinder, intersection);
	}
	else
	{
		shader->rgb_object[R] = cylinder->rgb[R];
		shader->rgb_object[G] = cylinder->rgb[G];
		shader->rgb_object[B] = cylinder->rgb[B];
	}
}

void	set_cylinder_normal2(t_shader *shader, t_cylinder *cylinder, t_map m)
{
	m.invq = get_inverse_quat(cylinder->q);
	rotate_vect(&m.p, cylinder->q);
	rotate_vect(&(shader->hit_normal), cylinder->q);
	m.tanq = get_cylinder_tan_quat(shader->hit_normal);
	rotate_vect(&(shader->hit_normal), m.tanq);
	m.s[0] = cylinder->vector_map->vm_main->width;
	m.s[1] = cylinder->vector_map->vm_main->height;
	m.s[2] = cylinder->height;
	get_cylinder_uv(m.p, &m.u, &m.v, m.s);
	m.w = m.s[0] * 4;
	m.newnormal
		= get_new_normal(cylinder->vector_map->vm_main->pixels[(m.v
				* m.w) + (m.u * 4)],
			cylinder->vector_map->vm_main->pixels[(m.v * m.w)
			+ (m.u * 4) + 1],
			cylinder->vector_map->vm_main->pixels[(m.v * m.w)
			+ (m.u * 4) + 2]);
	m.invt = get_inverse_quat(m.tanq);
	rotate_vect(&m.newnormal, m.invt);
	rotate_vect(&m.newnormal, m.invq);
	shader->hit_normal.x = m.newnormal.x;
	shader->hit_normal.y = m.newnormal.y;
	shader->hit_normal.z = m.newnormal.z;
}

void	set_cylinder_normal(t_shader *shader,
	t_cylinder *cylinder, t_vect3f intersection)
{
	t_map	m;

	if (cylinder->vector_map)
	{
		if (cylinder->vector_map->vm_main)
		{
			m.p.x = intersection.x - cylinder->coords[X];
			m.p.y = intersection.y - cylinder->coords[Y];
			m.p.z = intersection.z - cylinder->coords[Z];
			set_cylinder_normal2(shader, cylinder, m);
		}
	}
}
