/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   o_cylinder.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 15:43:36 by plouda            #+#    #+#             */
/*   Updated: 2023/12/08 11:11:11 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minirt.h"

void	init_cylinders(t_rt *rt, int *ids)
{
	int	i;

	i = 0;
	rt->cylinders = malloc(sizeof(t_cylinder *) * (ids[5] + 1));
	rt->n_cylinders = 0;
	while (i < (ids[5]))
	{
		rt->cylinders[i] = malloc(sizeof(t_cylinder));
		rt->cylinders[i]->coords = malloc(sizeof(double) * 3);
		rt->cylinders[i]->nvect = malloc(sizeof(double) * 3);
		rt->cylinders[i]->rgb = malloc(sizeof(double) * 3);
		init_discs(rt->cylinders[i]);
		i++;
	}
	rt->cylinders[i] = NULL;
}

int	check_format_cylinder(char **split)
{
	if (!has_spec_count(split, 6))
		return (id_err("cy", E_SPEC, "5"));
	if (!is_float_triad(split[1]))
		return (id_err("cy", E_COORD, E_TRIAD_INTFLOAT));
	if (!triad_in_range(split[1]))
		return (id_err("cy", E_COORD, E_MAX_DIGITS));
	if (!is_float_triad(split[2]))
		return (id_err("cy", E_VECT, E_TRIAD_INTFLOAT));
	if (!triad_in_range(split[2]))
		return (id_err("cy", E_VECT, E_MAX_DIGITS));
	if (!is_floatable(split[3]))
		return (id_err("cy", E_DIA, E_INTFLOAT));
	if (!float_in_range(split[3]))
		return (id_err("cy", E_DIA, E_MAX_DIGITS));
	if (!is_floatable(split[4]))
		return (id_err("cy", E_HEIGHT, E_INTFLOAT));
	if (!float_in_range(split[4]))
		return (id_err("cy", E_HEIGHT, E_MAX_DIGITS));
	if (!is_rgb_format(split[5]))
		return (id_err("cy", E_RGB, E_TRIAD_INT));
	return (0);
}

int	fill_cylinder(t_rt *rt, char **split)
{
	int			i;

	i = rt->n_cylinders;
	if (check_format_cylinder(split))
		return (1);
	get_coords(rt->cylinders[i]->coords, split[1]);
	if (!get_nvect(rt->cylinders[i]->nvect, split[2]))
		return (id_err("cy", E_VECT_RANGE, E_RANGE_NORM));
	rt->cylinders[i]->diameter = ft_atof(split[3]);
	if (rt->cylinders[i]->diameter <= 0)
		return (id_err("cy", E_DIA_RANGE, E_RANGE_STRICT));
	rt->cylinders[i]->height = ft_atof(split[4]);
	if (rt->cylinders[i]->height <= 0)
		return (id_err("cy", E_HEIGHT_RANGE, E_RANGE_STRICT));
	if (!get_rgb(rt->cylinders[i]->rgb, split[5]))
		return (id_err("cy", E_RGB_RANGE, E_RANGE_INT));
	get_discs(rt->cylinders[i]);
	rt->n_cylinders++;
	return (0);
}

void	free_cylinders(t_rt *rt)
{
	int	i;

	i = 0;
	while (rt->cylinders[i] != NULL)
	{
		free(rt->cylinders[i]->coords);
		free(rt->cylinders[i]->nvect);
		free(rt->cylinders[i]->rgb);
		free_discs(rt->cylinders[i]);
		free(rt->cylinders[i++]);
	}
	free(rt->cylinders);
}
