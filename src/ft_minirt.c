/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minirt.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 13:40:49 by okraus            #+#    #+#             */
/*   Updated: 2023/11/13 09:22:24 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minirt.h"

int	init_objects(t_rt *rt, int *ids)
{
	int	i;

	rt->ambient = malloc(sizeof(t_ambient));
	rt->ambient->rgb = malloc(sizeof(int) * 3);
	rt->camera = malloc(sizeof(t_camera));
	rt->camera->coords = malloc(sizeof(double) * 3);
	rt->camera->nvect = malloc(sizeof(double) * 3);
	rt->light = malloc(sizeof(t_light));
	rt->light->coords = malloc(sizeof(double) * 3);
	rt->light->rgb = malloc(sizeof(int) * 3);
	i = 0;
	rt->spheres = malloc(sizeof(t_sphere *) * (ids[3] + 1));
	rt->n_spheres = 0;
	while (i < (ids[3]))
	{
		rt->spheres[i] = malloc(sizeof(t_sphere));
		rt->spheres[i]->coords = malloc(sizeof(double) * 3);
		rt->spheres[i++]->rgb = malloc(sizeof(int) * 3);
	}
	rt->spheres[i] = NULL;
	i = 0;
	rt->planes = malloc(sizeof(t_plane *) * (ids[4] + 1));
	rt->n_planes = 0;
	while (i < (ids[4]))
		rt->planes[i++] = malloc(sizeof(t_plane));
	rt->planes[i] = NULL;
	i = 0;
	rt->cylinders = malloc(sizeof(t_cylinder *) * (ids[5] + 1));
	rt->n_cylinders = 0;
	while (i < (ids[5]))
		rt->cylinders[i++] = malloc(sizeof(t_cylinder));
	rt->cylinders[i] = NULL;
	return (0);
}

int	load_file(char *file, t_rt *rt)
{
	int		fd;
	char	*line;
	int		flag;
	int		*ids;

	flag = 0;
	fd = open(file, O_RDONLY);
	ids = init_ids();
	check_identifiers(fd, ids, &flag);
	close(fd);
	init_objects(rt, ids);
	if (!flag)
	{
		fd = open(file, O_RDONLY);
		line = get_next_line(fd);
		while (line && !flag)
		{
			load_data(line, rt, &flag);
			free(line);
			line = get_next_line(fd);
		}
		free(line);
		close(fd);
	}
	free(ids);
	return (0);
}

int	main(int ac, char *av[])
{
	t_rt	*rt;
	int		i;

	i = 0;
	rt = NULL;
	rt = malloc(sizeof(t_rt));
	if (!rt)
		return (1);
	rt->ambient = NULL;
	rt->camera = NULL;
	rt->light = NULL;
	rt->spheres = NULL;
	rt->planes = NULL;
	rt->cylinders = NULL;
	if (ac != 2)
	{
		// error
		throw_error("Incorrect number of parameters");
		free(rt);
		return (1);
	}
	else
	{
		//mandatory (open specific map)
		ft_printf("Should open map: %s\n", av[1]);
		load_file(av[1], rt);
	}
	free(rt->ambient->rgb);
	free(rt->ambient);
	free(rt->camera->coords);
	free(rt->camera->nvect);
	free(rt->camera);
	free(rt->light->coords);
	free(rt->light->rgb);
	free(rt->light);
	while (rt->spheres[i] != NULL)
	{
		free(rt->spheres[i]->coords);
		free(rt->spheres[i]->rgb);
		free(rt->spheres[i++]);
	}
	free(rt->spheres);
	i = 0;
	while (rt->planes[i] != NULL)
	{
		/* free(rt->planes[i]->coords);
		free(rt->planes[i]->nvect);
		free(rt->planes[i]->rgb); */
		free(rt->planes[i++]);
	}
	free(rt->planes);
	i = 0;
	while (rt->cylinders[i] != NULL)
	{
		/* free(rt->cylinders[i]->coords);
		free(rt->cylinders[i]->nvect);
		free(rt->cylinders[i]->rgb); */
		free(rt->cylinders[i++]);
	}
	free(rt->cylinders);
	free(rt);
	return (0);
}
