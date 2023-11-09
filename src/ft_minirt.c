/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minirt.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 13:40:49 by okraus            #+#    #+#             */
/*   Updated: 2023/11/09 15:48:56 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minirt.h"

int	is_float_triad(char *str)
{
	int	i;
	int	j;
	int	flag;

	i = 0;
	j = 0;
	while (j < 3 && str[i])
	{
		flag = 0;
		if (str[i] == '-')
			i++;
		if (!ft_isdigit(str[i])) // is there at least one digit before the decimal
			flag++;
		while (str[i] && ft_isdigit(str[i]))
			i++;
		if (str[i] == '.')
			i++;
		if (!ft_isdigit(str[i]) && flag) // if no digits on either side, invalid
			return (0);
		while (str[i] && ft_isdigit(str[i]))
			i++;
		if (j != 2)
		{
			if (str[i++] != ',') // is there a comma at the expected place
				return (0);
		}
		else
		{
			if (str[i]) // is there anything after the last digit
				return (0); 
		}
		j++;
	}
	if (!str[i] && j < 3) // too short
		return (0);
	return (1);
}

int	is_rgb_format(char *str)
{
	int	i;
	int j;

	i = 0;
	j = 0;
	while (j < 3 && str[i])
	{
		if (str[i] == '-')
			i++;
		if (!ft_isdigit(str[i])) // is there at least one digit
			return (0);
		while (str[i] && ft_isdigit(str[i]))
			i++;
		if (j != 2)
		{
			if (str[i++] != ',') // is there a comma at the expected place
				return (0);
		}
		else
		{
			if (str[i]) // is there anything after the last digit
				return (0); 
		}
		j++;
	}
	if (!str[i] && j < 3) // too short
		return (0);
	return (1);
}

int	is_floatable(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-')
		i++;
	while (str[i] && ft_isdigit(str[i]))
		i++;
	if (str[i] == '.')
		i++;
	while (str[i] && ft_isdigit(str[i]))
		i++;
	if (str[i])
		return (0);
	return (1);
}

int	is_integer(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-')
		i++;
	while (str[i] && ft_isdigit(str[i]))
		i++;
	if (str[i])
		return (0);
	return (1);
	
}

int	float_in_range(char *str)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	if (str[i] == '-')
		i++;
	while (str[i] && ft_isdigit(str[i]))
	{
		i++;
		len++;
	}
	if (len > 4)
		return (0);
	len = 0;
	if (str[i] == '.')
		i++;
	while (str[i] && ft_isdigit(str[i]))
	{
		i++;
		len++;
	}
	if (len > 5)
		return (0);
	return (1);
}

int	check_format_ambient(char **split)
{
	int	num;

	num = 0;
	while (split[num])
		num++;
	if (num != 3)
	{
		throw_error("A: Invalid number of specifiers, expected 2");
		return (1);
	}
	if (!is_floatable(split[1]))
	{
		throw_error("A: Wrong ratio format, expected float");
		return (1);
	}
	if (!float_in_range(split[1]))
	{
		throw_error("A: Wrong ratio format, max. 4 integer digits and 5 decimals (ex. 9999.99999)");
		return (1);
	}
	if (!is_rgb_format(split[2]))
	{
		throw_error("A: Wrong RGB format, expected three integers separated by commas");
		return (1);
	}
	return (0);
}

int	check_format_camera(char **split)
{
	int		num;
	char	**subsplit;

	num = 0;
	while (split[num])
		num++;
	if (num != 4)
	{
		throw_error("C: Invalid number of specifiers, expected 3");
		return (1);
	}
	if (!is_float_triad(split[1]))
	{
		throw_error("C: Wrong coordinate format, expected three integers/floats separated by commas");
		return (1);
	}
	subsplit = ft_split(split[1], ',');
	if (!float_in_range(subsplit[0]) || !float_in_range(subsplit[1])
		|| !float_in_range(subsplit[2]))
	{
		throw_error("C: Wrong coordinate format, max. 4 integer digits and 5 decimals per coordinate (ex. 9999.99999)");
		ft_free_split(&subsplit);
		return (1);
	}
	ft_free_split(&subsplit);
	if (!is_float_triad(split[2]))
	{
		throw_error("C: Wrong vector format, expected three integers/floats separated by commas");
		return (1);
	}
	subsplit = ft_split(split[2], ',');
	if (!float_in_range(subsplit[0]) || !float_in_range(subsplit[1])
		|| !float_in_range(subsplit[2]))
	{
		throw_error("C: Wrong coordinate format, max. 4 integer digits and 5 decimals per coordinate (ex. 9999.99999)");
		ft_free_split(&subsplit);
		return (1);
	}
	ft_free_split(&subsplit);
	if (!is_integer(split[3]))
	{
		throw_error("C: Wrong FoV format, expected an integer");
		return (1);
	}
	return (0);
}

int	check_format_light(char **split)
{
	int		num;
	char	**subsplit;

	num = 0;
	while (split[num])
		num++;
	if (num != 4)
	{
		throw_error("L: Invalid number of specifiers, expected 3");
		return (1);
	}
	if (!is_float_triad(split[1]))
	{
		throw_error("L: Wrong coordinate format, expected three integers/floats separated by commas");
		return (1);
	}
	subsplit = ft_split(split[1], ',');
	if (!float_in_range(subsplit[0]) || !float_in_range(subsplit[1])
		|| !float_in_range(subsplit[2]))
	{
		throw_error("L: Wrong coordinate format, max. 4 integer digits and 5 decimals per coordinate (ex. 9999.99999)");
		ft_free_split(&subsplit);
		return (1);
	}
	ft_free_split(&subsplit);
	if (!is_floatable(split[2]))
	{
		throw_error("L: Wrong brightness format, expected float");
		return (1);
	}
	if (!float_in_range(split[2]))
	{
		throw_error("L: Wrong brightness format, max. 4 integer digits and 5 decimals (ex. 9999.99999)");
		return (1);
	}
	if (!is_rgb_format(split[3]))
	{
		throw_error("L: Wrong RGB format, expected three integers separated by commas");
		return (1);
	}
	return (0);
}

int	check_format_sphere(char **split)
{
	int		num;
	char	**subsplit;

	num = 0;
	while (split[num])
		num++;
	if (num != 4)
	{
		throw_error("sp: Invalid number of specifiers, expected 3");
		return (1);
	}
	if (!is_float_triad(split[1]))
	{
		throw_error("sp: Wrong coordinate format, expected three integers/floats separated by commas");
		return (1);
	}
	subsplit = ft_split(split[1], ',');
	if (!float_in_range(subsplit[0]) || !float_in_range(subsplit[1])
		|| !float_in_range(subsplit[2]))
	{
		throw_error("sp: Wrong coordinate format, max. 4 integer digits and 5 decimals per coordinate (ex. 9999.99999)");
		ft_free_split(&subsplit);
		return (1);
	}
	ft_free_split(&subsplit);
	if (!is_floatable(split[2]))
	{
		throw_error("sp: Wrong diameter format, expected float");
		return (1);
	}
	if (!float_in_range(split[2]))
	{
		throw_error("sp: Wrong diameter format, max. 4 integer digits and 5 decimals (ex. 9999.99999)");
		return (1);
	}
	if (!is_rgb_format(split[3]))
	{
		throw_error("sp: Wrong RGB format, expected three integers separated by commas");
		return (1);
	}
	return (0);
}

int	get_ambient(t_rt *rt, char **split)
{
	//t_ambient	*ambient;
	char		**rgb;

	/* needs protection against:
		- swapped values
		- values not digits (?)
		- only some values (e.g. missing rgb)
		- only some rgb specs (e.g. missing blue)
		- too many decimal places (check before atof)
		- number too big (check before atof)
		- unique properties more than once
	*/

	if (check_format_ambient(split))
		return (1);
	/* ambient = malloc(sizeof(t_ambient));
 	rt->ambient = ambient; */
	rt->ambient->ratio = ft_atof(split[1]);
	if (rt->ambient->ratio < 0.0 || rt->ambient->ratio > 1.0)
	{
		throw_error("A: Ratio out of bounds, expected number between 0.0 and 1.0");
		return (1);
	}
	rgb = ft_split(split[2], ',');
	rt->ambient->rgb = malloc(sizeof(int) * 3);
	rt->ambient->rgb[R] = ft_atoi(rgb[0]);
	rt->ambient->rgb[G] = ft_atoi(rgb[1]);
	rt->ambient->rgb[B] = ft_atoi(rgb[2]);
	ft_free_split(&rgb);
	if (rt->ambient->rgb[R]> 255 || rt->ambient->rgb[R] < 0
		|| rt->ambient->rgb[G] > 255 || rt->ambient->rgb[G] < 0
		|| rt->ambient->rgb[B] > 255 || rt->ambient->rgb[B] < 0)
	{
		throw_error("A: RGB out of bounds, expected integers between 0 and 255");
		return (1);
	}
	return (0);
}

