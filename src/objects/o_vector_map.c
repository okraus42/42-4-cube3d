/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   o_vector_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 09:47:41 by plouda            #+#    #+#             */
/*   Updated: 2024/02/23 10:11:25 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	init_vector_maps(t_rt *rt, int *ids)
{
	int	i;

	i = 0;
	rt->vector_maps = ft_calloc(ids[9] + 1, sizeof(t_vector_map *));
	rt->n_vector_maps = 0;
	while (i < (ids[9]))
	{
		rt->vector_maps[i] = ft_calloc(1, sizeof(t_vector_map));
		//rt->vector_maps[i]->tx_path = NULL;
		// ft_bzero(rt->vector_maps[i]->tx_path, 1024);
		// ft_bzero(rt->vector_maps[i]->botcap_tx_path, 1024);
		// ft_bzero(rt->vector_maps[i]->topcap_tx_path, 1024);
		i++;
	}
	rt->vector_maps[i] = NULL;
}

int	check_format_vector_map(char **split)
{
	int	i;
	int	spec_count;

	i = 1;
	spec_count = 0;
	if (!has_spec_count_strict(split, 4) && !has_spec_count_strict(split, 3) && !has_spec_count_strict(split, 2))
		return (id_err(".vm", E_SPEC, "up to 3"));
	if (!has_valid_id_attribute(split[0]))
		return (id_err(".vm", "Invalid vector_map ID", NULL));
	while (split[spec_count])
		spec_count++;
	while (i < spec_count)
	{
		if (!contains_valid_key_value_pair(split[i]))
			return (id_err(".vm", "invalid key-value pair", "key{0,1,2}=\"value\""));
		if (!is_valid_texture_file(split[i]))
			return (id_err(".vm", "Invalid vector_map file", NULL));
		i++;
	}
	return (0);
}

int	set_vector_map_id(t_vector_map *vector_map, char *specifier)
{
	char	**split;
	int		i;
	int		flag;

	i = 0;
	flag = 0;
	while (specifier[i])
	{
		if (specifier[i] == '/')
			flag = 1;
		i++;
	}
	if (!flag)
		return (flag);
	split = ft_split(specifier, '/');
	vector_map->id = ft_atoi(split[1]);
	ft_free_split(&split);
	return (flag);
}

int	set_vector_map_paths(t_vector_map *vector_map, char *str)
{
	char	**key_value_pair;
	char	*key;
	char	*value;
	char	*path;

	key_value_pair = ft_split(str, '=');
	key = key_value_pair[0];
	value = key_value_pair[1];
	path = ft_strtrim(value, "\"");
	if (key[0] == '0' && !vector_map->vm_path[0])
	{
		ft_strlcpy(vector_map->vm_path, path, ft_strlen(path) + 1);
	}
	else if (key[0] == '1' && !vector_map->botcap_vm_path[0])
		ft_strlcpy(vector_map->botcap_vm_path, path, ft_strlen(path) + 1);
	else if (key[0] == '2' && !vector_map->topcap_vm_path[0])
		ft_strlcpy(vector_map->topcap_vm_path, path, ft_strlen(path) + 1);
	ft_free_split(&key_value_pair);
	free(path);
	return (1);
}

void	ft_load_vector_map(t_vector_map *vm)
{
	if (vm->vm_path[0])
	{
		if (access(vm->vm_path, R_OK) < 0)
		{
			dprintf(2, "Cannot open texture %s\n", vm->vm_path);
			exit(-3);
		}
		vm->vm_main = mlx_load_png(vm->vm_path);
		if (!vm->vm_main)
		{
			dprintf(2, "Error loading texture %s\n", vm->vm_path);
			exit(-4);
		}
	}
	if (vm->topcap_vm_path[0])
	{
		if (access(vm->topcap_vm_path, R_OK) < 0)
		{
			dprintf(2, "Cannot open texture %s\n", vm->topcap_vm_path);
			exit(-3);
		}
		vm->vm_top = mlx_load_png(vm->topcap_vm_path);
		if (!vm->vm_top)
		{
			dprintf(2, "Error loading texture %s\n", vm->topcap_vm_path);
			exit(-4);
		}
	}
	if (vm->botcap_vm_path[0])
	{
		if (access(vm->botcap_vm_path, R_OK) < 0)
		{
			dprintf(2, "Cannot open texture %s\n", vm->botcap_vm_path);
			exit(-3);
		}
		vm->vm_bot = mlx_load_png(vm->botcap_vm_path);
		if (!vm->vm_bot)
		{
			dprintf(2, "Error loading texture %s\n", vm->botcap_vm_path);
			exit(-4);
		}
	}
}

int	fill_vector_map(t_rt *rt, char **split)
{
	int			i;
	int			j;

	i = rt->n_vector_maps;
	j = 1;
	if (check_format_vector_map(split))
		return (1);
	while (split[j])
	{
		set_vector_map_paths(rt->vector_maps[i], split[j]);
		j++;
	}
	if (!set_vector_map_id(rt->vector_maps[i], split[0]))
		return (id_err(".vm", "Invalid .vm specification", NULL));
	// ft_putstr_fd(rt->vector_maps[i]->vm_path, 1);
	// write(1, "\n", 1);
	// ft_putstr_fd(rt->vector_maps[i]->botcap_vm_path, 1);
	// write(1, "\n", 1);
	// ft_putstr_fd(rt->vector_maps[i]->topcap_vm_path, 1);
	// write(1, "\n", 1);
	ft_load_vector_map(rt->vector_maps[i]);
	//printf("vm->botcap_vm_path %s\n", rt->vector_maps[i]->topcap_vm_path);
	rt->n_vector_maps++;
	return (0);
}

void	free_vector_maps(t_rt *rt)
{
	int	i;

	i = 0;
	while (rt->vector_maps[i] != NULL)
	{
		free(rt->vector_maps[i++]);
	}
	free(rt->vector_maps);
}

