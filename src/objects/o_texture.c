/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   o_texture.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 09:47:14 by plouda            #+#    #+#             */
/*   Updated: 2024/03/04 18:34:05 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	init_textures(t_rt *rt, int *ids)
{
	int	i;

	i = 0;
	rt->textures = ft_calloc(ids[8] + 1, sizeof(t_texture *));
	rt->n_textures = 0;
	while (i < (ids[8]))
	{
		rt->textures[i] = ft_calloc(1, sizeof(t_texture));
		i++;
	}
	rt->textures[i] = NULL;
}

int	check_format_texture(char **split)
{
	int	i;
	int	spec_count;

	i = 1;
	spec_count = 0;
	if (!has_spec_count_strict(split, 4)
		&& !has_spec_count_strict(split, 3) && !has_spec_count_strict(split, 2))
		return (id_err(".tx", E_SPEC, "up to 3"));
	if (!has_valid_id_attribute(split[0]))
		return (id_err(".tx", "Invalid texture ID", NULL));
	while (split[spec_count])
		spec_count++;
	while (i < spec_count)
	{
		if (!contains_valid_key_value_pair(split[i]))
			return (id_err(".tx",
					"invalid key-value pair", "key{0,1,2}=\"value\""));
		if (!is_valid_texture_file(split[i]))
			return (id_err(".tx", "Invalid texture file", NULL));
		i++;
	}
	return (0);
}

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

void	ft_load_texture(t_texture *tx)
{
	if (tx->tx_path[0])
	{
		tx->tx_main = mlx_load_png(tx->tx_path);
		if (!tx->tx_main)
			id_warn(".tx", "Cannot read texture file under", tx->tx_path, 0);
	}
	if (tx->topcap_tx_path[0])
	{
		tx->tx_top = mlx_load_png(tx->topcap_tx_path);
		if (!tx->tx_top)
			id_warn(".tx", "Cannot read texture file under",
				tx->topcap_tx_path, 0);
	}
	if (tx->botcap_tx_path[0])
	{
		tx->tx_bot = mlx_load_png(tx->botcap_tx_path);
		if (!tx->tx_bot)
			id_warn(".tx", "Cannot read texture file under",
				tx->botcap_tx_path, 0);
	}
}

int	fill_texture(t_rt *rt, char **split)
{
	int			i;
	int			j;

	i = rt->n_textures;
	j = 1;
	if (check_format_texture(split))
		return (1);
	while (split[j])
	{
		set_texture_paths(rt->textures[i], split[j]);
		j++;
	}
	if (!set_texture_id(rt->textures[i], split[0]))
		return (id_err(".tx", "Invalid .tx specification", NULL));
	ft_load_texture(rt->textures[i]);
	rt->n_textures++;
	return (0);
}

void	free_textures(t_rt *rt)
{
	int	i;

	i = 0;
	while (rt->textures[i] != NULL)
	{
		if (rt->textures[i]->tx_main)
			mlx_delete_texture(rt->textures[i]->tx_main);
		if (rt->textures[i]->tx_top)
			mlx_delete_texture(rt->textures[i]->tx_top);
		if (rt->textures[i]->tx_bot)
			mlx_delete_texture(rt->textures[i]->tx_bot);
		free(rt->textures[i++]);
	}
	free(rt->textures);
}
