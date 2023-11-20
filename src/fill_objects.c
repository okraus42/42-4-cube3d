/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_data_from_line.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 09:18:20 by plouda            #+#    #+#             */
/*   Updated: 2023/11/20 13:22:22 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minirt.h"

int	fill_ambient(t_rt *rt, char **split)
{
	if (check_format_ambient(split))
		return (1);
	rt->ambient->ratio = ft_atof(split[1]);
	if (rt->ambient->ratio < 0.0 || rt->ambient->ratio > 1.0)
		return (id_err("A", E_RATIO_RANGE, E_RANGE_POS));
	if (!get_rgb(rt->ambient->rgb, split[2]))
		return (id_err("A", E_RGB_RANGE, E_RANGE_INT));
	return (0);
}

int	fill_camera(t_rt *rt, char **split)
{
	if (check_format_camera(split))
		return (1);
	get_coords(rt->camera->coords, split[1]);
	if (!get_nvect(rt->camera->nvect, split[2]))
		return (id_err("C", E_VECT_RANGE, E_RANGE_NORM));
	rt->camera->fov = ft_atoi(split[3]);
	if (rt->camera->fov < 0 || rt->camera->fov > 180)
		return (id_err("C", E_FOV_RANGE, "a value in range (0;180)"));
	return (0);
}

int	fill_light(t_rt *rt, char **split)
{
	if (check_format_light(split))
		return (1);
	get_coords(rt->light->coords, split[1]);
	rt->light->brightness = ft_atof(split[2]);
	if (rt->light->brightness < 0.0 || rt->light->brightness > 1.0)
		return (id_err("L", E_BRIGHT_RANGE, E_RANGE_POS));
	if (!get_rgb(rt->light->rgb, split[3]))
		return (id_err("L", E_RGB_RANGE, E_RANGE_INT));
	return (0);
}

int	fill_sphere(t_rt *rt, char **split)
{
	int			i;

	i = rt->n_spheres;
	if (check_format_sphere(split))
		return (1);
	get_coords(rt->spheres[i]->coords, split[1]);
	rt->spheres[i]->diameter = ft_atof(split[2]);
	if (rt->spheres[i]->diameter <= 0)
		return (id_err("sp", E_DIA_RANGE, E_RANGE_STRICT));
	if (!get_rgb(rt->spheres[i]->rgb, split[3]))
		return (id_err("sp", E_RGB_RANGE, E_RANGE_INT));
	rt->n_spheres++;
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
	rt->n_cylinders++;
	return (0);
}

void	fill_objects(t_rt *rt, char **split, int *flag)
{
	if (ft_strlen(split[0]) == 1)
	{
		if (!ft_strncmp(split[0], "A", 1))
			*flag = fill_ambient(rt, split);
		else if (!ft_strncmp(split[0], "C", 1))
			*flag = fill_camera(rt, split);
		else if (!ft_strncmp(split[0], "L", 1))
			*flag = fill_light(rt, split);
		else
			*flag = 1;
	}
	else if (ft_strlen(split[0]) == 2)
	{
		if (!ft_strncmp(split[0], "sp", 2))
			*flag = fill_sphere(rt, split);
		else if (!ft_strncmp(split[0], "pl", 2))
			*flag = fill_plane(rt, split);
		else if (!ft_strncmp(split[0], "cy", 2))
			*flag = fill_cylinder(rt, split);
		else
			*flag = 1;
	}
	else
		*flag = 1;
}

