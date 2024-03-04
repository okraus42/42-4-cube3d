/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_texture_data_pointers.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 13:30:20 by plouda            #+#    #+#             */
/*   Updated: 2024/03/04 17:29:30 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

static int	specifier_parsing_check(int i, int len, int *id, char **split)
{
	char	**specifier;

	specifier = ft_split(split[i], '/');
	while (specifier[1][len])
	{
		if (len > 3 || !ft_isdigit(specifier[1][len]))
		{
			ft_free_split(&specifier);
			return (-2);
		}
		len++;
	}
	*id = ft_atoi(specifier[1]);
	ft_free_split(&specifier);
	return (0);
}

static int	find_checkerboard_id(char **split)
{
	int		i;
	int		id;
	int		len;

	i = 0;
	id = -1;
	len = 0;
	while (split[i])
	{
		if (!ft_strncmp(split[i], ".ch/", 4))
			if (specifier_parsing_check(i, len, &id, split) < 0)
				return (-2);
		i++;
	}
	return (id);
}

int	set_checkerboard_pointer(char *obj,
	t_rt *rt, char **split, t_checkerboard **ptr)
{
	int	id;
	int	i;

	i = 0;
	id = find_checkerboard_id(split);
	if (id == -2)
		return (id_warn(obj, W_CHECKER_ID, W_CHECKER_CONT, 1));
	if (id == -1)
		return (0);
	while (i < rt->n_checkerboards)
	{
		if (rt->checkerboards[i]->id == id)
		{
			*ptr = rt->checkerboards[i];
			return (1);
		}
		i++;
	}
	return (id_warn(obj, W_CHECKER_ID, W_CHECKER_CONT, 1));
}

static int	find_texture_id(char **split)
{
	int		i;
	int		id;
	int		len;

	i = 0;
	id = -1;
	len = 0;
	while (split[i])
	{
		if (!ft_strncmp(split[i], ".tx/", 4))
			if (specifier_parsing_check(i, len, &id, split) < 0)
				return (-2);
		i++;
	}
	return (id);
}

int	set_texture_pointer(char *obj, t_rt *rt, char **split, t_texture **ptr)
{
	int	id;
	int	i;

	i = 0;
	id = find_texture_id(split);
	if (id == -2)
		return (id_warn(obj, W_TEXT_ID, W_TEXT_CONT, 1));
	if (id == -1)
		return (0);
	while (i < rt->n_textures)
	{
		if (rt->textures[i]->id == id)
		{
			*ptr = rt->textures[i];
			return (1);
		}
		i++;
	}
	return (id_warn(obj, W_TEXT_ID, W_TEXT_CONT, 1));
}

static int	find_vector_map_id(char **split)
{
	int		i;
	int		id;
	int		len;

	i = 0;
	id = -1;
	len = 0;
	while (split[i])
	{
		if (!ft_strncmp(split[i], ".vm/", 4))
			if (specifier_parsing_check(i, len, &id, split) < 0)
				return (-2);
		i++;
	}
	return (id);
}

int	set_vector_map_pointer(char *obj,
	t_rt *rt, char **split, t_vector_map **ptr)
{
	int	id;
	int	i;

	i = 0;
	id = find_vector_map_id(split);
	if (id == -2)
		return (id_warn(obj, W_VMAP_ID, W_VMAP_CONT, 1));
	if (id == -1)
		return (0);
	while (i < rt->n_vector_maps)
	{
		if (rt->vector_maps[i]->id == id)
		{
			*ptr = rt->vector_maps[i];
			return (1);
		}
		i++;
	}
	return (id_warn(obj, W_VMAP_ID, W_VMAP_CONT, 1));
}
