/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manipulate_light.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 12:15:05 by plouda            #+#    #+#             */
/*   Updated: 2024/03/05 12:27:19 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

static void	specular_options(t_master *master, keys_t key)
{
	double	highlight_size;

	highlight_size = master->options->spec_highlight_size;
	if (key == MLX_KEY_H)
	{
		highlight_size /= 2;
		if (highlight_size <= 2)
			highlight_size = 1024;
	}
	master->options->spec_highlight_size = highlight_size;
}

void	manipulate_light(t_master *master, mlx_key_data_t keydata)
{
	int	i;

	i = 0;
	while (i < master->rt->n_lights)
	{
		if (master->rt->light_spheres[i]->mode == HIGHLIGHT)
			break ;
		i++;
	}
	move(keydata.key, master->rt->camera, master->rt->light_spheres[i]->coords);
	specular_options(master, keydata.key);
	find_rays(master);
}
