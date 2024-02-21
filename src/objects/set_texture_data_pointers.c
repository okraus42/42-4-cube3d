/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_texture_data_pointers.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 13:30:20 by plouda            #+#    #+#             */
/*   Updated: 2024/02/21 11:42:42 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

static int	find_checkerboard_id(char **split)
{
	int	i;
	int	id;
	int	len;
	char	**specifier;

	i = 0;
	id = -1;
	len = 0;
	while (split[i])
		i++;
	if (!ft_strncmp(split[i-1], ".ch/", 4))
	{
		specifier = ft_split(split[i-1], '/');
		while (specifier[1][len])
		{
			if (len > 3 || !ft_isdigit(specifier[1][len]))
			{
				ft_free_split(&specifier);
				return (-2);
			}
			len++;
		}
		id = ft_atoi(specifier[1]);
		ft_free_split(&specifier);
	}
	return (id);
}

int	set_checkerboard_pointer(char *obj, t_rt *rt, char **split, t_checkerboard **ptr)
{
	int	id;
	int	i;

	i = 0;
	id = find_checkerboard_id(split);
	if (id == -2)
		return (id_warn(obj, W_CHECKER_ID, W_CHECKER_CONT));
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
	return (id_warn(obj, W_CHECKER_ID, W_CHECKER_CONT));
}

static int	find_texture_id(char **split)
{
	int	i;
	int	id;
	int	len;
	char	**specifier;

	i = 0;
	id = -1;
	len = 0;
	while (split[i])
		i++;
	if (!ft_strncmp(split[i-1], ".tx/", 4))
	{
		specifier = ft_split(split[i-1], '/');
		while (specifier[1][len])
		{
			if (len > 3 || !ft_isdigit(specifier[1][len]))
			{
				ft_free_split(&specifier);
				return (-2);
			}
			len++;
		}
		id = ft_atoi(specifier[1]);
		ft_free_split(&specifier);
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
		return (id_warn(obj, W_TEXT_ID, W_TEXT_CONT));
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
	return (id_warn(obj, W_TEXT_ID, W_TEXT_CONT));
}

static int	find_vector_map_id(char **split)
{
	int	i;
	int	id;
	int	len;
	char	**specifier;

	i = 0;
	id = -1;
	len = 0;
	while (split[i])
		i++;
	if (!ft_strncmp(split[i-1], ".vm/", 4))
	{
		specifier = ft_split(split[i-1], '/');
		while (specifier[1][len])
		{
			if (len > 3 || !ft_isdigit(specifier[1][len]))
			{
				ft_free_split(&specifier);
				return (-2);
			}
			len++;
		}
		id = ft_atoi(specifier[1]);
		ft_free_split(&specifier);
	}
	return (id);
}

int	set_vector_map_pointer(char *obj, t_rt *rt, char **split, t_vector_map **ptr)
{
	int	id;
	int	i;

	i = 0;
	id = find_vector_map_id(split);
	if (id == -2)
		return (id_warn(obj, W_VMAP_ID, W_VMAP_CONT));
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
	return (id_warn(obj, W_VMAP_ID, W_VMAP_CONT));
}
