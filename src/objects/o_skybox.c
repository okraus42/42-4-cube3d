/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   o_skybox.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 12:34:18 by plouda            #+#    #+#             */
/*   Updated: 2024/03/07 14:54:30 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	init_skybox(t_rt *rt)
{
	rt->skybox = ft_calloc(1, sizeof(t_sphere));
	rt->skybox->coords = ft_calloc(3, sizeof(double));
	rt->skybox->rgb = ft_calloc(3, sizeof(int));
	rt->skybox->normal = ft_calloc(1, sizeof(t_vect3f));
	rt->skybox->right = ft_calloc(1, sizeof(t_vect3f));
	rt->skybox->up = ft_calloc(1, sizeof(t_vect3f));
	rt->skybox->glossiness = 0.00001;
	rt->skybox->radius = 0;
	rt->skybox->checkerboard = NULL;
	rt->skybox->texture = NULL;
	rt->skybox->vector_map = NULL;
	rt->skybox->mode = DEFAULT;
}

int	check_format_skybox(char **split)
{
	int	spec_count_flag;

	spec_count_flag = has_spec_count(split, 2);
	if (!spec_count_flag)
		return (id_err("SK", E_SPEC, "up to 4"));
	else if (spec_count_flag < 0)
		return (id_err("SK", "Invalid specifier format",
				"up to 3 unique texture identifiers after diameter"));
	if (!is_floatable(split[1]))
		return (id_err("SK", E_DIA, E_INTFLOAT));
	if (!float_in_range(split[1]))
		return (id_err("SK", E_DIA, E_MAX_DIGITS));
	return (0);
}

int	fill_skybox(t_rt *rt, char **split)
{
	double	diameter;

	if (check_format_skybox(split))
		return (1);
	diameter = ft_atof(split[1]);
	if (diameter <= 1000)
	{
		id_warn("SK", E_DIA_RANGE, "setting to min. value: 2000", 1);
		diameter = 2000;
	}
	rt->skybox->radius = diameter / 2.;
	*rt->skybox->normal = (t_vect3f){0, 0, 1};
	set_sphere_vects(rt->skybox);
	set_checkerboard_pointer("SK", rt, split, &rt->skybox->checkerboard);
	set_texture_pointer("SK", rt, split, &rt->skybox->texture);
	set_vector_map_pointer("SK", rt, split, &rt->skybox->vector_map);
	return (0);
}

void	free_skybox(t_rt *rt)
{
	free(rt->skybox->coords);
	free(rt->skybox->rgb);
	free(rt->skybox->normal);
	free(rt->skybox->right);
	free(rt->skybox->up);
	free(rt->skybox);
}
