/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   has_spec_count.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 15:03:22 by plouda            #+#    #+#             */
/*   Updated: 2024/03/05 15:03:49 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

static int	has_spec_count_helper(char **split, int target, int num)
{
	int	ch_flag;
	int	tx_flag;
	int	vm_flag;
	int	i;

	ch_flag = 0;
	tx_flag = 0;
	vm_flag = 0;
	i = target - 1;
	while (++i < num && split[i])
	{
		if (!ft_strncmp(split[i], ".ch/", 4) && !ch_flag)
			ch_flag++;
		else if (!ft_strncmp(split[i], ".tx/", 4) && !tx_flag)
			tx_flag++;
		else if (!ft_strncmp(split[i], ".vm/", 4) && !vm_flag)
			vm_flag++;
		else
			return (1);
	}
	return (0);
}

int	has_spec_count(char **split, int target)
{
	int	num;

	num = 0;
	while (split[num])
		num++;
	if (num > target + 3)
		return (0);
	else if (num > target && num <= target + 3)
	{
		if (has_spec_count_helper(split, target, num))
			return (-1);
	}
	else if (num < target)
		return (0);
	return (1);
}

int	has_spec_count_strict(char **split, int target)
{
	int	num;

	num = 0;
	while (split[num])
		num++;
	if (num != target)
		return (0);
	return (1);
}
