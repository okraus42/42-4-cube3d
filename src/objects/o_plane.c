/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   o_plane.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 15:42:08 by plouda            #+#    #+#             */
/*   Updated: 2023/12/17 17:56:08 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	init_planes(t_rt *rt, int *ids)
{
	int	i;

	i = 0;
	rt->planes = ft_calloc(ids[4] + 1, sizeof(t_plane *));
	rt->n_planes = 0;
	while (i < (ids[4]))
	{
		rt->planes[i] = ft_calloc(1, sizeof(t_plane));
		rt->planes[i]->coords = ft_calloc(3, sizeof(double));
		rt->planes[i]->nvect = ft_calloc(3, sizeof(double));
		rt->planes[i]->rgb = ft_calloc(3, sizeof(int));
		rt->planes[i++]->normal = ft_calloc(1, sizeof(t_vect3f));
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
	*rt->planes[i]->normal = get_normal(rt->planes[i]->nvect[X], \
			rt->planes[i]->nvect[Y], rt->planes[i]->nvect[Z]);
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
		free(rt->planes[i]->normal);
		free(rt->planes[i++]);
	}
	free(rt->planes);
}
