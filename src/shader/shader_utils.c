/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shader_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 17:46:40 by plouda            #+#    #+#             */
/*   Updated: 2024/03/04 18:34:20 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

double	point_distance(t_vect3f p1, t_vect3f p2)
{
	double	dist;

	dist = sqrt(pow(p1.x - p2.x, 2)
			+ pow(p1.y - p2.y, 2) + pow(p1.z - p2.z, 2));
	return (dist);
}

void	get_clr_components(int *light, int *rgb, double ratio, double bright)
{
	light[R] = (double)rgb[R] * ratio * bright;
	light[G] = (double)rgb[G] * ratio * bright;
	light[B] = (double)rgb[B] * ratio * bright;
}

uint32_t	get_clr_int(int *rgb)
{
	uint32_t	clr;

	clr = (rgb[R] << 24 | rgb[G] << 16 | rgb[B] << 8 | 0xFF);
	return (clr);
}

// uint32_t	ft_max_clr(uint32_t a, uint32_t b)
// {
// 	uint32_t	c;

// 	c = MAX(a & 0xFF000000, b & 0xFF000000) | MAX(a & 0xFF0000, b & 0xFF0000)
// 		| MAX(a & 0xFF00, b & 0xFF00) | 0xFF;
// 	return (c);
// }

void	clamp(int min, int max, int *value)
{
	if (*value < min)
		*value = min;
	else if (*value > max)
		*value = max;
}

void	clampf(double min, double max, double *value)
{
	if (*value < min)
		*value = min;
	else if (*value > max)
		*value = max;
}

t_vect3f	get_cylinder_hit_normal(t_vect3f intersection, t_cylinder cylinder)
{
	double		t;
	t_vect3f	normal;

	t = dot_product(*cylinder.normal, subtract_vect3f(intersection,
				array_to_vect(cylinder.botcap->coords)));
	normal = subtract_vect3f(intersection, scale_vect3f(t, *cylinder.normal));
	normal = subtract_vect3f(normal, array_to_vect(cylinder.botcap->coords));
	normalize(&normal);
	return (normal);
}

t_vect3f	get_cone_hit_normal(t_vect3f intersection, t_cone cone)
{
	double		dist;
	t_vect3f	a;
	double		d;
	t_vect3f	normal;

	dist = point_distance(intersection, array_to_vect(cone.pinnacle->coords));
	d = dist * cone.dist_term;
	a.x = cone.pinnacle->coords[X] + cone.axis->x * d;
	a.y = cone.pinnacle->coords[Y] + cone.axis->y * d;
	a.z = cone.pinnacle->coords[Z] + cone.axis->z * d;
	normal = subtract_vect3f(intersection, a);
	normalize(&normal);
	return (normal);
}

void	get_plane_uv(t_vect3f p, long long *u, long long *v, double scale[2])
{
	double		uu;
	double		vv;

	uu = p.x / 16;
	vv = p.y / 16;
	*u = ((long long)(uu * scale[0] + 2147483648LL) % (int)scale[0]);
	*v = scale[1] - 1
		- ((long long)(vv * scale[1] + 2147483648LL) % (int)scale[1]);
}

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

void	get_sphere_uv(t_vect3f p, long long *u, long long *v, double scale[2])
{
	double		uu;
	double		vv;

	normalize(&p);
	uu = 1 - (0.5 + (atan2(p.z, p.x) / (6.28318530718)));
	vv = 1 - (0.5 + (asin(p.y) / 3.14159265358979));
	if (uu < 0)
		uu = 0;
	if (uu > 0.9999)
		uu = 0.9999;
	if (vv < 0)
		vv = 0;
	if (vv > 0.9999)
		vv = 0.9999;
	*u = (int)(uu * scale[0]);
	*v = (int)(vv * scale[1]);
}

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

void	get_cone_uv(t_vect3f p, long long *u, long long *v, double scale[2])
{
	double		uu;
	double		vv;

	vv = 1 - ((p.z + scale[2] / 2) / scale[2]);
	normalize(&p);
	uu = 0.5 + (atan2(p.y, p.x) / (6.28318530718));
	if (uu < 0)
		uu = 0;
	if (uu > 0.9999)
		uu = 0.9999;
	if (vv < 0)
		vv = 0;
	if (vv > 0.9999)
		vv = 0.9999;
	*u = (int)(uu * scale[0]);
	*v = (int)(vv * scale[1]);
}

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

void	get_disc_uv(t_vect3f p, long long *u, long long *v, double scale[3])
{
	double		uu;
	double		vv;

	uu = (p.x + scale[2]) / (2 * scale[2]);
	vv = (p.y + scale[2]) / (2 * scale[2]);
	if (uu < 0)
		uu = 0;
	if (uu > 0.9999)
		uu = 0.9999;
	if (vv < 0)
		vv = 0;
	if (vv > 0.9999)
		vv = 0.9999;
	*u = (long long)(uu * scale[0]);
	*v = scale[1] - (long long)(vv * scale[1]);
}

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

void	get_cylinder_uv(t_vect3f p, long long *u, long long *v, double scale[2])
{
	double		uu;
	double		vv;

	vv = 1 - ((p.z + scale[2] / 2) / scale[2]);
	normalize(&p);
	uu = (0.5 + (atan2(p.y, p.x) / (6.28318530718)));
	if (uu < 0)
		uu = 0;
	if (uu > 0.9999)
		uu = 0.9999;
	if (vv < 0)
		vv = 0;
	if (vv > 0.9999)
		vv = 0.9999;
	*u = (int)(uu * scale[0]);
	*v = (int)(vv * scale[1]);
}

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

void	set_ambient_intensity(t_shader *shader,
	int *ambient_rgb, double ambient_ratio)
{
	shader->illumination[R] = ambient_rgb[R] / 255.
		* ambient_ratio * shader->rgb_object[R] / 255.;
	shader->illumination[G] = ambient_rgb[G] / 255.
		* ambient_ratio * shader->rgb_object[G] / 255.;
	shader->illumination[B] = ambient_rgb[B] / 255.
		* ambient_ratio * shader->rgb_object[B] / 255.;
}

t_vect3f	get_new_normal(int x, int y, int z)
{
	t_vect3f	newnorm;

	x -= 128;
	y -= 128;
	z -= 128;
	if (z < 1)
		z = 1;
	newnorm.x = x;
	newnorm.y = y;
	newnorm.z = z;
	normalize(&newnorm);
	return (newnorm);
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
			m.tanq = get_cone_tan_quat(shader->hit_normal, cone, intersection);
			set_cone_normal2(shader, cone, m);
		}
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

void	set_disc_normal2(t_shader *shader, t_disc *disc, t_map m)
{
	rotate_vect(&m.p, disc->q);
	if (disc->is_inversed)
		rotate_vect(&m.p, m.invq);
	m.s[0] = disc->vector_map->vm_main->width;
	m.s[1] = disc->vector_map->vm_main->height;
	m.s[2] = 1 * disc->radius;
	get_disc_uv(m.p, &m.u, &m.v, m.s);
	m.w = m.s[0] * 4;
	m.newnormal = get_new_normal(disc->vector_map->vm_main->pixels[(m.v
				* m.w) + (m.u * 4)],
			disc->vector_map->vm_main->pixels[(m.v * m.w)
			+ (m.u * 4) + 1],
			disc->vector_map->vm_main->pixels[(m.v * m.w)
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
		if (disc->vector_map->vm_main)
		{
			m.p.x = intersection.x - disc->coords[X];
			m.p.y = intersection.y - disc->coords[Y];
			m.p.z = intersection.z - disc->coords[Z];
			set_disc_normal2(shader, disc, m);
		}
	}
}
