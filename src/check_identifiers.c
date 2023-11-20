/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_identifiers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 09:22:02 by plouda            #+#    #+#             */
/*   Updated: 2023/11/20 14:13:08 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minirt.h"

int	check_identifiers(int fd, int *ids, int *flag)
{
	char	*line;

	line = get_next_line(fd);
	while (line && !(*flag))
	{
		count_identifiers(line, ids, flag);
		check_duplicates(ids, flag);
		free(line);
		line = get_next_line(fd);
	}
	if (!*flag)
		check_missing(ids, flag);
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
