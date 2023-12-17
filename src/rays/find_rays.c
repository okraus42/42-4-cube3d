/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_rays.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 09:47:21 by plouda            #+#    #+#             */
/*   Updated: 2023/12/17 17:56:08 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	get_nearest_object(int flag, void *object_ptr, uint32_t *clr)
{
	t_sphere	*sphere;
	t_plane		*plane;
	t_cylinder	*cylinder;
	t_disc		*disc;

	sphere = NULL;
	plane = NULL;
	cylinder = NULL;
	disc = NULL;
	if (flag == SPHERE)
	{
		sphere = (t_sphere *)object_ptr;
		*clr = (sphere->rgb[0] << 24 | sphere->rgb[1] << 16 | sphere->rgb[2] << 8 | 0xFF);
	}
	else if (flag == PLANE)
	{
		plane = (t_plane *)object_ptr;
		*clr = (plane->rgb[0] << 24 | plane->rgb[1] << 16 | plane->rgb[2] << 8 | 0xFF);
	}
	else if (flag == CYLINDER)
	{
		cylinder = (t_cylinder *)object_ptr;
		*clr = (cylinder->rgb[0] << 24 | cylinder->rgb[1] << 16 | cylinder->rgb[2] << 8 | 0xFF);
	}
	else if (flag == DISC)
	{
		disc = (t_disc *)object_ptr;
		*clr = 0xff00ffff;
		(void)disc;
	}
}

void	find_rays(t_master *master)
{
	int		i;
	int		x;
	int		y;
	double	ratio;
	int		fov;
	double	t_near;
	double	t;
	int		object_flag;
	uint32_t	clr;
	void	*object_ptr;
	t_ray	**rays;
	t_vect3f	origin;
	t_vect3f	direction;
	double	scale;

	rays = malloc(sizeof(t_ray *) * (WIDTH));
	t_near = (double)INT_MAX;
	t = (double)INT_MAX;
	i = 0;
	x = 0;
	y = 0;
	fov = master->rt->camera->fov;
	ratio = (double)WIDTH / (double)HEIGHT; // assuming WIDTH > HEIGHT
	update_camera_matrix(master->rt->camera);
	origin = shift_origin(master->rt->camera->matrix); // translate camera position
	// why cos(rad)? see https://www.permadi.com/tutorial/raycast/rayc8.html, alternatively change z to a lower value (e.g. -10)
	scale = tan(cos(rad(fov * 0.5)));
	while (x < WIDTH)
	{
		rays[x] = malloc(sizeof(t_ray) * (HEIGHT));
		y = 0;
		while (y < HEIGHT)
		{
			// will cause conditional jumps for now
			object_flag = EMPTY;
			object_ptr = NULL;
			clr = 0x000000FF;
			t_near = (double)INT_MAX;
			rays[x][y].origin.x = origin.x;
			rays[x][y].origin.y = origin.y;
			rays[x][y].origin.z = origin.z;
			direction.x = (2. * ((x + 0.5) / (double)WIDTH) - 1.) * ratio * scale;
			direction.y = (1. - 2. * ((y + 0.5) / (double)HEIGHT)) * scale;
			direction.z = -1; // change to calibrate focal length, should be -1
			change_ray_direction(master->rt->camera->matrix, &rays[x][y].direction, direction); // change direction based on camera position and its direction
			i = 0;
			while (i < master->rt->n_spheres)
			{
				if (intersect_sphere(rays[x][y], master->rt->spheres[i], &t))
				{
					if (t < t_near)
					{
						t_near = t;
						object_flag = SPHERE;
						object_ptr = master->rt->spheres[i];
					}
				}
				i++;
			}
			i = 0;
			while (i < master->rt->n_planes)
			{
				if (intersect_plane(rays[x][y], master->rt->planes[i], &t, PLANE))
				{
					if (t < t_near)
					{
						t_near = t;
						object_flag = PLANE;
						object_ptr = master->rt->planes[i];
					}
				}
				i++;
			}
			i = 0;
			while (i < master->rt->n_cylinders)
			{
				if (intersect_cylinder(rays[x][y], master->rt->cylinders[i], &t))
				{
					if (t < t_near)
					{
						t_near = t;
						object_flag = CYLINDER;
						object_ptr = master->rt->cylinders[i];
					}
				}
				i++;
			}
			i = 0;
			while (i < master->rt->n_cylinders)
			{
				if (intersect_disc(rays[x][y], master->rt->cylinders[i]->topcap, &t))
				{
					if (t < t_near)
					{
						t_near = t;
						object_flag = DISC;
						object_ptr = master->rt->cylinders[i]->topcap;
					}
				}
				i++;
			}
			i = 0;
			while (i < master->rt->n_cylinders)
			{
				if (intersect_disc(rays[x][y], master->rt->cylinders[i]->botcap, &t))
				{
					if (t < t_near)
					{
						t_near = t;
						object_flag = DISC;
						object_ptr = master->rt->cylinders[i]->botcap;
					}
				}
				i++;
			}
			get_nearest_object(object_flag, object_ptr, &clr);
			mlx_put_pixel(master->img, x, y, clr);
			y++;
		}
		free(rays[x]);
		x++;
	}
	free(rays);
}
