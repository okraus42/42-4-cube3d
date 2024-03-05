/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reset_to_default.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 11:53:57 by plouda            #+#    #+#             */
/*   Updated: 2024/03/05 11:54:26 by plouda           ###   ########.fr       */
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
