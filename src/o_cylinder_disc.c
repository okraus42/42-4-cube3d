/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   o_cylinder_disc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 16:30:02 by plouda            #+#    #+#             */
/*   Updated: 2023/12/08 11:20:52 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minirt.h"

void	init_discs(t_cylinder *cylinder)
{
	cylinder->botcap = malloc(sizeof(t_disc));
	cylinder->topcap = malloc(sizeof(t_disc));
	cylinder->botcap->coords = malloc(sizeof(double) * 3);
	cylinder->topcap->coords = malloc(sizeof(double) * 3);
	cylinder->botcap->nvect = malloc(sizeof(double) * 3);
	cylinder->topcap->nvect = malloc(sizeof(double) * 3);
	cylinder->botcap->rgb = malloc(sizeof(int) * 3);
	cylinder->topcap->rgb = malloc(sizeof(int) * 3);
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
	botcap->radius = cylinder->diameter / 2;
}

void	define_topcap(t_cylinder *cylinder)
{
	t_disc	*topcap;

	topcap = cylinder->topcap;
	topcap->coords[X] = cylinder->coords[X] + cylinder->height * cylinder->nvect[X];
	topcap->coords[Y] = cylinder->coords[Y] + cylinder->height * cylinder->nvect[Y];
	topcap->coords[Z] = cylinder->coords[Z] + cylinder->height * cylinder->nvect[Z];
	topcap->nvect[X] = cylinder->nvect[X];
	topcap->nvect[Y] = cylinder->nvect[Y];
	topcap->nvect[Z] = cylinder->nvect[Z];
	topcap->rgb[R] = cylinder->rgb[R];
	topcap->rgb[G] = cylinder->rgb[G];
	topcap->rgb[B] = cylinder->rgb[B];
	cylinder->topcap->radius = cylinder->diameter / 2;
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
	free(cylinder->topcap->coords);
	free(cylinder->topcap->nvect);
	free(cylinder->topcap->rgb);
}
