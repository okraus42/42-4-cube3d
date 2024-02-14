/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   o_checkerboard.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 18:21:02 by plouda            #+#    #+#             */
/*   Updated: 2024/02/14 11:35:15 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	init_checkerboards(t_rt *rt, int *ids)
{
	int	i;

	i = 0;
	rt->checkerboards = ft_calloc(ids[7] + 1, sizeof(t_checkerboard *));
	rt->n_checkerboards = 0;
	while (i < (ids[7]))
	{
		rt->checkerboards[i] = ft_calloc(1, sizeof(t_checkerboard));
		rt->checkerboards[i]->rgb1 = ft_calloc(3, sizeof(int));
		rt->checkerboards[i++]->rgb2 = ft_calloc(3, sizeof(int));
	}
	rt->checkerboards[i] = NULL;
}

int	check_format_checkerboard(char **split)
{
	if (!has_spec_count(split, 4))
		return (id_err(".ch", E_SPEC, "3"));
	if (!has_valid_id_attribute(split[0]))
		return (id_err(".ch", "Invalid checkerboard ID", NULL));
	if (!is_rgb_format(split[1]))
		return (id_err(".ch", E_RGB, E_TRIAD_INT));
	if (!is_rgb_format(split[2]))
		return (id_err(".ch", E_RGB, E_TRIAD_INT));
	if (!is_floatable(split[3]))
		return (id_err(".ch", "Invalid magnitude format", E_INTFLOAT));
	if (!float_in_range(split[3]))
		return (id_err(".ch", "Invalid magnitude format", E_MAX_DIGITS));
	return (0);
}

int	get_checkerboard_id(t_checkerboard *checkerboard, char *specifier)
{
	char	**split;
	int		i;
	int		flag;

	i = 0;
	flag = 0;
	while (specifier[i])
	{
		if (specifier[i] == '/')
			flag = 1;
		i++;
	}
	if (!flag)
		return (flag);
	split = ft_split(specifier, '/');
	checkerboard->id = ft_atoi(split[1]);
	free(split[0]);
	free(split[1]);
	free(split);
	return (flag);
}

int	fill_checkerboard(t_rt *rt, char **split)
{
	int			i;

	i = rt->n_checkerboards;
	if (check_format_checkerboard(split))
		return (1);
	if (!get_rgb(rt->checkerboards[i]->rgb1, split[1]))
		return (id_err(".ch", E_RGB_RANGE, E_RANGE_INT));
	if (!get_rgb(rt->checkerboards[i]->rgb2, split[2]))
		return (id_err(".ch", E_RGB_RANGE, E_RANGE_INT));
	rt->checkerboards[i]->magnitude = ft_atof(split[3]);
	if (rt->checkerboards[i]->magnitude <= 0)
		return (id_err(".ch", "Magnitude value too low", E_RANGE_STRICT));
	if (!get_checkerboard_id(rt->checkerboards[i], split[0]))
		return (id_err(".ch", "Invalid .ch specification", NULL));
	rt->n_checkerboards++;
	return (0);
}

void	free_checkerboards(t_rt *rt)
{
	int	i;

	i = 0;
	while (rt->checkerboards[i] != NULL)
	{
		free(rt->checkerboards[i]->rgb1);
		free(rt->checkerboards[i]->rgb2);
		free(rt->checkerboards[i++]);
	}
	free(rt->checkerboards);
}
