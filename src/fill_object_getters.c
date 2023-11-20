/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_objects_getters.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 14:03:53 by plouda            #+#    #+#             */
/*   Updated: 2023/11/20 15:53:21 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minirt.h"

int	get_rgb(int *rgb, char *triad)
{
	char	**rgb_split;

	rgb_split = ft_split(triad, ',');
	rgb[R] = ft_atoi(rgb_split[0]);
	rgb[G] = ft_atoi(rgb_split[1]);
	rgb[B] = ft_atoi(rgb_split[2]);
	ft_free_split(&rgb_split);
	if (rgb[R] > 255 || rgb[R] < 0
		|| rgb[G] > 255 || rgb[G] < 0
		|| rgb[B] > 255 || rgb[B] < 0)
		return (0);
	return (1);
}

int	get_coords(double *coords, char *triad)
{
	char	**coords_split;

	coords_split = ft_split(triad, ',');
	coords[X] = ft_atof(coords_split[0]);
	coords[Y] = ft_atof(coords_split[1]);
	coords[Z] = ft_atof(coords_split[2]);
	ft_free_split(&coords_split);
	return (1);
}

int	get_nvect(double *nvect, char *triad)
{
	char	**nvect_split;

	nvect_split = ft_split(triad, ',');
	nvect[X] = ft_atof(nvect_split[0]);
	nvect[Y] = ft_atof(nvect_split[1]);
	nvect[Z] = ft_atof(nvect_split[2]);
	ft_free_split(&nvect_split);
	if (nvect[X] < -1. || nvect[X] > 1.
		|| nvect[Y] < -1. || nvect[Y] > 1.
		|| nvect[Z] < -1. || nvect[Z] > 1.)
		return (0);
	return (1);
}
