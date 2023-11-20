/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   o_plane.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 15:42:08 by plouda            #+#    #+#             */
/*   Updated: 2023/11/20 15:46:47 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minirt.h"

void	init_planes(t_rt *rt, int *ids)
{
	int	i;

	i = 0;
	rt->planes = malloc(sizeof(t_plane *) * (ids[4] + 1));
	rt->n_planes = 0;
	while (i < (ids[4]))
	{
		rt->planes[i] = malloc(sizeof(t_plane));
		rt->planes[i]->coords = malloc(sizeof(double) * 3);
		rt->planes[i]->nvect = malloc(sizeof(double) * 3);
		rt->planes[i++]->rgb = malloc(sizeof(int) * 3);
	}
	rt->planes[i] = NULL;
}

int	check_format_plane(char **split)
{
	if (!has_spec_count(split, 4))
		return (id_err("pl", E_SPEC, "3"));
	if (!is_float_triad(split[1]))
		return (id_err("pl", E_COORD, E_TRIAD_INTFLOAT));
	if (!triad_in_range(split[1]))
		return (id_err("pl", E_COORD, E_MAX_DIGITS));
	if (!is_float_triad(split[2]))
		return (id_err("pl", E_VECT, E_TRIAD_INTFLOAT));
	if (!triad_in_range(split[2]))
		return (id_err("pl", E_VECT, E_MAX_DIGITS));
	if (!is_rgb_format(split[3]))
		return (id_err("pl", E_RGB, E_TRIAD_INT));
	return (0);
}

int	fill_plane(t_rt *rt, char **split)
{
	int			i;

	i = rt->n_planes;
	if (check_format_plane(split))
		return (1);
	get_coords(rt->planes[i]->coords, split[1]);
	if (!get_nvect(rt->planes[i]->nvect, split[2]))
		return (id_err("pl", E_VECT_RANGE, E_RANGE_NORM));
	if (!get_rgb(rt->planes[i]->rgb, split[3]))
		return (id_err("pl", E_RGB_RANGE, E_RANGE_INT));
	rt->n_planes++;
	return (0);
}

void	free_planes(t_rt *rt)
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
