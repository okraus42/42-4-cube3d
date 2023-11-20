/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 09:15:13 by plouda            #+#    #+#             */
/*   Updated: 2023/11/20 13:45:44 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minirt.h"

int	throw_error(char *str)
{
	char	*start;
	char	*end;
	char	*startstr;
	char	*error;

	start = "\e[91m";
	end = "\n\e[0m";
	startstr = ft_strjoin(start, str);
	error = ft_strjoin(startstr, end);
	free(startstr);
	write(2, error, ft_strlen(error));
	free(error);
	return (1);
}

int	id_err(char *id, char *err_str, char *details)
{
	if (id == NULL)
		ft_dprintf(2, "%.*CError: %s\n%C", 0xE74C3C, err_str);
	else
		ft_dprintf(2, "%.*CError: %s: %s, expected %s\n%C", \
		0xE74C3C, id, err_str, details);
	return (1);
}
