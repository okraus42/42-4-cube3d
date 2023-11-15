/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 09:26:32 by plouda            #+#    #+#             */
/*   Updated: 2023/11/15 09:47:33 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minirt.h"

void	free_all(t_rt *rt)
{
	int	i;

	i = 0;
	free(rt->ambient->rgb);
	free(rt->ambient);
	free(rt->camera->coords);
	free(rt->camera->nvect);
	free(rt->camera);
	free(rt->light->coords);
	free(rt->light->rgb);
	free(rt->light);
	while (rt->spheres[i] != NULL)
	{
		free(rt->spheres[i]->coords);
		free(rt->spheres[i]->rgb);
		free(rt->spheres[i++]);
	}
	free(rt->spheres);
	i = 0;
	while (rt->planes[i] != NULL)
	{
		free(rt->planes[i]->coords);
		free(rt->planes[i]->nvect);
		free(rt->planes[i]->rgb);
		free(rt->planes[i++]);
	}
	free(rt->planes);
	i = 0;
	while (rt->cylinders[i] != NULL)
	{
		free(rt->cylinders[i]->coords);
		free(rt->cylinders[i]->nvect);
		free(rt->cylinders[i]->rgb);
		free(rt->cylinders[i++]);
	}
	free(rt->cylinders);
	free(rt);
}
