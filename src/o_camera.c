/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   o_camera.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 15:37:54 by plouda            #+#    #+#             */
/*   Updated: 2023/12/14 17:58:12 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minirt.h"

void	init_camera(t_rt *rt)
{
	rt->camera = ft_calloc(1, sizeof(t_camera));
	rt->camera->coords = ft_calloc(3, sizeof(double));
	rt->camera->nvect = ft_calloc(3, sizeof(double));
	rt->camera->normal = ft_calloc(1, sizeof(t_vect3f));
	rt->camera->right = ft_calloc(1, sizeof(t_vect3f));
	rt->camera->up = ft_calloc(1, sizeof(t_vect3f));
	rt->camera->matrix = ft_calloc(4, sizeof(double *));
	rt->camera->matrix[0] = ft_calloc(4, sizeof(double));
	rt->camera->matrix[1] = ft_calloc(4, sizeof(double));
	rt->camera->matrix[2] = ft_calloc(4, sizeof(double));
	rt->camera->matrix[3] = ft_calloc(4, sizeof(double));
}

int	check_format_camera(char **split)
{
	if (!has_spec_count(split, 4))
		return (id_err("C", E_SPEC, "3"));
	if (!is_float_triad(split[1]))
		return (id_err("C", E_COORD, E_TRIAD_INTFLOAT));
	if (!triad_in_range(split[1]))
		return (id_err("C", E_COORD, E_MAX_DIGITS));
	if (!is_float_triad(split[2]))
		return (id_err("C", E_VECT, E_TRIAD_INTFLOAT));
	if (!triad_in_range(split[2]))
		return (id_err("C", E_VECT, E_MAX_DIGITS));
	if (!is_integer(split[3]))
		return (id_err("C", E_FOV, "an integer"));
	return (0);
}

int	fill_camera(t_rt *rt, char **split)
{
	if (check_format_camera(split))
		return (1);
	get_coords(rt->camera->coords, split[1]);
	if (!get_nvect(rt->camera->nvect, split[2]))
		return (id_err("C", E_VECT_RANGE, E_RANGE_NORM));
	*rt->camera->normal = get_normal(rt->camera->nvect[X], \
		rt->camera->nvect[Y], rt->camera->nvect[Z]);
	rt->camera->fov = ft_atoi(split[3]);
	if (rt->camera->fov < 0 || rt->camera->fov > 180)
		return (id_err("C", E_FOV_RANGE, "a value in range (0;180)"));
	set_camera(rt->camera);
	return (0);
}

void	free_camera(t_rt *rt)
{
	free(rt->camera->coords);
	free(rt->camera->nvect);
	free(rt->camera->normal);
	free(rt->camera->right);
	free(rt->camera->up);
	free(rt->camera->matrix[0]);
	free(rt->camera->matrix[1]);
	free(rt->camera->matrix[2]);
	free(rt->camera->matrix[3]);
	free(rt->camera->matrix);
	free(rt->camera);
}
