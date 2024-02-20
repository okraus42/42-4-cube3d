/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   o_cylinder_disc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 16:30:02 by plouda            #+#    #+#             */
/*   Updated: 2024/02/20 10:35:11 by plouda           ###   ########.fr       */
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
	cylinder->botcap->normal = ft_calloc(1, sizeof(t_vect3f));
	cylinder->topcap->normal = ft_calloc(1, sizeof(t_vect3f));
	cylinder->botcap->right = ft_calloc(1, sizeof(t_vect3f));
	cylinder->topcap->right = ft_calloc(1, sizeof(t_vect3f));
	cylinder->botcap->up = ft_calloc(1, sizeof(t_vect3f));
	cylinder->topcap->up = ft_calloc(1, sizeof(t_vect3f));
	cylinder->botcap->mode = DEFAULT;
	cylinder->topcap->mode = DEFAULT;
	cylinder->botcap->checkerboard = NULL;
	cylinder->topcap->checkerboard = NULL;
	cylinder->botcap->texture = NULL;
	cylinder->topcap->texture = NULL;
	cylinder->botcap->vector_map = NULL;
	cylinder->topcap->vector_map = NULL;

}

void	define_botcap(t_cylinder *cylinder)
{
	t_disc	*botcap;

	botcap = cylinder->botcap;
	botcap->glossiness = cylinder->glossiness;
	botcap->coords[X] = cylinder->coords[X] + (-1 * cylinder->normal->x * cylinder->height / 2);
	botcap->coords[Y] = cylinder->coords[Y] + (-1 * cylinder->normal->y * cylinder->height / 2);
	botcap->coords[Z] = cylinder->coords[Z] + (-1 * cylinder->normal->z * cylinder->height / 2);
	botcap->nvect[X] = cylinder->normal->x * -1;
	botcap->nvect[Y] = cylinder->normal->y * -1;
	botcap->nvect[Z] = cylinder->normal->z * -1;
	botcap->rgb[R] = cylinder->rgb[R];
	botcap->rgb[G] = cylinder->rgb[G];
	botcap->rgb[B] = cylinder->rgb[B];
	botcap->radius = cylinder->diameter / 2;
	*botcap->normal = get_normal(botcap->nvect[X], \
		botcap->nvect[Y], botcap->nvect[Z]);
	set_disc_vects(botcap);
	botcap->checkerboard = cylinder->checkerboard;
	botcap->texture = cylinder->texture;
	if (botcap->texture)
		botcap->tx_disc = cylinder->texture->tx_bot;
	else
		botcap->tx_disc = NULL;
	botcap->vector_map = cylinder->vector_map;
	if (botcap->vector_map)
		botcap->vm_disc = cylinder->vector_map->vm_bot;
	else
		botcap->vm_disc = NULL;
	//botcap->texture->tx_main = cylinder->texture->tx_bot;
}

void	define_topcap(t_cylinder *cylinder)
{
	t_disc	*topcap;

	topcap = cylinder->topcap;
	topcap->glossiness = cylinder->glossiness;
	topcap->coords[X] = cylinder->coords[X] \
			+ cylinder->height / 2 * cylinder->normal->x;
	topcap->coords[Y] = cylinder->coords[Y] \
			+ cylinder->height / 2 * cylinder->normal->y;
	topcap->coords[Z] = cylinder->coords[Z] \
			+ cylinder->height / 2 * cylinder->normal->z;
	topcap->nvect[X] = cylinder->normal->x;
	topcap->nvect[Y] = cylinder->normal->y;
	topcap->nvect[Z] = cylinder->normal->z;
	topcap->rgb[R] = cylinder->rgb[R];
	topcap->rgb[G] = cylinder->rgb[G];
	topcap->rgb[B] = cylinder->rgb[B];
	cylinder->topcap->radius = cylinder->diameter / 2;
	*topcap->normal = get_normal(topcap->nvect[X], \
		topcap->nvect[Y], topcap->nvect[Z]);
	set_disc_vects(topcap);
	topcap->checkerboard = cylinder->checkerboard;
	topcap->texture = cylinder->texture;
	if (topcap->texture)
		topcap->tx_disc = cylinder->texture->tx_top;
	else
		topcap->tx_disc = NULL;
	topcap->vector_map = cylinder->vector_map;
	if (topcap->vector_map)
		topcap->vm_disc = cylinder->vector_map->vm_top;
	else
		topcap->vm_disc = NULL;
	//topcap->texture->tx_main = cylinder->texture->tx_top;
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
	free(cylinder->botcap->normal);
	free(cylinder->botcap->right);
	free(cylinder->botcap->up);
	free(cylinder->botcap);
	free(cylinder->topcap->coords);
	free(cylinder->topcap->nvect);
	free(cylinder->topcap->rgb);
	free(cylinder->topcap->normal);
	free(cylinder->topcap->right);
	free(cylinder->topcap->up);
	free(cylinder->topcap);
}
