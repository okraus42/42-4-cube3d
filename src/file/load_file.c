/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 10:48:21 by plouda            #+#    #+#             */
/*   Updated: 2024/03/05 10:51:43 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

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
