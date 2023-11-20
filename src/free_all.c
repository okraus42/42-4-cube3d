/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 09:26:32 by plouda            #+#    #+#             */
/*   Updated: 2023/11/20 12:47:21 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minirt.h"

static void	free_acl(t_rt *rt)
{
	free(rt->ambient->rgb);
	free(rt->ambient);
	free(rt->camera->coords);
	free(rt->camera->nvect);
	free(rt->camera);
	free(rt->light->coords);
	free(rt->light->rgb);
	free(rt->light);
}

static void	free_spheres(t_rt *rt)
{
	int	i;

	i = 0;
	while (rt->spheres[i] != NULL)
	{
		free(rt->spheres[i]->coords);
		free(rt->spheres[i]->rgb);
		free(rt->spheres[i++]);
	}
	free(rt->spheres);
}

static void	free_planes(t_rt *rt)
{
	int	i;

	i = 0;
	while (rt->planes[i] != NULL)
	{
		free(rt->planes[i]->coords);
		free(rt->planes[i]->nvect);
		free(rt->planes[i]->rgb);
		free(rt->planes[i++]);
	}
	free(rt->planes);
}

static void	free_cylinders(t_rt *rt)
{
	int	i;

	i = 0;
	while (rt->cylinders[i] != NULL)
	{
		free(rt->cylinders[i]->coords);
		free(rt->cylinders[i]->nvect);
		free(rt->cylinders[i]->rgb);
		free(rt->cylinders[i++]);
	}
	free(rt->cylinders);
}

void	free_all(t_rt *rt)
{
	free_acl(rt);
	free_spheres(rt);
	free_planes(rt);
	free_cylinders(rt);
	free(rt);
}
