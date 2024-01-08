/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   o_cylinder_disc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 16:30:02 by plouda            #+#    #+#             */
/*   Updated: 2024/01/08 15:26:43 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	init_discs(t_cylinder *cylinder)
{
	cylinder->botcap = ft_calloc(1, sizeof(t_disc));
	cylinder->topcap = ft_calloc(1, sizeof(t_disc));
	cylinder->botcap->coords = ft_calloc(3, sizeof(double));
	cylinder->topcap->coords = ft_calloc(3, (sizeof(double)));
	cylinder->botcap->nvect = ft_calloc(3, sizeof(double));
	cylinder->topcap->nvect = ft_calloc(3, sizeof(double));
	cylinder->botcap->rgb = ft_calloc(3, sizeof(int));
	cylinder->topcap->rgb = ft_calloc(3, sizeof(int));
	cylinder->botcap->rgb_ambient = ft_calloc(3, sizeof(int));
	cylinder->topcap->rgb_ambient = ft_calloc(3, sizeof(int));
	cylinder->botcap->rgb_light = ft_calloc(3, sizeof(int));
	cylinder->topcap->rgb_light = ft_calloc(3, sizeof(int));
	cylinder->botcap->normal = ft_calloc(1, sizeof(t_vect3f));
	cylinder->topcap->normal = ft_calloc(1, sizeof(t_vect3f));
}

void	define_botcap(t_cylinder *cylinder)
{
	t_disc	*botcap;

	botcap = cylinder->botcap;
	botcap->coords[X] = cylinder->coords[X];
	botcap->coords[Y] = cylinder->coords[Y];
	botcap->coords[Z] = cylinder->coords[Z];
	botcap->nvect[X] = cylinder->nvect[X] * -1;
	botcap->nvect[Y] = cylinder->nvect[Y] * -1;
	botcap->nvect[Z] = cylinder->nvect[Z] * -1;
	botcap->rgb[R] = cylinder->rgb[R];
	botcap->rgb[G] = cylinder->rgb[G];
	botcap->rgb[B] = cylinder->rgb[B];
	botcap->rgb_ambient[R] = cylinder->rgb_ambient[R];
	botcap->rgb_ambient[G] = cylinder->rgb_ambient[G];
	botcap->rgb_ambient[B] = cylinder->rgb_ambient[B];
	botcap->rgb_light[R] = cylinder->rgb_light[R];
	botcap->rgb_light[G] = cylinder->rgb_light[G];
	botcap->rgb_light[B] = cylinder->rgb_light[B];
	botcap->radius = cylinder->diameter / 2;
	*botcap->normal = get_normal(botcap->nvect[X], \
		botcap->nvect[Y], botcap->nvect[Z]);
}

void	define_topcap(t_cylinder *cylinder)
{
	t_disc	*topcap;

	topcap = cylinder->topcap;
	topcap->coords[X] = cylinder->coords[X] \
			+ cylinder->height * cylinder->nvect[X];
	topcap->coords[Y] = cylinder->coords[Y] \
			+ cylinder->height * cylinder->nvect[Y];
	topcap->coords[Z] = cylinder->coords[Z] \
			+ cylinder->height * cylinder->nvect[Z];
	topcap->nvect[X] = cylinder->nvect[X];
	topcap->nvect[Y] = cylinder->nvect[Y];
	topcap->nvect[Z] = cylinder->nvect[Z];
	topcap->rgb[R] = cylinder->rgb[R];
	topcap->rgb[G] = cylinder->rgb[G];
	topcap->rgb[B] = cylinder->rgb[B];
	topcap->rgb_ambient[R] = cylinder->rgb_ambient[R];
	topcap->rgb_ambient[G] = cylinder->rgb_ambient[G];
	topcap->rgb_ambient[B] = cylinder->rgb_ambient[B];
	topcap->rgb_light[R] = cylinder->rgb_light[R];
	topcap->rgb_light[G] = cylinder->rgb_light[G];
	topcap->rgb_light[B] = cylinder->rgb_light[B];
	cylinder->topcap->radius = cylinder->diameter / 2;
	*topcap->normal = get_normal(topcap->nvect[X], \
		topcap->nvect[Y], topcap->nvect[Z]);
}

void	get_discs(t_cylinder *cylinder)
{
	define_botcap(cylinder);
	define_topcap(cylinder);
}

void	free_discs(t_cylinder *cylinder)
{
	free(cylinder->botcap->coords);
	free(cylinder->botcap->nvect);
	free(cylinder->botcap->rgb);
	free(cylinder->botcap->rgb_ambient);
	free(cylinder->botcap->rgb_light);
	free(cylinder->botcap->normal);
	free(cylinder->topcap->coords);
	free(cylinder->topcap->nvect);
	free(cylinder->topcap->rgb);
	free(cylinder->topcap->rgb_ambient);
	free(cylinder->topcap->rgb_light);
	free(cylinder->topcap->normal);
	free(cylinder->botcap);
	free(cylinder->topcap);
}
