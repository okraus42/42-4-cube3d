/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shader_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 17:46:40 by plouda            #+#    #+#             */
/*   Updated: 2024/02/27 15:10:27 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

double	point_distance(t_vect3f p1, t_vect3f p2)
{
	double	dist;

	dist = sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2) + pow(p1.z - p2.z, 2));
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

uint32_t	ft_max_clr(uint32_t a, uint32_t b)
{
	uint32_t	c;

	c = MAX(a & 0xFF000000, b & 0xFF000000) | MAX(a & 0xFF0000, b & 0xFF0000)
		| MAX(a & 0xFF00, b & 0xFF00) | 0xFF;
	return (c);
}

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

t_vect3f	get_cylinder_hit_normal(t_rayfinder *rf, t_ray ray, t_vect3f intersection, t_cylinder cylinder)
{

	double t;
	t_vect3f	normal;

	(void)rf;
	(void)ray;
	t = dot_product(*cylinder.normal, subtract_vect3f(intersection, array_to_vect(cylinder.botcap->coords)));
	normal = subtract_vect3f(intersection, scale_vect3f(t, *cylinder.normal));
	normal = subtract_vect3f(normal, array_to_vect(cylinder.botcap->coords));
	normalize(&normal);
	return (normal);
}

t_vect3f	get_cone_hit_normal(t_vect3f intersection, t_cone cone)
{
	double	dist;
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

// void	set_ambient_intensity(t_shader *shader, int *ambient_rgb, double ambient_ratio, int *object_rgb)
// {
// 	shader->illumination[R] = ambient_rgb[R] / 255. * ambient_ratio * object_rgb[R] / 255.;
// 	shader->illumination[G] = ambient_rgb[G] / 255. * ambient_ratio * object_rgb[G] / 255.;
// 	shader->illumination[B] = ambient_rgb[B] / 255. * ambient_ratio * object_rgb[B] / 255.;
// }

void	get_plane_uv(t_vect3f p, long long *u, long long *v, int scale[2])
{
	double		uu;
	double		vv;

	uu = p.x / 16;	//to make bigger checkerboard
	vv = p.y / 16;
	// rewrite to get values 0 - (scale[0] - 1)
	// if (uu < 0)
	// 	uu = 0;
	// if (uu > 0.9999)
	// 	uu = 0.9999;
	// if (vv < 0)
	// 	vv = 0;
	// if (vv > 0.9999)
	// 	vv = 0.9999;
	*u = ((long long)(uu * scale[0] + 2147483648LL) % scale[0]); //offset to not get weird stuff around 0
	*v = (scale[1] - 1) - ((long long)(vv * scale[1] + 2147483648LL) % scale[1]);
	// to work well on negative coordinates
}

void	set_plane_rgb(t_shader *shader, t_plane *plane, t_vect3f intersection)
{
	long long	u;
	long long	v;
	int			s[2];
	int			w;
	t_vect3f	p;

	if (plane->texture)
	{
		shader->rgb_object[R] = 255;
		shader->rgb_object[G] = 0;
		shader->rgb_object[B] = 255;
		if (plane->texture->tx_main)
		{
			p.x = intersection.x - plane->coords[X];
			p.y = intersection.y - plane->coords[Y];
			p.z = intersection.z - plane->coords[Z];
			rotate_vect(&p, plane->q);
			s[0] = plane->texture->tx_main->width;
			s[1] = plane->texture->tx_main->height;
			get_plane_uv(p, &u, &v, s);
			w = s[0] * 4;
			shader->rgb_object[R] = plane->texture->tx_main->pixels[(v * w) + (u * 4)];
			shader->rgb_object[G] = plane->texture->tx_main->pixels[(v * w) + (u * 4) + 1];
			shader->rgb_object[B] = plane->texture->tx_main->pixels[(v * w) + (u * 4) + 2];
		}
	}
	else if (plane->checkerboard)
	{
		p.x = intersection.x - plane->coords[X];
		p.y = intersection.y - plane->coords[Y];
		p.z = intersection.z - plane->coords[Z];
		s[0] = (int)plane->checkerboard->magnitude;
		s[1] = (int)plane->checkerboard->magnitude;
		rotate_vect(&p, plane->q);
		get_plane_uv(p, &u, &v, s);
		if (((u & 1) && (v & 1)) || (!(u & 1) && !(v & 1)))
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
		if (!u)
		{
			if (!v)
			{
				shader->rgb_object[R] = 0;
				shader->rgb_object[G] = 255;
				shader->rgb_object[B] = 255;
			}
			else if (v == 1)
			{
				shader->rgb_object[R] = 255;
				shader->rgb_object[G] = 0;
				shader->rgb_object[B] = 255;
			}
			else if (v == 2)
			{
				shader->rgb_object[R] = 255;
				shader->rgb_object[G] = 255;
				shader->rgb_object[B] = 0;
			}
		}
		if (u == 1 && !v)
		{
			shader->rgb_object[R] = 255;
			shader->rgb_object[G] = 0;
			shader->rgb_object[B] = 255;
		}
		if (u == 2 && !v)
		{
			shader->rgb_object[R] = 255;
			shader->rgb_object[G] = 255;
			shader->rgb_object[B] = 0;
		}
	}
	else
	{
		shader->rgb_object[R] = plane->rgb[R];
		shader->rgb_object[G] = plane->rgb[G];
		shader->rgb_object[B] = plane->rgb[B];
	}
}

void	get_sphere_uv(t_vect3f p, int *u, int *v, int scale[2])
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

void	set_sphere_rgb(t_shader *shader, t_sphere *sphere, t_vect3f intersection)
{
	int			u;
	int			v;
	int			w;
	int			s[2];
	t_vect3f	p;

	if (sphere->texture)
	{
		shader->rgb_object[R] = 255;
		shader->rgb_object[G] = 0;
		shader->rgb_object[B] = 255;
		if (sphere->texture->tx_main)
		{
			p.x = intersection.x - sphere->coords[X];
			p.y = intersection.y - sphere->coords[Y];
			p.z = intersection.z - sphere->coords[Z];
			rotate_vect(&p, sphere->q);
			s[0] = sphere->texture->tx_main->width;
			s[1] = sphere->texture->tx_main->height;
			get_sphere_uv(p, &u, &v, s);
			w = s[0] * 4;
			shader->rgb_object[R] = sphere->texture->tx_main->pixels[(v * w) + (u * 4)];
			shader->rgb_object[G] = sphere->texture->tx_main->pixels[(v * w) + (u * 4) + 1];
			shader->rgb_object[B] = sphere->texture->tx_main->pixels[(v * w) + (u * 4) + 2];
		}
	}
	else if (sphere->checkerboard)
	{
		p.x = intersection.x - sphere->coords[X];
		p.y = intersection.y - sphere->coords[Y];
		p.z = intersection.z - sphere->coords[Z];
		//printf("Q %f %f %f %f\n", sphere->q.q0, sphere->q.q1, sphere->q.q2, sphere->q.q3);
		//printf("N %f %f %f \n", sphere->normal->x, sphere->normal->y, sphere->normal->z);
		//printf("P1 %f %f %f \n", p.x, p.y, p.z);
		rotate_vect(&p, sphere->q);
		//printf("P2 %f %f %f \n", p.x, p.y, p.z);
		s[0] = (int)sphere->checkerboard->magnitude;
		s[1] = (int)sphere->checkerboard->magnitude;
		get_sphere_uv(p, &u, &v, s);
		if (((u & 1) && (v & 1)) || (!(u & 1) && !(v & 1)))
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
		if (!u)
		{
			if (!v)
			{
				shader->rgb_object[R] = 0;
				shader->rgb_object[G] = 255;
				shader->rgb_object[B] = 255;
			}
			else if (v == 1)
			{
				shader->rgb_object[R] = 255;
				shader->rgb_object[G] = 0;
				shader->rgb_object[B] = 255;
			}
			else if (v == 2)
			{
				shader->rgb_object[R] = 255;
				shader->rgb_object[G] = 255;
				shader->rgb_object[B] = 0;
			}
		}
		if (u == 1 && !v)
		{
			shader->rgb_object[R] = 255;
			shader->rgb_object[G] = 0;
			shader->rgb_object[B] = 255;
		}
		if (u == 2 && !v)
		{
			shader->rgb_object[R] = 0;
			shader->rgb_object[G] = 0;
			shader->rgb_object[B] = 0;
		}
	}
	else
	{
		shader->rgb_object[R] = sphere->rgb[R];
		shader->rgb_object[G] = sphere->rgb[G];
		shader->rgb_object[B] = sphere->rgb[B];
	}
}

void	get_cone_uv(t_vect3f p, int *u, int *v, double scale[2])
{
	double		uu;
	double		vv;

	vv = 1 - ((p.z + scale[2] / 2) / scale[2]);
	//printf ("%f %f %f\n", p.z, vv, scale[2]);
	normalize(&p);
	uu = 0.5 + (atan2(p.y, p.x) / (6.28318530718));	
	//printf ("%f %f\n", uu, vv);
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

void	set_cone_rgb(t_shader *shader, t_cone *cone, t_vect3f intersection)
{
	int			u;
	int			v;
	int			w;
	double		s[3];
	t_vect3f	p;

	if (cone->texture)
	{
		shader->rgb_object[R] = 255;
		shader->rgb_object[G] = 0;
		shader->rgb_object[B] = 255;
		if (cone->texture->tx_main)
		{
			p.x = intersection.x - cone->coords[X];
			p.y = intersection.y - cone->coords[Y];
			p.z = intersection.z - cone->coords[Z];
			rotate_vect(&p, cone->q);
			s[0] = cone->texture->tx_main->width;
			s[1] = cone->texture->tx_main->height;
			s[2] = cone->height;
			get_cone_uv(p, &u, &v, s);
			w = s[0] * 4;
			shader->rgb_object[R] = cone->texture->tx_main->pixels[(v * w) + (u * 4)];
			shader->rgb_object[G] = cone->texture->tx_main->pixels[(v * w) + (u * 4) + 1];
			shader->rgb_object[B] = cone->texture->tx_main->pixels[(v * w) + (u * 4) + 2];
		}
	}
	else if (cone->checkerboard)
	{
		p.x = intersection.x - cone->coords[X];
		p.y = intersection.y - cone->coords[Y];
		p.z = intersection.z - cone->coords[Z];
		s[0] = (int)cone->checkerboard->magnitude;
		s[1] = (int)(0.5 + cone->checkerboard->magnitude * sqrt(cone->height * cone->height + cone->radius * 2 * cone->radius * 2 / 4) / (M_PI * cone->radius * 2));
		s[2] = cone->height;
		//printf("%f %f %f\n",intersection.z, p.z, cone->coords[Z]);
		rotate_vect(&p, cone->q);
		//printf("q0 %f q1 %f q2 %f q3 %f\n", cone->q.q0, cone->q.q1, cone->q.q2, cone->q.q3);
		//printf("px %f py %f pz %f\n", p.x, p.y, p.z);
		get_cone_uv(p, &u, &v, s);
		if (((u & 1) && (v & 1)) || (!(u & 1) && !(v & 1)))
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
		if (!u)
		{
			if (!v)
			{
				shader->rgb_object[R] = 0;
				shader->rgb_object[G] = 255;
				shader->rgb_object[B] = 255;
			}
			else if (v == 1)
			{
				shader->rgb_object[R] = 255;
				shader->rgb_object[G] = 0;
				shader->rgb_object[B] = 255;
			}
			else if (v == 2)
			{
				shader->rgb_object[R] = 255;
				shader->rgb_object[G] = 255;
				shader->rgb_object[B] = 0;
			}
		}
		if (u == 1 && !v)
		{
			shader->rgb_object[R] = 255;
			shader->rgb_object[G] = 0;
			shader->rgb_object[B] = 255;
		}
		if (u == 2 && !v)
		{
			shader->rgb_object[R] = 255;
			shader->rgb_object[G] = 255;
			shader->rgb_object[B] = 0;
		}
		// if (intersection.z < 0)
		// {
		// 	shader->rgb_object[R] = 255;
		// 	shader->rgb_object[G] = 0;
		// 	shader->rgb_object[B] = 255;
		// }
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

	uu = (p.x + scale[2]) / (2 * scale[2]);	//to make bigger checkerboard
	vv = (p.y + scale[2]) / (2 * scale[2]);
	if (uu < 0)
		uu = 0;
	if (uu > 0.9999)
		uu = 0.9999;
	if (vv < 0)
		vv = 0;
	if (vv > 0.9999)
		vv = 0.9999;
	*u = (long long)(uu * scale[0]); //offset to not get weird stuff around 0
	*v = scale[1] - (long long)(vv * scale[1]);
	// to work well on negative coordinates
}

void	set_disc_rgb(t_shader *shader, t_disc *disc, t_vect3f intersection)
{
	long long	u;
	long long	v;
	int			w;
	double		s[3];
	t_vect3f	p;
	t_quat		inv;

	inv.q0 = 0;
	inv.q1 = 0;
	inv.q2 = 1;
	inv.q3 = 0;
	if (disc->texture)
	{
		//printf("disc has texture\n");
		shader->rgb_object[R] = 255;
		shader->rgb_object[G] = 0;
		shader->rgb_object[B] = 255;
		if (disc->tx_disc)
		{
			p.x = intersection.x - disc->coords[X];
			p.y = intersection.y - disc->coords[Y];
			p.z = intersection.z - disc->coords[Z];
			rotate_vect(&p, disc->q);
			if (disc->is_inversed)
				rotate_vect(&p, inv);
			s[0] = disc->tx_disc->width;
			s[1] = disc->tx_disc->height;
			s[2] = 1 * disc->radius;
			get_disc_uv(p, &u, &v, s);
			w = s[0] * 4;
			shader->rgb_object[R] = disc->tx_disc->pixels[(v * w) + (u * 4)];
			shader->rgb_object[G] = disc->tx_disc->pixels[(v * w) + (u * 4) + 1];
			shader->rgb_object[B] = disc->tx_disc->pixels[(v * w) + (u * 4) + 2];
		}
	}
	else if (disc->checkerboard)
	{
		p.x = intersection.x - disc->coords[X];
		p.y = intersection.y - disc->coords[Y];
		p.z = intersection.z - disc->coords[Z];
		s[0] = (int)disc->checkerboard->magnitude;
		s[1] = (int)disc->checkerboard->magnitude;
		s[2] = 1 * disc->radius;
		rotate_vect(&p, disc->q);
		if (disc->is_inversed)
				rotate_vect(&p, inv);		
		get_disc_uv(p, &u, &v, s);
		if (((u & 1) && (v & 1)) || (!(u & 1) && !(v & 1)))
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
	else
	{
		shader->rgb_object[R] = disc->rgb[R];
		shader->rgb_object[G] = disc->rgb[G];
		shader->rgb_object[B] = disc->rgb[B];
	}
}

void	get_cylinder_uv(t_vect3f p, int *u, int *v, double scale[2])
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

void	set_cylinder_rgb(t_shader *shader, t_cylinder *cylinder, t_vect3f intersection)
{
	int			u;
	int			v;
	int			w;
	double		s[3];
	t_vect3f	p;

	if (cylinder->texture)
	{
		shader->rgb_object[R] = 255;
		shader->rgb_object[G] = 0;
		shader->rgb_object[B] = 255;
		if (cylinder->texture->tx_main)
		{
			p.x = intersection.x - cylinder->coords[X];
			p.y = intersection.y - cylinder->coords[Y];
			p.z = intersection.z - cylinder->coords[Z];
			rotate_vect(&p, cylinder->q);
			s[0] = cylinder->texture->tx_main->width;
			s[1] = cylinder->texture->tx_main->height;
			s[2] = cylinder->height;
			get_cylinder_uv(p, &u, &v, s);
			w = s[0] * 4;
			shader->rgb_object[R] = cylinder->texture->tx_main->pixels[(v * w) + (u * 4)];
			shader->rgb_object[G] = cylinder->texture->tx_main->pixels[(v * w) + (u * 4) + 1];
			shader->rgb_object[B] = cylinder->texture->tx_main->pixels[(v * w) + (u * 4) + 2];
		}
	}
	else if (cylinder->checkerboard)
	{
		p.x = intersection.x - cylinder->coords[X];
		p.y = intersection.y - cylinder->coords[Y];
		p.z = intersection.z - cylinder->coords[Z];
		s[0] = (int)cylinder->checkerboard->magnitude;
		s[1] = (int)(0.5 + cylinder->checkerboard->magnitude * cylinder->height / (M_PI * cylinder->radius * 2));
		s[2] = cylinder->height;
		//printf("%f %f %f\n",intersection.z, p.z, cylinder->coords[Z]);
		rotate_vect(&p, cylinder->q);
		get_cylinder_uv(p, &u, &v, s);
		if (((u & 1) && (v & 1)) || (!(u & 1) && !(v & 1)))
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
		if (!u)
		{
			if (!v)
			{
				shader->rgb_object[R] = 0;
				shader->rgb_object[G] = 255;
				shader->rgb_object[B] = 255;
			}
			else if (v == 1)
			{
				shader->rgb_object[R] = 255;
				shader->rgb_object[G] = 0;
				shader->rgb_object[B] = 255;
			}
			else if (v == 2)
			{
				shader->rgb_object[R] = 255;
				shader->rgb_object[G] = 255;
				shader->rgb_object[B] = 0;
			}
		}
		if (u == 1 && !v)
		{
			shader->rgb_object[R] = 255;
			shader->rgb_object[G] = 0;
			shader->rgb_object[B] = 255;
		}
		if (u == 2 && !v)
		{
			shader->rgb_object[R] = 255;
			shader->rgb_object[G] = 255;
			shader->rgb_object[B] = 0;
		}
		// if (intersection.z < 0)
		// {
		// 	shader->rgb_object[R] = 255;
		// 	shader->rgb_object[G] = 0;
		// 	shader->rgb_object[B] = 255;
		// }
	}
	else
	{
		shader->rgb_object[R] = cylinder->rgb[R];
		shader->rgb_object[G] = cylinder->rgb[G];
		shader->rgb_object[B] = cylinder->rgb[B];
	}
}

void	set_ambient_intensity(t_shader *shader, int *ambient_rgb, double ambient_ratio)
{
	shader->illumination[R] = ambient_rgb[R] / 255. * ambient_ratio * shader->rgb_object[R] / 255.;
	shader->illumination[G] = ambient_rgb[G] / 255. * ambient_ratio * shader->rgb_object[G] / 255.;
	shader->illumination[B] = ambient_rgb[B] / 255. * ambient_ratio * shader->rgb_object[B] / 255.;
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
	newnorm.z = z; //shou be from 0 to -1, but I think it can work if we assume normal nomal to go to 1 and not -1
	normalize(&newnorm);
	//printf("%f %f %f\n", newnorm.x, newnorm.y, newnorm.z);
	return (newnorm);
}

void	set_sphere_normal(t_shader *shader, t_sphere *sphere, t_vect3f intersection)
{
	int			u;
	int			v;
	int			w;
	int			s[2];
	t_vect3f	p;
	t_vect3f	newnormal;
//	t_vect3f	fictnormal;
//	t_quat		rot;
	t_quat		tanq;
	t_quat		invt;
	t_quat		invq;

	if (sphere->vector_map)
	{

		if (sphere->vector_map->vm_main)
		{
			p.x = intersection.x - sphere->coords[X];
			p.y = intersection.y - sphere->coords[Y];
			p.z = intersection.z - sphere->coords[Z];
			invq = get_inverse_quat(sphere->q);
			rotate_vect(&p, sphere->q);
			//printf("\nnormal0 %f %f %f\n", shader->hit_normal.x, shader->hit_normal.y, shader->hit_normal.z);
			rotate_vect(&(shader->hit_normal), sphere->q);
			//printf("normal1 %f %f %f\n", shader->hit_normal.x, shader->hit_normal.y, shader->hit_normal.z);
			tanq = get_sphere_tan_quat(shader->hit_normal);
			rotate_vect(&(shader->hit_normal), tanq);
			//printf("normal2 %f %f %f\n", shader->hit_normal.x, shader->hit_normal.y, shader->hit_normal.z);
			s[0] = sphere->vector_map->vm_main->width;
			s[1] = sphere->vector_map->vm_main->height;
			get_sphere_uv(p, &u, &v, s);
			w = s[0] * 4;
			newnormal = get_new_normal(sphere->vector_map->vm_main->pixels[(v * w) + (u * 4)],
				sphere->vector_map->vm_main->pixels[(v * w) + (u * 4) + 1],
				sphere->vector_map->vm_main->pixels[(v * w) + (u * 4) + 2]);
			invt = get_inverse_quat(tanq);
			//printf("newnormal0 %f %f %f\n", newnormal.x, newnormal.y, newnormal.z);
			rotate_vect(&newnormal, invt);
			//printf("newnormal1 %f %f %f\n", newnormal.x, newnormal.y, newnormal.z);
			rotate_vect(&newnormal, invq);
			//printf("newnormal2 %f %f %f\n", newnormal.x, newnormal.y, newnormal.z);
			shader->hit_normal.x = newnormal.x;
			shader->hit_normal.y = newnormal.y;
			shader->hit_normal.z = newnormal.z;
		}
	}
	// if (sphere->vector_map)
	// {

	// 	if (sphere->vector_map->vm_main)
	// 	{
	// 		p.x = intersection.x - sphere->coords[X];
	// 		p.y = intersection.y - sphere->coords[Y];
	// 		p.z = intersection.z - sphere->coords[Z];
	// 		rotate_vect(&p, sphere->q);
	// 		s[0] = sphere->vector_map->vm_main->width;
	// 		s[1] = sphere->vector_map->vm_main->height;
	// 		get_sphere_uv(p, &u, &v, s);
	// 		w = s[0] * 4;
	// 		newnormal = get_new_normal(sphere->vector_map->vm_main->pixels[(v * w) + (u * 4)],
	// 			sphere->vector_map->vm_main->pixels[(v * w) + (u * 4) + 1],
	// 			sphere->vector_map->vm_main->pixels[(v * w) + (u * 4) + 2]);
	// 		fictnormal.x = 0;
	// 		fictnormal.y = 0;
	// 		fictnormal.z = 1;
	// 		rot = get_rotvect_quat(fictnormal, newnormal);
	// 		rotate_vect(&(shader->hit_normal), rot);
	// 	}
	// }
}

void	set_cylinder_normal(t_shader *shader, t_cylinder *cylinder, t_vect3f intersection)
{
	int			u;
	int			v;
	int			w;
	double		s[3];
	t_vect3f	p;
	t_vect3f	newnormal;
//	t_vect3f	fictnormal;
	t_quat		tanq;
	t_quat		invt;
	t_quat		invq;
//	t_quat		rot;

	if (cylinder->vector_map)
	{

		if (cylinder->vector_map->vm_main)
		{
			p.x = intersection.x - cylinder->coords[X];
			p.y = intersection.y - cylinder->coords[Y];
			p.z = intersection.z - cylinder->coords[Z];
			invq = get_inverse_quat(cylinder->q);
			rotate_vect(&p, cylinder->q);
			//printf("normal0 %f %f %f\n", shader->hit_normal.x, shader->hit_normal.y, shader->hit_normal.z);
			rotate_vect(&(shader->hit_normal), cylinder->q);
			//printf("normal1 %f %f %f\n", shader->hit_normal.x, shader->hit_normal.y, shader->hit_normal.z);
			tanq = get_cylinder_tan_quat(shader->hit_normal);
			// fictnormal.x = 0;
			// fictnormal.y = 0;
			// fictnormal.z = -1;
			//rot = get_rotvect_quat(fictnormal, shader->hit_normal);
			rotate_vect(&(shader->hit_normal), tanq);
			//printf("normal2 %f %f %f\n", shader->hit_normal.x, shader->hit_normal.y, shader->hit_normal.z);
			if (shader->hit_normal.z < 0.999)
				printf("LOOKHERE!!!\n");
			s[0] = cylinder->vector_map->vm_main->width;
			s[1] = cylinder->vector_map->vm_main->height;
			s[2] = cylinder->height;
			get_cylinder_uv(p, &u, &v, s);
			w = s[0] * 4;
			newnormal = get_new_normal(cylinder->vector_map->vm_main->pixels[(v * w) + (u * 4)],
				cylinder->vector_map->vm_main->pixels[(v * w) + (u * 4) + 1],
				cylinder->vector_map->vm_main->pixels[(v * w) + (u * 4) + 2]);
			//printf("newnormal0 %f %f %f\n", newnormal.x, newnormal.y, newnormal.z);
			invt = get_inverse_quat(tanq);
			rotate_vect(&newnormal, invt);
			//printf("newnormal1 %f %f %f\n", newnormal.x, newnormal.y, newnormal.z);
			rotate_vect(&newnormal, invq);
			//printf("newnormal2 %f %f %f\n", newnormal.x, newnormal.y, newnormal.z);
			shader->hit_normal.x = newnormal.x;
			shader->hit_normal.y = newnormal.y;
			shader->hit_normal.z = newnormal.z;
			// shader->hit_normal.x = 0.945;
			// shader->hit_normal.y = 0.312;
			// shader->hit_normal.z = -0.07;
		}
	}
}

void	set_cone_normal(t_shader *shader, t_cone *cone, t_vect3f intersection)
{
	int			u;
	int			v;
	int			w;
	double		s[3];
	t_vect3f	p;
	t_vect3f	newnormal;
//	t_vect3f	fictnormal;
	t_quat		tanq;
	t_quat		invt;
	t_quat		invq;
//	t_quat		rot;

	if (cone->vector_map)
	{

		if (cone->vector_map->vm_main)
		{
			p.x = intersection.x - cone->coords[X];
			p.y = intersection.y - cone->coords[Y];
			p.z = intersection.z - cone->coords[Z];
			invq = get_inverse_quat(cone->q);
			rotate_vect(&p, cone->q);
			//printf("normal0 %f %f %f\n", shader->hit_normal.x, shader->hit_normal.y, shader->hit_normal.z);
			rotate_vect(&(shader->hit_normal), cone->q);
			//printf("normal1 %f %f %f\n", shader->hit_normal.x, shader->hit_normal.y, shader->hit_normal.z);
			tanq = get_cone_tan_quat(shader->hit_normal, cone, intersection);
			// fictnormal.x = 0;
			// fictnormal.y = 0;
			// fictnormal.z = -1;
			//rot = get_rotvect_quat(fictnormal, shader->hit_normal);
			rotate_vect(&(shader->hit_normal), tanq);
			//printf("normal2 %f %f %f\n", shader->hit_normal.x, shader->hit_normal.y, shader->hit_normal.z);
			// if (shader->hit_normal.z < 0.999)
			// 	printf("LOOKHERE!!!\n");
			s[0] = cone->vector_map->vm_main->width;
			s[1] = cone->vector_map->vm_main->height;
			s[2] = cone->height;
			get_cone_uv(p, &u, &v, s);
			w = s[0] * 4;
			newnormal = get_new_normal(cone->vector_map->vm_main->pixels[(v * w) + (u * 4)],
				cone->vector_map->vm_main->pixels[(v * w) + (u * 4) + 1],
				cone->vector_map->vm_main->pixels[(v * w) + (u * 4) + 2]);
			//printf("newnormal0 %f %f %f\n", newnormal.x, newnormal.y, newnormal.z);
			invt = get_inverse_quat(tanq);
			rotate_vect(&newnormal, invt);
			//printf("newnormal1 %f %f %f\n", newnormal.x, newnormal.y, newnormal.z);
			rotate_vect(&newnormal, invq);
			//printf("newnormal2 %f %f %f\n", newnormal.x, newnormal.y, newnormal.z);
			shader->hit_normal.x = newnormal.x;
			shader->hit_normal.y = newnormal.y;
			shader->hit_normal.z = newnormal.z;
			// shader->hit_normal.x = 0.945;
			// shader->hit_normal.y = 0.312;
			// shader->hit_normal.z = -0.07;
		}
	}
}

// void	set_cone_normal(t_shader *shader, t_cone *cone, t_vect3f intersection)
// {
// 	int			u;
// 	int			v;
// 	int			w;
// 	double		s[3];
// 	t_vect3f	p;
// 	t_vect3f	newnormal;
// 	t_quat		tanq;
// 	t_quat		inv;

// 	if (cone->vector_map)
// 	{

// 		if (cone->vector_map->vm_main)
// 		{
// 			p.x = intersection.x - cone->coords[X];
// 			p.y = intersection.y - cone->coords[Y];
// 			p.z = intersection.z - cone->coords[Z];
// 			rotate_vect(&p, cone->q);
// 			rotate_vect(&(shader->hit_normal), cone->q);
// 			tanq = get_cone_tan_quat(shader->hit_normal);
// 			s[0] = cone->vector_map->vm_main->width;
// 			s[1] = cone->vector_map->vm_main->height;
// 			s[2] = cone->height;
// 			get_cone_uv(p, &u, &v, s);
// 			w = s[0] * 4;
// 			newnormal = get_new_normal(cone->vector_map->vm_main->pixels[(v * w) + (u * 4)],
// 				cone->vector_map->vm_main->pixels[(v * w) + (u * 4) + 1],
// 				cone->vector_map->vm_main->pixels[(v * w) + (u * 4) + 2]);
// 			inv = get_inverse_quat(tanq);
// 			rotate_vect(&newnormal, inv);
// 			inv = get_inverse_quat(cone->q);
// 			rotate_vect(&newnormal, inv);
// 			shader->hit_normal.x = newnormal.x;
// 			shader->hit_normal.y = newnormal.y;
// 			shader->hit_normal.z = newnormal.z;
// 		}
// 	}
// }

void	set_plane_normal(t_shader *shader, t_plane *plane, t_vect3f intersection)
{
	long long	u;
	long long	v;
	int			w;
	int			s[2];
	t_vect3f	p;
	t_vect3f	newnormal;
	t_quat		inv;

	if (plane->vector_map)
	{

		if (plane->vector_map->vm_main)
		{
			p.x = intersection.x - plane->coords[X];
			p.y = intersection.y - plane->coords[Y];
			p.z = intersection.z - plane->coords[Z];
			rotate_vect(&p, plane->q);
			s[0] = plane->vector_map->vm_main->width;
			s[1] = plane->vector_map->vm_main->height;
			get_plane_uv(p, &u, &v, s);
			w = s[0] * 4;
			newnormal = get_new_normal(plane->vector_map->vm_main->pixels[(v * w) + (u * 4)],
				plane->vector_map->vm_main->pixels[(v * w) + (u * 4) + 1],
				plane->vector_map->vm_main->pixels[(v * w) + (u * 4) + 2]);
			inv = get_inverse_quat(plane->q);
			rotate_vect(&newnormal, inv);
			shader->hit_normal.x = newnormal.x;
			shader->hit_normal.y = newnormal.y;
			shader->hit_normal.z = newnormal.z;
		}
	}
}

void	set_disc_normal(t_shader *shader, t_disc *disc, t_vect3f intersection)
{
	long long	u;
	long long	v;
	int			w;
	double		s[3];
	t_vect3f	p;
	t_vect3f	newnormal;
	//t_vect3f	fictnormal;
	//t_quat		rot;
	t_quat		inv;


	inv.q0 = 0;
	inv.q1 = 0;
	inv.q2 = 1;
	inv.q3 = 0;
	if (disc->vector_map)
	{
		if (disc->vector_map->vm_main)
		{
			p.x = intersection.x - disc->coords[X];
			p.y = intersection.y - disc->coords[Y];
			p.z = intersection.z - disc->coords[Z];
			rotate_vect(&p, disc->q);
			if (disc->is_inversed)
				rotate_vect(&p, inv);
			s[0] = disc->vector_map->vm_main->width;
			s[1] = disc->vector_map->vm_main->height;
			s[2] = 1 * disc->radius;
			get_disc_uv(p, &u, &v, s);
			w = s[0] * 4;
			newnormal = get_new_normal(disc->vector_map->vm_main->pixels[(v * w) + (u * 4)],
				disc->vector_map->vm_main->pixels[(v * w) + (u * 4) + 1],
				disc->vector_map->vm_main->pixels[(v * w) + (u * 4) + 2]);
			if (disc->is_inversed)
				rotate_vect(&newnormal, inv);
			inv = get_inverse_quat(disc->q);
			rotate_vect(&newnormal, inv);
			// fictnormal.x = 0;
			// fictnormal.y = 0;
			// fictnormal.z = 1;
			// rot = get_rotvect_quat(fictnormal, newnormal);
			//printf("%f %f %f %f\n", rot.q0, rot.q1, rot.q2, rot.q3);
			//rotate_vect(&(shader->hit_normal), rot);
			shader->hit_normal.x = newnormal.x;
			shader->hit_normal.y = newnormal.y;
			shader->hit_normal.z = newnormal.z;
		}
	}
}