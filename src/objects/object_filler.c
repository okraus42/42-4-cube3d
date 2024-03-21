/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_filler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 13:54:24 by plouda            #+#    #+#             */
/*   Updated: 2024/03/07 14:54:38 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

static void	fill_objects_1(t_rt *rt, char **split, int *flag)
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

static void	fill_objects_2(t_rt *rt, char **split, int *flag)
{
	if (!ft_strncmp(split[0], "sp", 2))
		*flag = fill_sphere(rt, split);
	else if (!ft_strncmp(split[0], "pl", 2))
		*flag = fill_plane(rt, split);
	else if (!ft_strncmp(split[0], "cy", 2))
		*flag = fill_cylinder(rt, split);
	else if (!ft_strncmp(split[0], "co", 2))
		*flag = fill_cone(rt, split);
	else if (!ft_strncmp(split[0], "SK", 2))
		*flag = fill_skybox(rt, split);
	else
		*flag = 1;
}

void	fill_objects(t_rt *rt, char **split, int *flag)
{
	if (ft_strlen(split[0]) == 1)
		fill_objects_1(rt, split, flag);
	else if (ft_strlen(split[0]) == 2)
		fill_objects_2(rt, split, flag);
	else if (!ft_strncmp(split[0], ".ch/", 4))
		return ;
	else if (!ft_strncmp(split[0], ".tx/", 4))
		return ;
	else if (!ft_strncmp(split[0], ".vm/", 4))
		return ;
	else
		*flag = 1;
}
