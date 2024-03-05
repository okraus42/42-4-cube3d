/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_format_checks.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 15:02:42 by plouda            #+#    #+#             */
/*   Updated: 2024/03/05 15:05:20 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

int	has_valid_id_attribute(char *str)
{
	int	i;

	i = 4;
	while (i < (int)ft_strlen(str))
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	contains_valid_key_value_pair_helper(char **key_value_pair)
{
	if (key_value_pair[0] && (ft_strncmp("0", key_value_pair[0],
				ft_strlen(key_value_pair[0]))
			&& ft_strncmp("1", key_value_pair[0],
				ft_strlen(key_value_pair[0]))
			&& ft_strncmp("2", key_value_pair[0],
				ft_strlen(key_value_pair[0]))))
	{
		ft_free_split(&key_value_pair);
		return (0);
	}
	if (key_value_pair[1])
	{
		ft_free_split(&key_value_pair);
		return (1);
	}
	else
	{
		ft_free_split(&key_value_pair);
		return (0);
	}
}

int	contains_valid_key_value_pair(char *str)
{
	int		i;
	char	**key_value_pair;

	i = 0;
	key_value_pair = ft_split(str, '=');
	while (key_value_pair[i])
		i++;
	if (i != 2)
	{
		ft_free_split(&key_value_pair);
		return (0);
	}
	return (contains_valid_key_value_pair_helper(key_value_pair));
}

static int	is_valid_texture_file_helper(char *value, char ***key_value_pair)
{
	char	*path;
	int		fd;

	path = ft_strtrim(value, "\"");
	if (ft_strlen(path) > 1024)
		return (0);
	fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		free(path);
		ft_free_split(key_value_pair);
		return (0);
	}
	free(path);
	close (fd);
	return (1);
}

int	is_valid_texture_file(char *str)
{
	int		i;
	char	**key_value_pair;
	char	*value;

	i = 0;
	key_value_pair = ft_split(str, '=');
	value = key_value_pair[1];
	while (key_value_pair[i])
		i++;
	if (i > 2)
	{
		ft_free_split(&key_value_pair);
		return (0);
	}
	if (!is_valid_texture_file_helper(value, &key_value_pair))
		return (0);
	ft_free_split(&key_value_pair);
	return (1);
}

