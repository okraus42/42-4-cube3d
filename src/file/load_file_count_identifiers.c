/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_file_count_identifiers.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 13:29:39 by plouda            #+#    #+#             */
/*   Updated: 2024/03/07 14:53:11 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

static void	get_identifiers_1(char **split, int *ids, int *flag)
{
	if (!ft_strncmp(split[0], "A", 1))
		ids[0]++;
	else if (!ft_strncmp(split[0], "C", 1))
		ids[1]++;
	else if (!ft_strncmp(split[0], "L", 1))
		ids[2]++;
	else if (!ft_strncmp(split[0], "#", 1))
		return ;
	else
		*flag = 1;
}

static void	get_identifiers_2(char **split, int *ids, int *flag)
{
	if (!ft_strncmp(split[0], "sp", 2))
		ids[3]++;
	else if (!ft_strncmp(split[0], "pl", 2))
		ids[4]++;
	else if (!ft_strncmp(split[0], "cy", 2))
		ids[5]++;
	else if (!ft_strncmp(split[0], "co", 2))
		ids[6]++;
	else if (!ft_strncmp(split[0], "SK", 2))
		ids[10]++;
	else
		*flag = 1;
}

void	get_identifiers(char **split, int *ids, int *flag)
{
	if (ft_strlen(split[0]) == 1)
		get_identifiers_1(split, ids, flag);
	else if (ft_strlen(split[0]) == 2)
		get_identifiers_2(split, ids, flag);
	else if (ft_strlen(split[0]) == 6 || ft_strlen(split[0]) == 5)
	{
		if (!ft_strncmp(split[0], ".ch/", 4))
			ids[7]++;
		else if (!ft_strncmp(split[0], ".tx/", 4))
			ids[8]++;
		else if (!ft_strncmp(split[0], ".vm/", 4))
			ids[9]++;
		else
			*flag = 1;
	}
	else
		*flag = 1;
}

int	count_identifiers(char *line, int *ids, int *flag)
{
	char	**split;
	char	*trimmed;

	trimmed = ft_strtrim(line, " \n");
	split = ft_split(trimmed, ' ');
	free(trimmed);
	if (ft_strlen(split[0]) > 0)
		get_identifiers(split, ids, flag);
	ft_free_split(&split);
	if (*flag)
		id_err(NULL, "Unknown identifier in file", NULL);
	return (0);
}
