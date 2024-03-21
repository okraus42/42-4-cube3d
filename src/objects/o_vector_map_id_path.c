/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   o_vector_map_id_path.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 13:44:54 by plouda            #+#    #+#             */
/*   Updated: 2024/03/05 13:46:36 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

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
