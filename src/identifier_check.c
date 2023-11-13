/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identifier_check.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 09:22:02 by plouda            #+#    #+#             */
/*   Updated: 2023/11/13 10:43:03 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minirt.h"

// Check for missing obligatory identifiers

int	count_identifiers(char *line, int *ids, int *flag)
{
	char	**split;
	char	*trimmed;

	trimmed = ft_strtrim(line, " \n");
	split = ft_split(trimmed, ' ');
	free(trimmed);
	if (ft_strlen(split[0]) != 0)
	{
		if (!ft_strncmp(split[0], "A", ft_strlen(split[0])))
			ids[0]++;
		else if (!ft_strncmp(split[0], "C", ft_strlen(split[0])))
			ids[1]++;
		else if (!ft_strncmp(split[0], "L", ft_strlen(split[0])))
			ids[2]++;
		else if (!ft_strncmp(split[0], "sp", ft_strlen(split[0])))
			ids[3]++;
		else if (!ft_strncmp(split[0], "pl", ft_strlen(split[0])))
			ids[4]++;
		else if (!ft_strncmp(split[0], "cy", ft_strlen(split[0])))
			ids[5]++;
		else
		{
			throw_error("Unknown identifier in file");
			*flag = 1;
		}
	}
	ft_free_split(&split);
	return (0);
}

// checks for uniqueness of A, C and L, and the number of sp, pl and cy
// order: A, C, L, sp, pl, cy
int	check_identifiers(int fd, int *ids, int *flag)
{
	
	char	*line;

	line = get_next_line(fd);
	while (line && !(*flag))
	{
		count_identifiers(line, ids, flag);
		//ft_dprintf(2, "A: %i, C: %i, L: %i, sp: %i, pl: %i, cy: %i\n", ids[0], ids[1], ids[2], ids[3], ids[4], ids[5]);
		if (ids[0] > 1)
			throw_error("Duplicate identifiers for A");
		else if (ids[1] > 1)
			throw_error("Duplicate identifiers for C");
		else if (ids[2] > 1)
			throw_error("Duplicate identifiers for L");
		if (ids[0] > 1 || ids[1] > 1 || ids[2] > 1)
			*flag = 1;
		free(line);
		line = get_next_line(fd);
	}
	free(line);
	return (0);
}

int	*init_ids(void)
{
	int		*ids;
	int		i;

	i = 0;
	ids = malloc(sizeof(int) * 6);
	while (i < 6)
		ids[i++] = 0;
	return (ids);
}
