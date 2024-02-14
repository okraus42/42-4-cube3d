/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_checkerboard_pointer.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 13:30:20 by plouda            #+#    #+#             */
/*   Updated: 2024/02/14 13:30:55 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

static int	find_checkerboard_id(char **split)
{
	int	i;
	int	j;
	int	id;
	char	**specifier;

	i = 0;
	j = 0;
	id = -1;
	while (split[i])
		i++;
	if (!ft_strncmp(split[i-1], ".ch/", 4))
	{
		specifier = ft_split(split[i-1], '/');
		id = ft_atoi(specifier[1]);
		ft_free_split(&specifier);
	}
	return (id);
}

void	get_checkerboard_pointer(t_rt *rt, char **split, t_checkerboard **ptr)
{
	int	id;
	int	i;

	i = 0;
	id = find_checkerboard_id(split);
	if (id < 0) // can this even happen?
		return ;
	while (i < rt->n_checkerboards)
	{
		if (rt->checkerboards[i]->id == id)
		{
			*ptr = rt->checkerboards[i];
			return ;
		}
		i++;
	}
}
