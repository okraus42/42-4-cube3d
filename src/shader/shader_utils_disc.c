/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shader_utils_disc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 15:57:11 by okraus            #+#    #+#             */
/*   Updated: 2024/03/12 16:53:32 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	set_disc_texture(t_shader *shader,
	t_disc *disc, t_vect3f intersection)
{
	t_map	m;

	m.invq = (t_quat){0., 0., 1., 0.};
	m.p.x = intersection.x - disc->coords[X];
	m.p.y = intersection.y - disc->coords[Y];
	m.p.z = intersection.z - disc->coords[Z];
	rotate_vect(&m.p, disc->q);
	if (disc->is_inversed)
		rotate_vect(&m.p, m.invq);
	m.s[0] = disc->tx_disc->width;
	m.s[1] = disc->tx_disc->height;
	m.s[2] = 1 * disc->radius;
	get_disc_uv(m.p, &m.u, &m.v, m.s);
	m.w = m.s[0] * 4;
	shader->rgb_object[R] = disc->tx_disc->pixels[(m.v
			* m.w) + (m.u * 4)];
	shader->rgb_object[G] = disc->tx_disc->pixels[(m.v
			* m.w) + (m.u * 4) + 1];
	shader->rgb_object[B] = disc->tx_disc->pixels[(m.v
			* m.w) + (m.u * 4) + 2];
}

void	set_disc_checkerboard(t_shader *shader,
	t_disc *disc, t_vect3f intersection)
{
	t_map	m;

	m.invq = (t_quat){0., 0., 1., 0.};
	m.p.x = intersection.x - disc->coords[X];
	m.p.y = intersection.y - disc->coords[Y];
	m.p.z = intersection.z - disc->coords[Z];
	m.s[0] = (int)disc->checkerboard->magnitude / 2;
	m.s[1] = (int)disc->checkerboard->magnitude / 2;
	m.s[2] = 1 * disc->radius;
	rotate_vect(&m.p, disc->q);
	if (disc->is_inversed)
		rotate_vect(&m.p, m.invq);
	get_disc_uv(m.p, &m.u, &m.v, m.s);
	if (((m.u & 1) && (m.v & 1)) || (!(m.u & 1) && !(m.v & 1)))
	{
		shader->rgb_object[R] = disc->checkerboard->rgb1[R];
		shader->rgb_object[G] = disc->checkerboard->rgb1[G];
		shader->rgb_object[B] = disc->checkerboard->rgb1[B];
	}
	else
	{
		shader->rgb_object[R] = disc->checkerboard->rgb2[R];
		shader->rgb_object[G] = disc->checkerboard->rgb2[G];
		shader->rgb_object[B] = disc->checkerboard->rgb2[B];
	}
}

void	set_disc_rgb(t_shader *shader, t_disc *disc, t_vect3f intersection)
{
	if (disc->texture && disc->tx_disc)
	{
		set_disc_texture(shader, disc, intersection);
	}
	else if (disc->checkerboard)
	{
		set_disc_checkerboard(shader, disc, intersection);
	}
	else
	{
		shader->rgb_object[R] = disc->rgb[R];
		shader->rgb_object[G] = disc->rgb[G];
		shader->rgb_object[B] = disc->rgb[B];
	}
}

void	set_disc_normal2(t_shader *shader, t_disc *disc, t_map m)
{
	rotate_vect(&m.p, disc->q);
	if (disc->is_inversed)
		rotate_vect(&m.p, m.invq);
	m.s[0] = disc->vm_disc->width;
	m.s[1] = disc->vm_disc->height;
	m.s[2] = 1 * disc->radius;
	get_disc_uv(m.p, &m.u, &m.v, m.s);
	m.w = m.s[0] * 4;
	m.newnormal = get_new_normal(disc->vm_disc->pixels[(m.v
				* m.w) + (m.u * 4)],
			disc->vm_disc->pixels[(m.v * m.w)
			+ (m.u * 4) + 1],
			disc->vm_disc->pixels[(m.v * m.w)
			+ (m.u * 4) + 2]);
	if (disc->is_inversed)
		rotate_vect(&m.newnormal, m.invq);
	m.invq = get_inverse_quat(disc->q);
	rotate_vect(&m.newnormal, m.invq);
	shader->hit_normal.x = m.newnormal.x;
	shader->hit_normal.y = m.newnormal.y;
	shader->hit_normal.z = m.newnormal.z;
}

void	set_disc_normal(t_shader *shader, t_disc *disc, t_vect3f intersection)
{
	t_map	m;

	m.invq = (t_quat){0., 0., 1., 0.};
	if (disc->vector_map)
	{
		if (disc->vm_disc)
		{
			m.p.x = intersection.x - disc->coords[X];
			m.p.y = intersection.y - disc->coords[Y];
			m.p.z = intersection.z - disc->coords[Z];
			set_disc_normal2(shader, disc, m);
		}
	}
}
