/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   choose_object_by_mouse_reference.c                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 11:52:05 by plouda            #+#    #+#             */
/*   Updated: 2024/03/07 14:15:57 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

static void	connect_disc_to_object(t_rt *rt, void *object_ptr)
{
	int	i;

	i = 0;
	while (i < rt->n_cylinders)
	{
		if (rt->cylinders[i]->botcap == object_ptr
			|| rt->cylinders[i]->topcap == object_ptr)
			change_modes_cylinder(rt->cylinders[i], HIGHLIGHT);
		i++;
	}
	i = 0;
	while (i < rt->n_cones)
	{
		if (rt->cones[i]->base == object_ptr
			|| rt->cones[i]->pinnacle == object_ptr)
			change_modes_cone(rt->cones[i], HIGHLIGHT);
		i++;
	}
}

static void	test_reference_spheres_planes(t_master *master, t_rayfinder rf)
{
	int	i;

	i = 0;
	if (rf.object_flag == SPHERE)
	{
		while (i < master->rt->n_spheres)
		{
			if (master->rt->spheres[i] == rf.object_ptr)
				master->rt->spheres[i]->mode = HIGHLIGHT;
			i++;
		}
	}
	else if (rf.object_flag == PLANE)
	{
		while (i < master->rt->n_planes)
		{
			if (master->rt->planes[i] == rf.object_ptr)
				master->rt->planes[i]->mode = HIGHLIGHT;
			i++;
		}
	}
}

static void	test_reference_cylinders_cones(t_master *master, t_rayfinder rf)
{
	int	i;

	i = 0;
	if (rf.object_flag == CYLINDER)
	{
		while (i < master->rt->n_cylinders)
		{
			if (master->rt->cylinders[i] == rf.object_ptr)
				change_modes_cylinder(master->rt->cylinders[i], HIGHLIGHT);
			i++;
		}
	}
	else if (rf.object_flag == CONE)
	{
		while (i < master->rt->n_cones)
		{
			if (master->rt->cones[i] == rf.object_ptr)
				change_modes_cone(master->rt->cones[i], HIGHLIGHT);
			i++;
		}
	}
}

void	set_highlight_from_reference(t_master *master, t_rayfinder rf)
{
	int	i;

	i = 0;
	if (rf.object_flag == EMPTY || rf.object_flag == SKYBOX)
		return ;
	test_reference_spheres_planes(master, rf);
	test_reference_cylinders_cones(master, rf);
	if (rf.object_flag == DISC)
		connect_disc_to_object(master->rt, rf.object_ptr);
	else if (rf.object_flag == LIGHT)
	{
		master->options->mode = LIGHTING;
		while (i < master->rt->n_lights)
		{
			if (master->rt->light_spheres[i] == rf.object_ptr)
			{
				master->rt->light_spheres[i]->mode = HIGHLIGHT;
				return ;
			}
			i++;
		}
	}
	master->options->mode = HIGHLIGHT;
}
