/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   choose_object.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 11:52:19 by plouda            #+#    #+#             */
/*   Updated: 2024/03/04 19:33:35 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

static void	reset_spheres_planes_lights(t_rt *rt)
{
	int	i;

	i = 0;
	while (i < rt->n_spheres)
	{
		rt->spheres[i]->mode = DEFAULT;
		i++;
	}
	i = 0;
	while (i < rt->n_planes)
	{
		rt->planes[i]->mode = DEFAULT;
		i++;
	}
	i = 0;
	while (i < rt->n_lights)
	{
		rt->light_spheres[i]->mode = DEFAULT;
		i++;
	}
}

static void	reset_cylinders_cones(t_rt *rt)
{
	int	i;

	i = 0;
	while (i < rt->n_cylinders)
	{
		rt->cylinders[i]->mode = DEFAULT;
		rt->cylinders[i]->botcap->mode = DEFAULT;
		rt->cylinders[i]->topcap->mode = DEFAULT;
		i++;
	}
	i = 0;
	while (i < rt->n_cones)
	{
		rt->cones[i]->mode = DEFAULT;
		rt->cones[i]->base->mode = DEFAULT;
		rt->cones[i]->pinnacle->mode = DEFAULT;
		i++;
	}
}

void	reset_to_default(t_master *master)
{
	reset_spheres_planes_lights(master->rt);
	reset_cylinders_cones(master->rt);
	master->options->mode = DEFAULT;
	find_rays(master);
}

static void	pick_light(t_rt *rt)
{
	int	i;

	i = 0;
	while (i < rt->n_lights)
	{
		if (rt->light_spheres[i]->mode == HIGHLIGHT)
		{
			rt->light_spheres[i]->mode = DEFAULT;
			if (i + 1 < rt->n_lights)
				rt->light_spheres[i + 1]->mode = HIGHLIGHT;
			else
				rt->light_spheres[0]->mode = HIGHLIGHT;
			return ;
		}
		i++;
	}
}

static void	pick_object(t_rt *rt)
{
	if (0 < rt->n_spheres)
		rt->spheres[0]->mode = HIGHLIGHT;
	else if (0 < rt->n_planes)
		rt->planes[0]->mode = HIGHLIGHT;
	else if (0 < rt->n_cylinders)
	{
		rt->cylinders[0]->mode = HIGHLIGHT;
		rt->cylinders[0]->botcap->mode = HIGHLIGHT;
		rt->cylinders[0]->topcap->mode = HIGHLIGHT;
	}
	else if (0 < rt->n_cones)
	{
		rt->cones[0]->mode = HIGHLIGHT;
		rt->cones[0]->base->mode = HIGHLIGHT;
		rt->cones[0]->pinnacle->mode = HIGHLIGHT;
	}
}

static int	change_modes_cone(t_cone *cone, t_mode mode)
{
	cone->mode = mode;
	cone->base->mode = mode;
	cone->pinnacle->mode = mode;
	return (0);
}

static int	change_modes_cylinder(t_cylinder *cylinder, t_mode mode)
{
	cylinder->mode = mode;
	cylinder->botcap->mode = mode;
	cylinder->topcap->mode = mode;
	return (0);
}

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

static int	iterate_and_pick_objects(t_master *master, int *flag)
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

void	choose_object(t_master *master)
{
	int	flag;

	flag = 0;
	if (master->options->mode == LIGHTING)
	{
		pick_light(master->rt);
		return ;
	}
	else if (master->options->mode == OBJECT_CHOICE)
	{
		pick_object(master->rt);
		master->options->mode = HIGHLIGHT;
		return ;
	}
	else
		iterate_and_pick_objects(master, &flag);
}

void	connect_disc_to_object(t_rt *rt, void *object_ptr)
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
	if (rf.object_flag == EMPTY)
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
