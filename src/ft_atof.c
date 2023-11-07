/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 13:26:55 by plouda            #+#    #+#             */
/*   Updated: 2023/11/07 13:45:55 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minirt.h"

static int	space_sign(char *str, int *neg)
{
	int	i;

	i = 0;
	while (str[i] == '\n' || str[i] == '\r' || str[i] == '\t'
		|| str[i] == '\f' || str[i] == '\v' || str[i] == ' ')
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			*neg *= -1;
		i++;
	}
	return (i);
}

double	ft_atof(char *str)
{
	int		i;
	double	nb;
	int		neg;
	double	div;

	nb = 0;
	div = 0.1;
	neg = 1;
	i = space_sign(str, &neg);
	while (str[i] && ft_isdigit(str[i]) && str[i] != '.')
	{
		nb = (nb * 10.0) + (str[i] - '0');
		i++;
	}
	if (str[i] == '.')
		i++;
	while (str[i] && ft_isdigit(str[i]))
	{
		nb = nb + ((str[i] - '0') * div);
		div *= 0.1;
		i++;
	}
	/* if (str[i] && !ft_isdigit(str[i]))
		return (-100); */
	return (nb * neg);
}