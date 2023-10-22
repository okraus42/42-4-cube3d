/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minirt.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 13:40:49 by okraus            #+#    #+#             */
/*   Updated: 2023/10/22 14:01:12 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minirt.h"

int	main(int ac, char *av[])
{
	if (ac != 2)
	{
		// error
		ft_dprintf(2, "Incorrect number of parameters\n");
	}
	else
	{
		//mandatory (open specific map)
		ft_printf("Should open map: %s\n", av[1]);
	}
	return (0);
}
