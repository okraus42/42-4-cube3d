/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   choose_object_by_keys.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 11:52:19 by plouda            #+#    #+#             */
/*   Updated: 2024/03/05 12:04:52 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

static void	pick_first_light(t_rt *rt)
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

static void	pick_first_object(t_rt *rt)
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

void	choose_object(t_master *master)
{
	int	flag;

	flag = 0;
	if (master->options->mode == LIGHTING)
	{
		pick_first_light(master->rt);
		return ;
	}
	else if (master->options->mode == OBJECT_CHOICE)
	{
		pick_first_object(master->rt);
		master->options->mode = HIGHLIGHT;
		return ;
	}
	else
		iterate_and_pick(master, &flag);
}
