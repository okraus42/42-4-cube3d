/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   o_light.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 15:29:51 by plouda            #+#    #+#             */
/*   Updated: 2024/02/08 12:30:40 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	init_light(t_rt *rt, int *ids)
{
	int	i;

	i = 0;
	rt->lights = ft_calloc(ids[2] + 1, sizeof(t_light *));
	rt->n_lights = 0;
	while (i < (ids[2]))
	{
		rt->lights[i] = ft_calloc(1, sizeof(t_light));
		rt->lights[i]->coords = ft_calloc(3, sizeof(double));
		rt->lights[i++]->rgb = ft_calloc(3, sizeof(int));
	}
	rt->lights[i] = NULL;
}

int	check_format_light(char **split)
{
	if (!has_spec_count(split, 4))
		return (id_err("L", E_SPEC, "3"));
	if (!is_float_triad(split[1]))
		return (id_err("L", E_COORD, E_TRIAD_INTFLOAT));
	if (!triad_in_range(split[1]))
		return (id_err("L", E_COORD, E_MAX_DIGITS));
	if (!is_floatable(split[2]))
		return (id_err("L", E_BRIGHT, E_INTFLOAT));
	if (!float_in_range(split[2]))
		return (id_err("L", E_BRIGHT, E_MAX_DIGITS));
	if (!is_rgb_format(split[3]))
		return (id_err("L", E_RGB, E_TRIAD_INT));
	return (0);
}

int	fill_light(t_rt *rt, char **split)
{
	int			i;

	i = rt->n_lights;
	if (check_format_light(split))
		return (1);
	get_coords(rt->lights[i]->coords, split[1]);
	rt->lights[i]->brightness = ft_atof(split[2]);
	if (rt->lights[i]->brightness < 0.0 || rt->lights[i]->brightness > 1.0)
		return (id_err("L", E_BRIGHT_RANGE, E_RANGE_POS));
	if (!get_rgb(rt->lights[i]->rgb, split[3]))
		return (id_err("L", E_RGB_RANGE, E_RANGE_INT));
	rt->n_lights++;
	return (0);
}

void	free_light(t_rt *rt)
{
	int	i;

	i = 0;
	while (rt->lights[i] != NULL)
	{
			free(rt->lights[i]->coords);
			free(rt->lights[i]->rgb);
			free(rt->lights[i++]);
	}
	free(rt->lights);
}
