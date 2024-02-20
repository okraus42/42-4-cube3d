/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 15:51:52 by plouda            #+#    #+#             */
/*   Updated: 2024/02/20 09:26:55 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

int	*init_ids(void)
{
	int		*ids;
	int		i;

	i = 0;
	ids = malloc(sizeof(int) * 10);
	while (i < 10)
		ids[i++] = 0;
	return (ids);
}

int	check_identifiers(int fd, int *ids, int *flag)
{
	char	*line;

	line = get_next_line(fd);
	while (line && !(*flag))
	{
		count_identifiers(line, ids, flag);
		check_duplicates(ids, flag);
		free(line);
		line = get_next_line(fd);
	}
	if (!*flag)
		check_missing(ids, flag);
	free(line);
	return (0);
}

void	init_objects(t_rt *rt, int *ids)
{
	init_ambient(rt);
	init_light_sphere(rt, ids);
	init_camera(rt);
	init_spheres(rt, ids);
	init_planes(rt, ids);
	init_cylinders(rt, ids);
	init_cones(rt, ids);
	init_checkerboards(rt, ids);
	init_textures(rt, ids);
	init_vector_maps(rt, ids);
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
			*flag = fill_light_sphere(rt, split);
		else if (!ft_strncmp(split[0], "#", 1))
			return ;
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
		else if (!ft_strncmp(split[0], "co", 2))
			*flag = fill_cone(rt, split);
		else
			*flag = 1;
	}
 	else if (!ft_strncmp(split[0], ".ch/", 4)) // ignoring checkerboard call during object parsing
		return ;
	else if (!ft_strncmp(split[0], ".tx/", 4)) // ignoring texture call during object parsing
		return ;
	else if (!ft_strncmp(split[0], ".vm/", 4)) // ignoring vector map call during object parsing
		return ;
	else
		*flag = 1;
}

void	free_objects(t_rt *rt)
{
	free_ambient(rt);
	free_camera(rt);
	free_spheres(rt);
	free_planes(rt);
	free_cylinders(rt);
	free_cones(rt);
	free_light_sphere(rt);
	free_checkerboards(rt);
}
