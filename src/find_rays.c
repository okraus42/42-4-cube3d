/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_rays.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 09:47:21 by plouda            #+#    #+#             */
/*   Updated: 2023/12/04 17:49:03 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minirt.h"

double	deg(double rad)
{
	double	deg;

	deg = rad * 180 / M_PI;
	return (deg);
}

double	rad(double deg)
{
	double	rad;

	rad = deg * M_PI / 180;
	return (rad);
}


void	ft_swapf(double *a, double *b)
{
	double temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

double	absf(double n)
{
	if (n < 0)
		return (-n);
	else
		return (n);
}

void	normalize(t_ray *ray, double px, double py, double pz)
{
	double	magnitude;

	magnitude = sqrt(pow(px, 2) + pow(py, 2) + pow(pz, 2));
	ray->direction.x = px / magnitude;
	ray->direction.y = py / magnitude;
	ray->direction.z = pz / magnitude;
}

double	dot_product(t_vect3f vect1, t_vect3f vect2)
{
	double	res;

	res = vect1.x * vect2.x + vect1.y * vect2.y + vect1.z * vect2.z;
	return (res);
}

t_vect3f	subtract_center(t_vect3f vect1, double *coords)
{
	t_vect3f	res;

	res.x = vect1.x - coords[X];
	res.y = vect1.y - coords[Y];
	res.z = vect1.z - coords[Z];
	return (res);
}

t_vect3f	subtract_center_abs(t_vect3f vect1, double *coords)
{
	t_vect3f	res;

	res.x = absf(vect1.x - coords[X]);
	res.y = absf(vect1.y - coords[Y]);
	res.z = absf(vect1.z - coords[Z]);
	return (res);
}

double	*test_sphere(t_ray *ray, t_sphere *sphere)
{
	double	*res;
	double	a;
	double	b;
	double	c;
	double	discr;
	double	q;
	t_vect3f	centered;
	t_vect3f	centered_abs;

	res = malloc(sizeof(int) * 2);
	centered = subtract_center(ray->origin, sphere->coords);
	centered_abs = subtract_center_abs(ray->origin, sphere->coords);
	a = dot_product(ray->direction, ray->direction);
	b = 2 * dot_product(ray->direction, centered);
	c = dot_product(centered_abs, centered_abs) - pow(sphere->diameter / 2, 2);
	discr = pow(b, 2) - 4 * a * c;
	//printf("Discr: %.5f\n", discr);
	if (discr < 0)
		return (NULL);
	else if (discr == 0)
	{
		res[0] = - 0.5 * b / a;
		res[1] = res[0];
		return (res);
	}
	else
	{
		if (b > 0)
			q = -0.5 * (b + sqrt(discr));
		else
			q = -0.5 * (b - sqrt(discr));
		res[0] = q / a;
		res[1] = c / q;
		//printf("Intersect: %.5f, %.5f\n", res[0], res[1]);
		if (res[0] > res[1])
			ft_swapf(&res[0], &res[1]);
		if (res[0] < 0)
		{
			res[0] = res[1];
			if (res[0] < 0)
				return (NULL);
		}
		return (res);
	}
	
}

void	find_rays(t_master *master)
{
	int		x;
	int		y;
	double	ratio;
	double	px;
	double	py;
	double	pz;
	int		fov;
	double	*inter;
	//double	t;
	t_ray	**rays;

	rays = malloc(sizeof(t_ray *) * (WIDTH));
	x = 0;
	while (x < WIDTH)
	{
		rays[x] = malloc(sizeof(t_ray) * (HEIGHT));
		x++;
	}

	x = 0;
	y = 0;
	fov = master->rt->camera->fov;
	ratio = (double)WIDTH / (double)HEIGHT; // assuming WIDTH > HEIGHT
	while (x < WIDTH)
	{
		y = 0;
		while (y < HEIGHT)
		{
			mlx_put_pixel(master->img, x, y, 0x000000FF);
			px = (2 * ((x + 0.5) / WIDTH) - 1) * ratio * tan(rad(fov * 0.5));
			py = (1 - 2 * ((y + 0.5) / HEIGHT)) * tan(rad(fov * 0.5));
			pz = -1;
			// some camera stuff goes here
			rays[x][y].origin.x = 0;
			rays[x][y].origin.y = 0;
			rays[x][y].origin.z = 0;
			normalize(&rays[x][y], px, py, pz);
			inter = test_sphere(&rays[x][y], master->rt->spheres[0]);
			if (inter != NULL)
			{
				mlx_put_pixel(master->img, x, y, 0xFFFFFFFF);
			}
			free(inter);
			y++;
		}
		x++;
	}
	printf("Ray dir of 0,0: %.5f,%.5f,%.5f\n", rays[0][0].direction.x, rays[0][0].direction.y, rays[0][0].direction.z);
	printf("Ray dir of 4,2: %.5f,%.5f,%.5f\n", rays[4][2].direction.x, rays[4][2].direction.y, rays[4][2].direction.z);
	printf("Ray dir of 640,360: %.5f,%.5f,%.5f\n", rays[640][360].direction.x, rays[640][360].direction.y, rays[640][360].direction.z);
	//printf("Ray dir of 1279,719: %.5f,%.5f,%.5f\n", rays[1279][719].direction.x, rays[1279][719].direction.y, rays[1279][719].direction.z);
}
