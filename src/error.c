/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 09:15:13 by plouda            #+#    #+#             */
/*   Updated: 2023/11/16 11:04:22 by plouda           ###   ########.fr       */
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

int	id_err(char id, char *err_str, char *details)
{
	ft_dprintf(2, "%.*CError: %c: %s, expected %s\n%C", \
	0xE74C3C, id, err_str, details);
	return (1);
}