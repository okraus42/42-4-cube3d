/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   choose_object.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 11:52:19 by plouda            #+#    #+#             */
/*   Updated: 2024/03/02 15:39:14 by okraus           ###   ########.fr       */
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
	i = 0;
	while (i < master->rt->n_cones)
	{
		master->rt->cones[i]->mode = DEFAULT;
		master->rt->cones[i]->base->mode = DEFAULT;
		master->rt->cones[i]->pinnacle->mode = DEFAULT;
		i++;
	}
	i = 0;
	while (i < master->rt->n_lights)
	{
		master->rt->light_spheres[i]->mode = DEFAULT;
		i++;
	}
	master->options->mode = DEFAULT;
	find_rays(master);
}

void	choose_object(t_master *master, mlx_key_data_t keydata)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	if (master->options->mode == LIGHTING)
	{
		while (i < master->rt->n_lights)
		{
			if (master->rt->light_spheres[i]->mode == HIGHLIGHT)
			{
				master->rt->light_spheres[i]->mode = DEFAULT;
				if (i + 1 < master->rt->n_lights)
					master->rt->light_spheres[i + 1]->mode = HIGHLIGHT;
				else
					master->rt->light_spheres[0]->mode = HIGHLIGHT;
				return ;
			}
			i++;
		}
	}
	else if (master->options->mode == OBJECT_CHOICE && keydata.key)
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
		else if (i < master->rt->n_cones)
		{
			master->rt->cones[i]->mode = HIGHLIGHT;
			master->rt->cones[i]->base->mode = HIGHLIGHT;
			master->rt->cones[i]->pinnacle->mode = HIGHLIGHT;
		}
		master->options->mode = HIGHLIGHT;
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
					master->rt->spheres[i + 1]->mode = HIGHLIGHT;
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
				return ;
			}
			if (master->rt->planes[i]->mode == HIGHLIGHT)
			{
				master->rt->planes[i]->mode = DEFAULT;
				if (i + 1 < master->rt->n_planes)
				{
					master->rt->planes[i + 1]->mode = HIGHLIGHT;
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
				return ;
			}
			if (master->rt->cylinders[i]->mode == HIGHLIGHT)
			{
				master->rt->cylinders[i]->mode = DEFAULT;
				master->rt->cylinders[i]->botcap->mode = DEFAULT;
				master->rt->cylinders[i]->topcap->mode = DEFAULT;
				if (i + 1 < master->rt->n_cylinders)
				{
					master->rt->cylinders[i + 1]->mode = HIGHLIGHT;
					master->rt->cylinders[i + 1]->botcap->mode = HIGHLIGHT;
					master->rt->cylinders[i + 1]->topcap->mode = HIGHLIGHT;
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
		while (i < master->rt->n_cones)
		{
			if (flag)
			{
				master->rt->cones[i]->mode = HIGHLIGHT;
				master->rt->cones[i]->base->mode = HIGHLIGHT;
				master->rt->cones[i]->pinnacle->mode = HIGHLIGHT;
				return ;
			}
			if (master->rt->cones[i]->mode == HIGHLIGHT)
			{
				master->rt->cones[i]->mode = DEFAULT;
				master->rt->cones[i]->base->mode = DEFAULT;
				master->rt->cones[i]->pinnacle->mode = DEFAULT;
				if (i + 1 < master->rt->n_cones)
				{
					master->rt->cones[i + 1]->mode = HIGHLIGHT;
					master->rt->cones[i + 1]->base->mode = HIGHLIGHT;
					master->rt->cones[i + 1]->pinnacle->mode = HIGHLIGHT;
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
}

void	connect_disc_to_object(t_rt *rt, void *object_ptr)
{
	int	i;

	i = 0;
	while (i < rt->n_cylinders)
	{
		if (rt->cylinders[i]->botcap == object_ptr
			|| rt->cylinders[i]->topcap == object_ptr)
		{
			rt->cylinders[i]->mode = HIGHLIGHT;
			rt->cylinders[i]->botcap->mode = HIGHLIGHT;
			rt->cylinders[i]->topcap->mode = HIGHLIGHT;
		}
		i++;
	}
	i = 0;
	while (i < rt->n_cones)
	{
		if (rt->cones[i]->base == object_ptr
			|| rt->cones[i]->pinnacle == object_ptr)
		{
			rt->cones[i]->mode = HIGHLIGHT;
			rt->cones[i]->base->mode = HIGHLIGHT;
			rt->cones[i]->pinnacle->mode = HIGHLIGHT;
		}
		i++;
	}
}

void	set_highlight_from_reference(t_master *master, t_rayfinder rf)
{
	int	i;

	i = 0;
	if (rf.object_flag == EMPTY)
		return ;
	else if (rf.object_flag == SPHERE)
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
	else if (rf.object_flag == CYLINDER)
	{
		while (i < master->rt->n_cylinders)
		{
			if (master->rt->cylinders[i] == rf.object_ptr)
			{
				master->rt->cylinders[i]->mode = HIGHLIGHT;
				master->rt->cylinders[i]->botcap->mode = HIGHLIGHT;
				master->rt->cylinders[i]->topcap->mode = HIGHLIGHT;
			}
			i++;
		}
	}
	else if (rf.object_flag == CONE)
	{
		while (i < master->rt->n_cones)
		{
			if (master->rt->cones[i] == rf.object_ptr)
			{
				master->rt->cones[i]->mode = HIGHLIGHT;
				master->rt->cones[i]->base->mode = HIGHLIGHT;
				master->rt->cones[i]->pinnacle->mode = HIGHLIGHT;
			}
			i++;
		}
	}
	else if (rf.object_flag == DISC)
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
