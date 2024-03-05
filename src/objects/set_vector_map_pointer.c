/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_vector_map_pointer.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 13:30:20 by plouda            #+#    #+#             */
/*   Updated: 2024/03/05 14:09:57 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

static int	find_vector_map_id(char **split)
{
	int		i;
	int		id;
	int		len;

	i = 0;
	id = -1;
	len = 0;
	while (split[i])
	{
		if (!ft_strncmp(split[i], ".vm/", 4))
			if (specifier_parsing_check(i, len, &id, split) < 0)
				return (-2);
		i++;
	}
	return (id);
}

int	set_vector_map_pointer(char *obj,
	t_rt *rt, char **split, t_vector_map **ptr)
{
	int	id;
	int	i;

	i = 0;
	id = find_vector_map_id(split);
	if (id == -2)
		return (id_warn(obj, W_VMAP_ID, W_VMAP_CONT, 1));
	if (id == -1)
		return (0);
	while (i < rt->n_vector_maps)
	{
		if (rt->vector_maps[i]->id == id)
		{
			*ptr = rt->vector_maps[i];
			return (1);
		}
		i++;
	}
	return (id_warn(obj, W_VMAP_ID, W_VMAP_CONT, 1));
}
