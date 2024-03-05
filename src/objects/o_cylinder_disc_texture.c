/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   o_cylinder_disc_texture.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 13:11:47 by plouda            #+#    #+#             */
/*   Updated: 2024/03/05 13:12:18 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	attribute_texture_data_cylinder_bot(t_disc *disc, t_cylinder *cylinder)
{
	disc->checkerboard = cylinder->checkerboard;
	disc->texture = cylinder->texture;
	if (disc->texture)
		disc->tx_disc = cylinder->texture->tx_bot;
	else
		disc->tx_disc = NULL;
	disc->vector_map = cylinder->vector_map;
	if (disc->vector_map)
		disc->vm_disc = cylinder->vector_map->vm_bot;
	else
		disc->vm_disc = NULL;
}

void	attribute_texture_data_cylinder_top(t_disc *disc, t_cylinder *cylinder)
{
	disc->checkerboard = cylinder->checkerboard;
	disc->texture = cylinder->texture;
	if (disc->texture)
		disc->tx_disc = cylinder->texture->tx_top;
	else
		disc->tx_disc = NULL;
	disc->vector_map = cylinder->vector_map;
	if (disc->vector_map)
		disc->vm_disc = cylinder->vector_map->vm_top;
	else
		disc->vm_disc = NULL;
}
