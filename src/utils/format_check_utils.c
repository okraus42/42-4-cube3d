/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_check_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 09:13:21 by plouda            #+#    #+#             */
/*   Updated: 2024/03/02 16:15:07 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

static int	iter_float_triad(char *str, int *i, int *j, int flag)
{
	if (str[*i] == '-')
		(*i)++;
	if (!ft_isdigit(str[*i]))
		flag++;
	while (str[*i] && ft_isdigit(str[*i]))
		(*i)++;
	if (str[*i] == '.')
		(*i)++;
	if (!ft_isdigit(str[*i]) && flag)
		return (0);
	while (str[*i] && ft_isdigit(str[*i]))
		(*i)++;
	if ((*j) != 2)
	{
		if (str[(*i)++] != ',')
			return (0);
	}
	else
	{
		if (str[*i])
			return (0);
	}
	(*j)++;
	return (1);
}

static int	iter_int_triad(char *str, int *i, int *j)
{
	if (str[*i] == '-')
		(*i)++;
	if (!ft_isdigit(str[*i]))
		return (0);
	while (str[*i] && ft_isdigit(str[*i]))
		(*i)++;
	if (*j != 2)
	{
		if (str[(*i)++] != ',')
			return (0);
	}
	else
	{
		if (str[*i])
			return (0);
	}
	(*j)++;
	return (1);
}

int	is_float_triad(char *str)
{
	int	i;
	int	j;
	int	flag;

	i = 0;
	j = 0;
	while (j < 3 && str[i])
	{
		flag = 0;
		if (!iter_float_triad(str, &i, &j, flag))
			return (0);
	}
	if (!str[i] && j < 3)
		return (0);
	return (1);
}

int	is_rgb_format(char *str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (j < 3 && str[i])
	{
		if (!iter_int_triad(str, &i, &j))
			return (0);
	}
	if (!str[i] && j < 3)
		return (0);
	return (1);
}

int	has_spec_count(char **split, int target)
{
	int	num;
	int	i;
	int	ch_flag;
	int	tx_flag;
	int	vm_flag;

	num = 0;
	i = 0;
	ch_flag = 0;
	tx_flag = 0;
	vm_flag = 0;
	while (split[num])
		num++;
	if (num > target + 3)
		return (0);
	else if (num > target && num <= target + 3)
	{
		i = target;
		while (i < num && split[i])
		{
			if (!ft_strncmp(split[i], ".ch/", 4) && !ch_flag)
				ch_flag++;
			else if (!ft_strncmp(split[i], ".tx/", 4) && !tx_flag)
				tx_flag++;
			else if (!ft_strncmp(split[i], ".vm/", 4) && !vm_flag)
				vm_flag++;
			else
				return (-1);
			i++;
		}
	}
	else if (num < target)
		return (0);
	return (1);
}

int	has_spec_count_strict(char **split, int target)
{
	int	num;

	num = 0;
	while (split[num])
		num++;
	if (num != target)
		return (0);
	return (1);
}

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
	i = 0;
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

int	is_valid_texture_file(char *str)
{
	int		i;
	char	**key_value_pair;
	char	*value;
	char	*path;
	int		fd;

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
	path = ft_strtrim(value, "\"");
	if (ft_strlen(path) > 1024)
		return (0);
	fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		free(path);
		ft_free_split(&key_value_pair);
		return (0);
	}
	free(path);
	ft_free_split(&key_value_pair);
	close (fd);
	return (1);
}
