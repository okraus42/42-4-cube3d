/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_texture_data.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 10:49:15 by plouda            #+#    #+#             */
/*   Updated: 2024/03/05 11:04:00 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

static void	get_texture_data_from_line(char *line, t_rt *rt, int *flag)
{
	char	**split;
	char	*trimmed;

	trimmed = ft_strtrim(line, " \n");
	split = ft_split(trimmed, ' ');
	free(trimmed);
	if (ft_strlen(split[0]) > 0)
	{
		if (!ft_strncmp(split[0], ".ch/", 4))
			*flag = fill_checkerboard(rt, split);
		if (!ft_strncmp(split[0], ".tx/", 4))
			*flag = fill_texture(rt, split);
		if (!ft_strncmp(split[0], ".vm/", 4))
			*flag = fill_vector_map(rt, split);
	}
	ft_free_split(&split);
}

void	get_texture_data(t_rt *rt, char *file, int *flag)
{
	int		fd;
	char	*line;

	fd = open(file, O_RDONLY);
	line = get_next_line(fd);
	while (line && !(*flag))
	{
		get_texture_data_from_line(line, rt, flag);
		free(line);
		line = get_next_line(fd);
	}
	free(line);
	close(fd);
}