int	get_camera(t_rt *rt, char **split)
{
	//t_camera	*camera;
	char		**coords;
	char		**nvect;

	if (check_format_camera(split))
		return (1);
	/* camera = malloc(sizeof(t_camera));
	rt->camera = camera; */
	coords = ft_split(split[1], ',');
	rt->camera->coords = malloc(sizeof(double) * 3);
	rt->camera->coords[X] = ft_atof(coords[0]);
	rt->camera->coords[Y] = ft_atof(coords[1]);
	rt->camera->coords[Z] = ft_atof(coords[2]);
	ft_free_split(&coords);
	nvect = ft_split(split[2], ',');
	rt->camera->nvect = malloc(sizeof(double) * 3);
	rt->camera->nvect[X] = ft_atof(nvect[0]);
	rt->camera->nvect[Y] = ft_atof(nvect[1]);
	rt->camera->nvect[Z] = ft_atof(nvect[2]);
	ft_free_split(&nvect);
	if (rt->camera->nvect[X] < -1. || rt->camera->nvect[X] > 1.
		|| rt->camera->nvect[Y] < -1. || rt->camera->nvect[Y] > 1.
		|| rt->camera->nvect[Z] < -1. || rt->camera->nvect[Z] > 1.)
	{
		throw_error("C: Vector coordinates out of bounds, expected numbers between [-1.0;1.0]");
		ft_dprintf(2, "%.*CC: Vector coordinates out of bounds, expected numbers between [-1.0;1.0]\n%C", 0xff0000);
		return (1);
	}
	rt->camera->fov = ft_atoi(split[3]);
	if (rt->camera->fov < 0 || rt->camera->fov > 180)
	{
		throw_error("C: FoV coordinates out of bounds, expected a number between [0;180]");
		return (1);
	}
	return (0);
}

