/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   o_cone_disc_texture.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 13:04:31 by plouda            #+#    #+#             */
/*   Updated: 2024/03/05 13:14:19 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

// only considers botcap, so base in this case (we don't care about pinnacle)
void	attribute_texture_data_cone(t_disc *disc, t_cone *cone)
{
	disc->checkerboard = cone->checkerboard;
	disc->texture = cone->texture;
	if (disc->texture)
		disc->tx_disc = cone->texture->tx_bot;
	else
		disc->tx_disc = NULL;
	disc->vector_map = cone->vector_map;
	if (disc->vector_map)
		disc->vm_disc = cone->vector_map->vm_bot;
	else
		disc->vm_disc = NULL;
}
