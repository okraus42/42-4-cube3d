/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_texture_pointer.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 14:07:38 by plouda            #+#    #+#             */
/*   Updated: 2024/03/05 14:08:09 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

static int	find_texture_id(char **split)
{
	int		i;
	int		id;
	int		len;

	i = 0;
	id = -1;
	len = 0;
	while (split[i])
	{
		if (!ft_strncmp(split[i], ".tx/", 4))
			if (specifier_parsing_check(i, len, &id, split) < 0)
				return (-2);
		i++;
	}
	return (id);
}

int	set_texture_pointer(char *obj, t_rt *rt, char **split, t_texture **ptr)
{
	int	id;
	int	i;

	i = 0;
	id = find_texture_id(split);
	if (id == -2)
		return (id_warn(obj, W_TEXT_ID, W_TEXT_CONT, 1));
	if (id == -1)
		return (0);
	while (i < rt->n_textures)
	{
		if (rt->textures[i]->id == id)
		{
			*ptr = rt->textures[i];
			return (1);
		}
		i++;
	}
	return (id_warn(obj, W_TEXT_ID, W_TEXT_CONT, 1));
}
