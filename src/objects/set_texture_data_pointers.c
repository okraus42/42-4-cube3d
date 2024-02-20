/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_texture_data_pointers.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 13:30:20 by plouda            #+#    #+#             */
/*   Updated: 2024/02/20 11:25:01 by plouda           ###   ########.fr       */
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
			if (len > 9 || !ft_isdigit(specifier[1][len]))
			{
				ft_free_split(&specifier);
				return (id);
			}
			len++;
		}
		id = ft_atoi(specifier[1]);
		ft_free_split(&specifier);
	}
	return (id);
}

void	set_checkerboard_pointer(t_rt *rt, char **split, t_checkerboard **ptr)
{
	int	id;
	int	i;

	i = 0;
	id = find_checkerboard_id(split);
	if (id < 0) // can this even happen?
		return ;
	while (i < rt->n_checkerboards)
	{
		if (rt->checkerboards[i]->id == id)
		{
			*ptr = rt->checkerboards[i];
			return ;
		}
		i++;
	}
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
			if (len > 9 || !ft_isdigit(specifier[1][len]))
			{
				ft_free_split(&specifier);
				return (id);
			}
			len++;
		}
		id = ft_atoi(specifier[1]);
		ft_free_split(&specifier);
	}
	return (id);
}

void	set_texture_pointer(t_rt *rt, char **split, t_texture **ptr)
{
	int	id;
	int	i;

	i = 0;
	id = find_texture_id(split);
	if (id < 0) // can this even happen?
		return ;
	while (i < rt->n_textures)
	{
		if (rt->textures[i]->id == id)
		{
			*ptr = rt->textures[i];
			return ;
		}
		i++;
	}
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
			if (len > 9 || !ft_isdigit(specifier[1][len]))
			{
				ft_free_split(&specifier);
				return (id);
			}
			len++;
		}
		id = ft_atoi(specifier[1]);
		ft_free_split(&specifier);
	}
	return (id);
}

void	set_vector_map_pointer(t_rt *rt, char **split, t_vector_map **ptr)
{
	int	id;
	int	i;

	i = 0;
	id = find_vector_map_id(split);
	if (id < 0) // can this even happen?
		return ;
	while (i < rt->n_vector_maps)
	{
		if (rt->vector_maps[i]->id == id)
		{
			*ptr = rt->vector_maps[i];
			return ;
		}
		i++;
	}
}
