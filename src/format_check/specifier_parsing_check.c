/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   specifier_parsing_check.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 14:09:43 by plouda            #+#    #+#             */
/*   Updated: 2024/03/05 14:10:07 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

int	specifier_parsing_check(int i, int len, int *id, char **split)
{
	char	**specifier;

	specifier = ft_split(split[i], '/');
	while (specifier[1][len])
	{
		if (len > 3 || !ft_isdigit(specifier[1][len]))
		{
			ft_free_split(&specifier);
			return (-2);
		}
		len++;
	}
	*id = ft_atoi(specifier[1]);
	ft_free_split(&specifier);
	return (0);
}