int	get_light(t_rt *rt, char **split)
{
	//t_light	*light;
	char	**coords;
	char	**rgb;

	if (check_format_light(split))
		return (1);
	/* light = malloc(sizeof(t_light));
	rt->light = light; */
	coords = ft_split(split[1], ',');
	rt->light->coords = malloc(sizeof(double) * 3);
	rt->light->coords[X] = ft_atof(coords[0]);
	rt->light->coords[Y] = ft_atof(coords[1]);
	rt->light->coords[Z] = ft_atof(coords[2]);
	ft_free_split(&coords);
	rt->light->brightness = ft_atof(split[2]);
	if (rt->light->brightness < 0.0 || rt->light->brightness > 1.0)
	{
		throw_error("L: Brightness out of bounds, expected number between 0.0 and 1.0");
		return (1);
	}
	rgb = ft_split(split[3], ',');
	rt->light->rgb = malloc(sizeof(int) * 3);
	rt->light->rgb[R] = ft_atoi(rgb[0]);
	rt->light->rgb[G] = ft_atoi(rgb[1]);
	rt->light->rgb[B] = ft_atoi(rgb[2]);
	ft_free_split(&rgb);
	if (rt->light->rgb[R]> 255 || rt->light->rgb[R] < 0
		|| rt->light->rgb[G] > 255 || rt->light->rgb[G] < 0
		|| rt->light->rgb[B] > 255 || rt->light->rgb[B] < 0)
	{
		throw_error("L: RGB out of bounds, expected integers between 0 and 255");
		return (1);
	}
	return (0);
}

