/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_object_vectors_helpers.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 13:59:33 by plouda            #+#    #+#             */
/*   Updated: 2024/03/05 14:00:19 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	set_cone_vects_ptrs(t_cone *cone, t_vect33f *o)
{
	o->normal = cone->normal;
	o->right = cone->right;
	o->up = cone->up;
}

void	set_cone_cap_inversion(t_cone *cone)
{
	cone->pinnacle->is_inversed = NORMALDISC;
	cone->base->is_inversed = INVERSEDISC;
	cone->base->q = cone->q;
	cone->pinnacle->q = cone->q;
}

void	set_cylinder_vects_ptrs(t_cylinder *cylinder, t_vect33f *o)
{
	o->normal = cylinder->normal;
	o->right = cylinder->right;
	o->up = cylinder->up;
}

void	set_cylinder_cap_inversion(t_cylinder *cylinder)
{
	cylinder->topcap->is_inversed = NORMALDISC;
	cylinder->botcap->is_inversed = INVERSEDISC;
	cylinder->topcap->q = cylinder->q;
	cylinder->botcap->q = cylinder->q;
}
