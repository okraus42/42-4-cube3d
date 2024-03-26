/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_modes_cylinder_cone.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 12:02:25 by plouda            #+#    #+#             */
/*   Updated: 2024/03/05 12:43:00 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

int	change_modes_cone(t_cone *cone, t_mode mode)
{
	cone->mode = mode;
	cone->base->mode = mode;
	cone->pinnacle->mode = mode;
	return (0);
}

int	change_modes_cylinder(t_cylinder *cylinder, t_mode mode)
{
	cylinder->mode = mode;
	cylinder->botcap->mode = mode;
	cylinder->topcap->mode = mode;
	return (0);
}
