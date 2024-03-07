/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 10:48:21 by plouda            #+#    #+#             */
/*   Updated: 2024/03/07 14:53:21 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	check_missing(int *ids, int *flag)
{
	if (!ids[0])
		id_err(NULL, "Missing obligatory identifier: A", NULL);
	else if (!ids[1])
		id_err(NULL, "Missing obligatory identifier: C", NULL);
	else if (!ids[2])
		id_err(NULL, "Missing obligatory identifier: L", NULL);
	if (!ids[0] || !ids[1] || !ids[2])
		*flag = 1;
}

void	check_duplicates(int *ids, int *flag)
{
	if (ids[0] > 1)
		id_err(NULL, "Duplicate identifier: A", NULL);
	else if (ids[1] > 1)
		id_err(NULL, "Duplicate identifier: C", NULL);
	else if (ids[10] > 1)
		id_err(NULL, "Duplicate identifier: SK", NULL);
	if (ids[0] > 1 || ids[1] > 1 || ids[10] > 1)
		*flag = 1;
}

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

int	load_file(char *file, t_rt *rt, int fd)
{
	int		flag;
	int		*ids;

	flag = 0;
	ids = init_ids();
	check_identifiers(fd, ids, &flag);
	close(fd);
	init_objects(rt, ids);
	free(ids);
	if (!flag)
		get_texture_data(rt, file, &flag);
	if (!flag)
		get_object_data(rt, file, &flag);
	return (flag);
}

int	open_file(char *path)
{
	int		fd;
	char	*extension;

	fd = -1;
	extension = ft_strrchr(path, '.');
	if (extension != NULL)
	{
		if (ft_strlen(extension) != 3 || ft_strncmp(extension, ".rt", 3))
		{
			id_err("File", E_FILE_EXT, ".rt");
			return (fd);
		}
	}
	else
	{
		id_err("File", E_FILE_EXT, ".rt");
		return (fd);
	}
	fd = open(path, O_RDONLY);
	if (fd < 0)
		id_err("File", "Invalid file", NULL);
	return (fd);
}
