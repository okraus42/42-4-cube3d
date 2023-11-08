/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 09:15:13 by plouda            #+#    #+#             */
/*   Updated: 2023/11/08 09:19:57 by plouda           ###   ########.fr       */
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
	return (0);
}