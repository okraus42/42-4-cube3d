/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   disc_or_plane.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 15:30:46 by plouda            #+#    #+#             */
/*   Updated: 2024/03/05 15:32:11 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	disc_or_plane(void *object,
	t_vect3f *pt, t_vect3f *normal, t_object f)
{
	t_disc		*disc;
	t_plane		*plane;

	disc = NULL;
	plane = NULL;
	if (f == DISC)
	{
		disc = (t_disc *)object;
		*pt = array_to_vect(disc->coords);
		*normal = *disc->normal;
	}
	else
	{
		plane = (t_plane *)object;
		*pt = array_to_vect(plane->coords);
		*normal = *plane->normal;
	}
}
