/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_string_in_window.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 10:46:15 by plouda            #+#    #+#             */
/*   Updated: 2024/03/05 12:42:38 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

static int	iterate_highlighted_object2(t_rt *rt)
{
	int	i;

	i = 0;
	while (i < rt->n_spheres)
	{
		if (rt->spheres[i]->mode == HIGHLIGHT)
			return (SPHERE | (i << 8));
		i++;
	}
	i = 0;
	while (i < rt->n_planes)
	{
		if (rt->planes[i]->mode == HIGHLIGHT)
			return (PLANE | (i << 8));
		i++;
	}
	i = 0;
	while (i < rt->n_cylinders)
	{
		if (rt->cylinders[i]->mode == HIGHLIGHT)
			return (CYLINDER | (i << 8));
		i++;
	}
	return (0);
}

static int	iterate_highlighted_object(t_rt *rt)
{
	int	i;

	i = iterate_highlighted_object2(rt);
	if (i)
		return (i);
	while (i < rt->n_cones)
	{
		if (rt->cones[i]->mode == HIGHLIGHT)
			return (CONE | (i << 8));
		i++;
	}
	i = 0;
	while (i < rt->n_lights)
	{
		if (rt->light_spheres[i]->mode == HIGHLIGHT)
			return (LIGHT | (i << 8));
		i++;
	}
	return (0);
}

static void	ft_draw_string2(t_master *master, char **s)
{
	if (master->options->mode == DEFAULT)
		*s = ft_strdup("MODE: DEFAULT");
	else if (master->options->mode == OBJECT_CHOICE)
		*s = ft_strdup("MODE: OBJECT_CHOICE");
	else if (master->options->mode == HIGHLIGHT)
		*s = ft_strdup("MODE: HIGHLIGHT");
	else if (master->options->mode == LIGHTING)
		*s = ft_strdup("MODE: LIGHTING");
	else if (master->options->mode == CAMERA)
		*s = ft_strdup("MODE: CAMERA");
	else
		*s = ft_strdup("UNKNOWN MODE");
}

void	ft_draw_string(t_master *master)
{
	int		i;
	char	*s[STRINGS];

	ft_draw_string2(master, &s[0]);
	i = iterate_highlighted_object(master->rt);
	if ((i & 0xFF) == SPHERE)
		ft_sprintf(master->str[1], "HIGHLIGHTED: SPHERE: %i", i >> 8);
	else if ((i & 0xFF) == PLANE)
		ft_sprintf(master->str[1], "HIGHLIGHTED: PLANE: %i", i >> 8);
	else if ((i & 0xFF) == CYLINDER)
		ft_sprintf(master->str[1], "HIGHLIGHTED: CYLINDER: %i", i >> 8);
	else if ((i & 0xFF) == CONE)
		ft_sprintf(master->str[1], "HIGHLIGHTED: CONE: %i", i >> 8);
	else if ((i & 0xFF) == LIGHT)
		ft_sprintf(master->str[1], "HIGHLIGHTED: LIGHT: %i", i >> 8);
	else
		ft_sprintf(master->str[1], " ");
	mlx_delete_image(master->mlx, master->string[0]);
	mlx_delete_image(master->mlx, master->string[1]);
	master->string[0] = mlx_put_string(master->mlx, s[0], 10, 5);
	master->string[1] = mlx_put_string(master->mlx, master->str[1], 10, 25);
	free (s[0]);
}
