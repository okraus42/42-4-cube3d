/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_objects.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 11:10:13 by plouda            #+#    #+#             */
/*   Updated: 2023/11/20 12:45:27 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minirt.h"

static void	init_acl(t_rt *rt)
{
	rt->ambient = malloc(sizeof(t_ambient));
	rt->ambient->rgb = malloc(sizeof(int) * 3);
	rt->camera = malloc(sizeof(t_camera));
	rt->camera->coords = malloc(sizeof(double) * 3);
	rt->camera->nvect = malloc(sizeof(double) * 3);
	rt->light = malloc(sizeof(t_light));
	rt->light->coords = malloc(sizeof(double) * 3);
	rt->light->rgb = malloc(sizeof(int) * 3);
}

static void	init_spheres(t_rt *rt, int *ids)
{
	int	i;

	i = 0;
	rt->spheres = malloc(sizeof(t_sphere *) * (ids[3] + 1));
	rt->n_spheres = 0;
	while (i < (ids[3]))
	{
		rt->spheres[i] = malloc(sizeof(t_sphere));
		rt->spheres[i]->coords = malloc(sizeof(double) * 3);
		rt->spheres[i++]->rgb = malloc(sizeof(int) * 3);
	}
	rt->spheres[i] = NULL;
}

static void	init_planes(t_rt *rt, int *ids)
{
	int	i;

	i = 0;
	rt->planes = malloc(sizeof(t_plane *) * (ids[4] + 1));
	rt->n_planes = 0;
	while (i < (ids[4]))
	{
		rt->planes[i] = malloc(sizeof(t_plane));
		rt->planes[i]->coords = malloc(sizeof(double) * 3);
		rt->planes[i]->nvect = malloc(sizeof(double) * 3);
		rt->planes[i++]->rgb = malloc(sizeof(int) * 3);
	}
	rt->planes[i] = NULL;
}

static void	init_cylinders(t_rt *rt, int *ids)
{
	int	i;

	i = 0;
	rt->cylinders = malloc(sizeof(t_cylinder *) * (ids[5] + 1));
	rt->n_cylinders = 0;
	while (i < (ids[5]))
	{
		rt->cylinders[i] = malloc(sizeof(t_cylinder));
		rt->cylinders[i]->coords = malloc(sizeof(double) * 3);
		rt->cylinders[i]->nvect = malloc(sizeof(double) * 3);
		rt->cylinders[i++]->rgb = malloc(sizeof(double) * 3);
	}
	rt->cylinders[i] = NULL;
}

int	init_objects(t_rt *rt, int *ids)
{
	init_acl(rt);
	init_spheres(rt, ids);
	init_planes(rt, ids);
	init_cylinders(rt, ids);
	return (0);
}
