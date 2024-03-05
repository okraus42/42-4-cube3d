/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   o_texture.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 09:47:14 by plouda            #+#    #+#             */
/*   Updated: 2024/03/05 13:42:25 by plouda           ###   ########.fr       */
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
