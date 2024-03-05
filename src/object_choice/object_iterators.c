/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_iterators.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 11:55:39 by plouda            #+#    #+#             */
/*   Updated: 2024/03/05 12:42:17 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

static int	iterate_and_pick_cones(t_master *master, int *flag)
{
	int	i;

	i = 0;
	while (i < master->rt->n_cones)
	{
		if (*flag && master->rt->cones[0]->mode != HIGHLIGHT)
			return (change_modes_cone(master->rt->cones[0], HIGHLIGHT));
		if (master->rt->cones[i]->mode == HIGHLIGHT)
		{
			change_modes_cone(master->rt->cones[i], DEFAULT);
			if (i + 1 < master->rt->n_cones)
				return (change_modes_cone(master->rt->cones[i + 1], HIGHLIGHT));
			else
			{
				master->options->mode = OBJECT_CHOICE;
				choose_object(master);
				return (0);
			}
		}
		i++;
	}
	master->options->mode = OBJECT_CHOICE;
	choose_object(master);
	return (0);
}

static int	iterate_and_pick_cylinders(t_master *master, int *flag)
{
	int	i;

	i = 0;
	while (i < master->rt->n_cylinders)
	{
		if (*flag && master->rt->cylinders[0]->mode != HIGHLIGHT)
			return (change_modes_cylinder(master->rt->cylinders[0], HIGHLIGHT));
		if (master->rt->cylinders[i]->mode == HIGHLIGHT)
		{
			change_modes_cylinder(master->rt->cylinders[i], DEFAULT);
			if (i + 1 < master->rt->n_cylinders)
			{
				return (change_modes_cylinder(master->rt->cylinders[i + 1],
						HIGHLIGHT));
			}
			else
			{
				*flag = 1;
				return (iterate_and_pick_cones(master, flag));
			}
		}
		i++;
	}
	return (iterate_and_pick_cones(master, flag));
}

static int	iterate_and_pick_planes(t_master *master, int *flag)
{
	int	i;

	i = 0;
	while (i < master->rt->n_planes)
	{
		if (*flag && master->rt->planes[0]->mode != HIGHLIGHT)
			return (master->rt->planes[0]->mode = HIGHLIGHT);
		if (master->rt->planes[i]->mode == HIGHLIGHT)
		{
			master->rt->planes[i]->mode = DEFAULT;
			if (i + 1 < master->rt->n_planes)
				return (master->rt->planes[i + 1]->mode = HIGHLIGHT);
			else
			{
				*flag = 1;
				return (iterate_and_pick_cylinders(master, flag));
			}
		}
		i++;
	}
	return (iterate_and_pick_cylinders(master, flag));
}

static int	iterate_and_pick_spheres(t_master *master, int *flag)
{
	int	i;

	i = 0;
	while (i < master->rt->n_spheres)
	{
		if (master->rt->spheres[i]->mode == HIGHLIGHT)
		{
			master->rt->spheres[i]->mode = DEFAULT;
			if (i + 1 < master->rt->n_spheres)
				return (master->rt->spheres[i + 1]->mode = HIGHLIGHT);
			else
			{
				*flag = 1;
				return (iterate_and_pick_planes(master, flag));
			}
		}
		i++;
	}
	return (iterate_and_pick_planes(master, flag));
}

int	iterate_and_pick(t_master *master, int *flag)
{
	return (iterate_and_pick_spheres(master, flag));
}
