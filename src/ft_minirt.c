/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minirt.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 13:40:49 by okraus            #+#    #+#             */
/*   Updated: 2023/11/08 16:03:44 by plouda           ###   ########.fr       */
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

int	get_ambient(t_rt *rt, char **split)
{
	t_ambient	*ambient;
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
	ambient = malloc(sizeof(t_ambient));
 	rt->ambient = ambient;
	ambient->ratio = ft_atof(split[1]);
	if (ambient->ratio < 0.0 || ambient->ratio > 1.0)
	{
		throw_error("A: Ratio out of bounds, expected number between 0.0 and 1.0");
		return (1);
	}
	rgb = ft_split(split[2], ',');
	ambient->rgb = malloc(sizeof(int) * 3);
	ambient->rgb[R] = ft_atoi(rgb[0]);
	ambient->rgb[G] = ft_atoi(rgb[1]);
	ambient->rgb[B] = ft_atoi(rgb[2]);
	ft_free_split(&rgb);
	if (ambient->rgb[R]> 255 || ambient->rgb[R] < 0
		|| ambient->rgb[G] > 255 || ambient->rgb[G] < 0
		|| ambient->rgb[B] > 255 || ambient->rgb[B] < 0)
	{
		throw_error("A: RGB out of bounds, expected integers between 0 and 255");
		return (1);
	}
	return (0);
}

int	get_camera(t_rt *rt, char **split)
{
	t_camera	*camera;
	char		**coords;
	char		**nvect;

	if (check_format_camera(split))
		return (1);
	camera = malloc(sizeof(t_camera));
	rt->camera = camera;
	coords = ft_split(split[1], ',');
	camera->coords = malloc(sizeof(double) * 3);
	camera->coords[X] = ft_atof(coords[0]);
	camera->coords[Y] = ft_atof(coords[1]);
	camera->coords[Z] = ft_atof(coords[2]);
	ft_free_split(&coords);
	nvect = ft_split(split[2], ',');
	camera->nvect = malloc(sizeof(double) * 3);
	camera->nvect[X] = ft_atof(nvect[0]);
	camera->nvect[Y] = ft_atof(nvect[1]);
	camera->nvect[Z] = ft_atof(nvect[2]);
	ft_free_split(&nvect);
	if (camera->nvect[X] < -1. || camera->nvect[X] > 1.
		|| camera->nvect[Y] < -1. || camera->nvect[Y] > 1.
		|| camera->nvect[Z] < -1. || camera->nvect[Z] > 1.)
	{
		throw_error("C: Vector coordinates out of bounds, expected numbers between [-1.0;1.0]");
		ft_dprintf(2, "%.*CC: Vector coordinates out of bounds, expected numbers between [-1.0;1.0]\n%C", 0xff0000);
		return (1);
	}
	camera->fov = ft_atoi(split[3]);
	if (camera->fov < 0 || camera->fov > 180)
	{
		throw_error("C: FoV coordinates out of bounds, expected a number between [0;180]");
		return (1);
	}
	return (0);
}

int	get_light(t_rt *rt, char **split)
{
	t_light	*light;
	char	**coords;
	char	**rgb;

	if (check_format_light(split))
		return (1);
	light = malloc(sizeof(t_light));
	rt->light = light;
	coords = ft_split(split[1], ',');
	light->coords = malloc(sizeof(double) * 3);
	light->coords[X] = ft_atof(coords[0]);
	light->coords[Y] = ft_atof(coords[1]);
	light->coords[Z] = ft_atof(coords[2]);
	ft_free_split(&coords);
	light->brightness = ft_atof(split[2]);
	if (light->brightness < 0.0 || light->brightness > 1.0)
	{
		throw_error("L: Brightness out of bounds, expected number between 0.0 and 1.0");
		return (1);
	}
	rgb = ft_split(split[3], ',');
	light->rgb = malloc(sizeof(int) * 3);
	light->rgb[R] = ft_atoi(rgb[0]);
	light->rgb[G] = ft_atoi(rgb[1]);
	light->rgb[B] = ft_atoi(rgb[2]);
	ft_free_split(&rgb);
	if (light->rgb[R]> 255 || light->rgb[R] < 0
		|| light->rgb[G] > 255 || light->rgb[G] < 0
		|| light->rgb[B] > 255 || light->rgb[B] < 0)
	{
		throw_error("L: RGB out of bounds, expected integers between 0 and 255");
		return (1);
	}
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
	if (!ft_strncmp(split[0], "A", ft_strlen(split[0])))
	{
		if (rt->ambient == NULL)
			*flag = get_ambient(rt, split);
		else
		{
			throw_error("Duplicate instruction for A");
			*flag = 1;
		}
	}
	else if (!ft_strncmp(split[0], "C", ft_strlen(split[0])))
	{
		if (rt->camera == NULL)
			*flag = get_camera(rt, split);
		else
		{
			throw_error("Duplicate instruction for C");
			*flag = 1;
		}
	}
	else if (!ft_strncmp(split[0], "L", ft_strlen(split[0])))
	{
		if (rt->light == NULL)
			*flag = get_light(rt, split);
		else
		{
			throw_error("Duplicate instruction for L");
			*flag = 1;
		}
	}
	/*
	else if (!ft_strncmp(split[0], "sp", ft_strlen(split[0])))
		get_sphere(rt, split);
	else if (!ft_strncmp(split[0], "pl", ft_strlen(split[0])))
		get_plane(rt, split);
	else if (!ft_strncmp(split[0], "cy", ft_strlen(split[0])))
		get_cylinder(rt, split); 
	*/
	ft_free_split(&split);
	return (0);
}

int	load_file(char *file, t_rt *rt)
{
	int		fd;
	char	*line;
	int		flag;

	flag = 0;
	fd = open(file, O_RDONLY);
	line = get_next_line(fd);
	while (line && !flag)
	{
		load_data(line, rt, &flag);
		free(line);
		line = get_next_line(fd);
	}
	free(line);
	return (0);
}

int	main(int ac, char *av[])
{
	t_rt	*rt;

	rt = NULL;
	rt = malloc(sizeof(t_rt));
	if (!rt)
		return (1);
	rt->ambient = NULL;
	rt->camera = NULL;
	rt->light = NULL;
	if (ac != 2)
	{
		// error
		throw_error("Incorrect number of parameters");
	}
	else
	{
		//mandatory (open specific map)
		ft_printf("Should open map: %s\n", av[1]);
		load_file(av[1], rt);
		/* printf("Ambience ratio: %f\n", rt->ambient->ratio);
		printf("R, G, B: %d, %d, %d\n", rt->ambient->r, rt->ambient->g, rt->ambient->b); */
	}
	free(rt->ambient);
	free(rt);
	return (0);
}
