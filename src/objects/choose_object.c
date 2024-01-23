/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   choose_object.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 11:52:19 by plouda            #+#    #+#             */
/*   Updated: 2024/01/22 10:52:01 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	reset_to_default(t_master *master)
{
	int	i;

	i = 0;
	while (i < master->rt->n_spheres)
	{
		master->rt->spheres[i]->mode = DEFAULT;
		i++;
	}
	i = 0;
	while (i < master->rt->n_planes)
	{
		master->rt->planes[i]->mode = DEFAULT;
		i++;
	}
	i = 0;
	while (i < master->rt->n_cylinders)
	{
		master->rt->cylinders[i]->mode = DEFAULT;
		master->rt->cylinders[i]->botcap->mode = DEFAULT;
		master->rt->cylinders[i]->topcap->mode = DEFAULT;
		i++;
	}
	while (i < master->rt->n_cones)
	{
		master->rt->cones[i]->mode = DEFAULT;
		master->rt->cones[i]->base->mode = DEFAULT;
		master->rt->cones[i]->pinnacle->mode = DEFAULT;
		i++;
	}
	master->rt->light_sphere->mode = DEFAULT;
	master->options->mode = DEFAULT;
	find_rays(master);
}


// add cones
void	choose_object(t_master *master, mlx_key_data_t keydata)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	if (master->options->mode == OBJECT_CHOICE && keydata.key)
	{
		if (i < master->rt->n_spheres)
			master->rt->spheres[i]->mode = HIGHLIGHT;
		else if (i < master->rt->n_planes)
			master->rt->planes[i]->mode = HIGHLIGHT;
		else if (i < master->rt->n_cylinders)
		{
			master->rt->cylinders[i]->mode = HIGHLIGHT;
			master->rt->cylinders[i]->botcap->mode = HIGHLIGHT;
			master->rt->cylinders[i]->topcap->mode = HIGHLIGHT;
		}
		master->options->mode = HIGHLIGHT;
		find_rays(master);
		return ;
	}
	else
	{
		while (i < master->rt->n_spheres)
		{
			if (master->rt->spheres[i]->mode == HIGHLIGHT)
			{
				master->rt->spheres[i]->mode = DEFAULT;
				if (i + 1 < master->rt->n_spheres)
				{
					master->rt->spheres[i+1]->mode = HIGHLIGHT;
					find_rays(master);
					return ;
				}
				else
				{
					flag = 1;
					break ;
				}
			}
			i++;
		}
		i = 0;
		while (i < master->rt->n_planes)
		{
			if (flag)
			{
				master->rt->planes[i]->mode = HIGHLIGHT;
				find_rays(master);
				return ;
			}
			if (master->rt->planes[i]->mode == HIGHLIGHT)
			{
				master->rt->planes[i]->mode = DEFAULT;
				if (i + 1 < master->rt->n_planes)
				{
					master->rt->planes[i+1]->mode = HIGHLIGHT;
					find_rays(master);
					return ;
				}
				else
				{
					flag = 1;
					break ;
				}
			}
			i++;
		}
		i = 0;
		while (i < master->rt->n_cylinders)
		{
			if (flag)
			{
				master->rt->cylinders[i]->mode = HIGHLIGHT;
				master->rt->cylinders[i]->botcap->mode = HIGHLIGHT;
				master->rt->cylinders[i]->topcap->mode = HIGHLIGHT;
				find_rays(master);
				return ;
			}
			if (master->rt->cylinders[i]->mode == HIGHLIGHT)
			{
				master->rt->cylinders[i]->mode = DEFAULT;
				master->rt->cylinders[i]->botcap->mode = DEFAULT;
				master->rt->cylinders[i]->topcap->mode = DEFAULT;
				if (i + 1 < master->rt->n_cylinders)
				{
					master->rt->cylinders[i+1]->mode = HIGHLIGHT;
					master->rt->cylinders[i+1]->botcap->mode = HIGHLIGHT;
					master->rt->cylinders[i+1]->topcap->mode = HIGHLIGHT;
					find_rays(master);
					return ;
				}
				else
					break ;
			}
			i++;
		}
	}
	master->options->mode = OBJECT_CHOICE;
	choose_object(master, keydata);
	find_rays(master);
}