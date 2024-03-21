/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   o_texture_id_path.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 13:40:14 by plouda            #+#    #+#             */
/*   Updated: 2024/03/05 13:42:28 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

int	set_texture_id(t_texture *texture, char *specifier)
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
	texture->id = ft_atoi(split[1]);
	ft_free_split(&split);
	return (flag);
}

int	set_texture_paths(t_texture *texture, char *str)
{
	char	**key_value_pair;
	char	*key;
	char	*value;
	char	*path;

	key_value_pair = ft_split(str, '=');
	key = key_value_pair[0];
	value = key_value_pair[1];
	path = ft_strtrim(value, "\"");
	if (key[0] == '0' && !texture->tx_path[0])
		ft_strlcpy(texture->tx_path, path, ft_strlen(path) + 1);
	else if (key[0] == '1' && !texture->botcap_tx_path[0])
		ft_strlcpy(texture->botcap_tx_path, path, ft_strlen(path) + 1);
	else if (key[0] == '2' && !texture->topcap_tx_path[0])
		ft_strlcpy(texture->topcap_tx_path, path, ft_strlen(path) + 1);
	ft_free_split(&key_value_pair);
	free(path);
	return (1);
}
