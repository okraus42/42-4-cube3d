/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 09:15:13 by plouda            #+#    #+#             */
/*   Updated: 2023/11/20 15:51:07 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minirt.h"

int	id_err(char *id, char *err_str, char *details)
{
	if (id == NULL)
		ft_dprintf(2, "%.*CError: %s\n%C", 0xE74C3C, err_str);
	else
		ft_dprintf(2, "%.*CError: %s: %s, expected %s\n%C", \
		0xE74C3C, id, err_str, details);
	return (1);
}