int	add_sphere(t_rt *rt, char **split)
{
	//t_sphere	*sphere;
	char		**coords;
	char		**rgb;
	int			i;

	if (check_format_sphere(split))
		return (1);
	//sphere = malloc(sizeof(t_sphere));
	/* while (rt->spheres[i] != NULL)
		i++; */
	//rt->spheres[i] = sphere;
	i = rt->n_spheres;
	ft_printf("Processing %i. sphere\n", i);
	coords = ft_split(split[1], ',');
	ft_printf("ptr: %p\n", rt->spheres[i]);
	rt->spheres[i]->coords = malloc(sizeof(double) * 3);
	rt->spheres[i]->coords[X] = ft_atof(coords[0]);
	rt->spheres[i]->coords[Y] = ft_atof(coords[1]);
	rt->spheres[i]->coords[Z] = ft_atof(coords[2]);
	ft_free_split(&coords);
	rt->spheres[i]->diameter = ft_atof(split[2]);
	if (rt->spheres[i]->diameter <= 0)
	{
		throw_error("sp: Nice try. Expected diameter value larger than 0.");
		return (1);
	}
	rgb = ft_split(split[3], ',');
	rt->spheres[i]->rgb = malloc(sizeof(int) * 3);
	rt->spheres[i]->rgb[R] = ft_atoi(rgb[0]);
	rt->spheres[i]->rgb[G] = ft_atoi(rgb[1]);
	rt->spheres[i]->rgb[B] = ft_atoi(rgb[2]);
	ft_free_split(&rgb);
	if (rt->spheres[i]->rgb[R]> 255 || rt->spheres[i]->rgb[R] < 0
		|| rt->spheres[i]->rgb[G] > 255 || rt->spheres[i]->rgb[G] < 0
		|| rt->spheres[i]->rgb[B] > 255 || rt->spheres[i]->rgb[B] < 0)
	{
		throw_error("sp: RGB out of bounds, expected integers between 0 and 255");
		return (1);
	}
	rt->n_spheres++;
	return (0);
}

int	load_data(char *line, t_rt *rt, int *flag)
{
	char	**split;
	char	*trimmed;
	int		i;

	i = 0;
	trimmed = ft_strtrim(line, " \n");
	split = ft_split(trimmed, ' ');
	free(trimmed);
	if (ft_strlen(split[0]) != 0)
	{
		if (!ft_strncmp(split[0], "A", ft_strlen(split[0])))
			*flag = get_ambient(rt, split);
		else if (!ft_strncmp(split[0], "C", ft_strlen(split[0])))
			*flag = get_camera(rt, split);
		else if (!ft_strncmp(split[0], "L", ft_strlen(split[0])))
			*flag = get_light(rt, split);
		else if (!ft_strncmp(split[0], "sp", ft_strlen(split[0])))
			*flag = add_sphere(rt, split);
		/*
		else if (!ft_strncmp(split[0], "pl", ft_strlen(split[0])))
			add_plane(rt, split);
		else if (!ft_strncmp(split[0], "cy", ft_strlen(split[0])))
			add_cylinder(rt, split); 
		*/
	}
	ft_free_split(&split);
	return (0);
}

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

int	init_objects(t_rt *rt, int *ids)
{
	int	i;

	rt->ambient = malloc(sizeof(t_ambient));
	rt->camera = malloc(sizeof(t_camera));
	rt->light = malloc(sizeof(t_light));
	i = 0;
	rt->spheres = malloc(sizeof(t_sphere *) * (ids[3] + 1));
	rt->n_spheres = 0;
	while (i < (ids[3]))
		rt->spheres[i++] = malloc(sizeof(t_sphere));
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
