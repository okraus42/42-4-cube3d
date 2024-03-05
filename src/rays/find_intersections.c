/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_intersections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 15:14:02 by plouda            #+#    #+#             */
/*   Updated: 2024/03/05 15:35:56 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

int	update_object_ref(t_rayfinder *rf, void *object, t_object flag)
{
	if (rf->t < rf->t_near)
	{
		rf->t_near = rf->t;
		rf->object_flag = flag;
		rf->object_ptr = object;
	}
	return (1);
}

static void	find_intersections_1(t_master *master,
	t_ray ray, t_rayfinder *rf, int *flag)
{
	int	i;

	i = 0;
	while (i < master->rt->n_spheres)
	{
		if (intersect_sphere(ray, master->rt->spheres[i], &rf->t))
			*flag = update_object_ref(rf, master->rt->spheres[i], SPHERE);
		i++;
	}
	i = 0;
	while (i < master->rt->n_planes)
	{
		if (intersect_plane(ray, master->rt->planes[i], &rf->t, PLANE))
			*flag = update_object_ref(rf, master->rt->planes[i], PLANE);
		i++;
	}
}

static void	find_intersections_2(t_master *master,
	t_ray ray, t_rayfinder *rf, int *flag)
{
	int	i;

	i = 0;
	while (i < master->rt->n_cylinders)
	{
		if (intersect_cylinder(ray, master->rt->cylinders[i], &rf->t))
			*flag = update_object_ref(rf, master->rt->cylinders[i], CYLINDER);
		if (intersect_disc(ray, master->rt->cylinders[i]->topcap, &rf->t))
			*flag = update_object_ref(rf,
					master->rt->cylinders[i]->topcap, DISC);
		if (intersect_disc(ray, master->rt->cylinders[i]->botcap, &rf->t))
			*flag = update_object_ref(rf,
					master->rt->cylinders[i]->botcap, DISC);
		i++;
	}
	i = 0;
	while (i < master->rt->n_cones)
	{
		if (intersect_cone(ray, master->rt->cones[i], &rf->t))
			*flag = update_object_ref(rf, master->rt->cones[i], CONE);
		if (intersect_disc(ray, master->rt->cones[i]->base, &rf->t))
			*flag = update_object_ref(rf, master->rt->cones[i]->base, DISC);
		i++;
	}
}

int	find_intersections(t_master *master,
	t_ray ray, t_rayfinder *rf, t_raytype type)
{
	int	i;
	int	flag;

	flag = 0;
	find_intersections_1(master, ray, rf, &flag);
	find_intersections_2(master, ray, rf, &flag);
	i = 0;
	while (i < master->rt->n_lights)
	{
		if (type != SHADOW && intersect_sphere(ray,
				master->rt->light_spheres[i], &rf->t))
			flag = update_object_ref(rf, master->rt->light_spheres[i], LIGHT);
		i++;
	}
	return (flag);
}
