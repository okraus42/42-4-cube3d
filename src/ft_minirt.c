/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minirt.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 13:40:49 by okraus            #+#    #+#             */
/*   Updated: 2023/11/15 09:44:46 by plouda           ###   ########.fr       */
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
	{
		rt->planes[i] = malloc(sizeof(t_plane));
		rt->planes[i]->coords = malloc(sizeof(double) * 3);
		rt->planes[i]->nvect = malloc(sizeof(double) * 3);
		rt->planes[i++]->rgb = malloc(sizeof(int) * 3);
	}
	rt->planes[i] = NULL;
	i = 0;
	rt->cylinders = malloc(sizeof(t_cylinder *) * (ids[5] + 1));
	rt->n_cylinders = 0;
	while (i < (ids[5]))
	{
		rt->cylinders[i] = malloc(sizeof(t_cylinder));
		rt->cylinders[i]->coords = malloc(sizeof(double) * 3);
		rt->cylinders[i]->nvect = malloc(sizeof(double) * 3);
		rt->cylinders[i++]->rgb = malloc(sizeof(double) * 3);
	}
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

int	print_contents(t_rt *rt)
{
	int	i;

	i = 0;
	printf("=================================================\n");
	printf("AMBIENT\n");
	printf("%-11s %.2f\n%-11s %i,%i,%i\n", "Ratio:", rt->ambient->ratio, \
		"RGB:", rt->ambient->rgb[0], rt->ambient->rgb[1], rt->ambient->rgb[2]);
	printf("=================================================\n");
	printf("CAMERA\n");
	printf("%-11s %.2f,%.2f,%.2f\n%-11s %.2f,%.2f,%.2f\n%-11s %i\n", \
			"Coords:", rt->camera->coords[0], rt->camera->coords[1], rt->camera->coords[2], \
			"Vector:", rt->camera->nvect[0], rt->camera->nvect[1], rt->camera->nvect[2], \
			"FoV:", rt->camera->fov);
	printf("=================================================\n");
	printf("LIGHT\n");
	printf("%-11s %.2f,%.2f,%.2f\n%-11s %.2f\n%-11s %i,%i,%i\n", \
			"Coords:", rt->light->coords[0], rt->light->coords[1], rt->light->coords[2], \
			"Brightness:", rt->light->brightness,
			"RGB:", rt->light->rgb[0], rt->light->rgb[1], rt->light->rgb[2]);
	printf("=================================================\n");
	while (rt->n_spheres > i)
	{
		printf("SPHERE %i\n", i + 1);
		printf("%-11s %.2f,%.2f,%.2f\n%-11s %.2f\n%-11s %i,%i,%i\n", \
				"Coords:", rt->spheres[i]->coords[0], rt->spheres[i]->coords[1], rt->spheres[i]->coords[2], \
				"Diameter:", rt->spheres[i]->diameter, \
				"RGB:", rt->spheres[i]->rgb[0], rt->spheres[i]->rgb[1], rt->spheres[i]->rgb[2]);
		printf("=================================================\n");
		i++;
	}
	i = 0;
	while (rt->n_planes > i)
	{
		printf("PLANE %i\n", i + 1);
		printf("%-11s %.2f,%.2f,%.2f\n%-11s %.2f,%.2f,%.2f\n%-11s %i,%i,%i\n", \
				"Coords:", rt->planes[i]->coords[0], rt->planes[i]->coords[1], rt->planes[i]->coords[2], \
				"Vector:", rt->planes[i]->nvect[0], rt->planes[i]->nvect[1], rt->planes[i]->nvect[2], \
				"RGB:", rt->planes[i]->rgb[0], rt->planes[i]->rgb[1], rt->planes[i]->rgb[2]);
		printf("=================================================\n");
		i++;
	}
	i = 0;
	while (rt->n_cylinders > i)
	{
		printf("CYLINDER %i\n", i + 1);
		printf("%-11s %.2f,%.2f,%.2f\n%-11s %.2f,%.2f,%.2f\n%-11s %.2f\n%-11s %.2f\n%-11s %i,%i,%i\n", \
				"Coords:", rt->cylinders[i]->coords[0], rt->cylinders[i]->coords[1], rt->cylinders[i]->coords[2], \
				"Vector:", rt->cylinders[i]->nvect[0], rt->cylinders[i]->nvect[1], rt->cylinders[i]->nvect[2], \
				"Diameter:", rt->cylinders[i]->diameter, \
				"Height:", rt->cylinders[i]->height, \
				"RGB:", rt->cylinders[i]->rgb[0], rt->cylinders[i]->rgb[1], rt->cylinders[i]->rgb[2]);
		printf("=================================================\n");
		i++;
	}
	return (0);
}

int	main(int ac, char *av[])
{
	t_rt	*rt;
	//int		i;

	//i = 0;
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
		print_contents(rt);
	}
	free_all(rt);
	return (0);
}
