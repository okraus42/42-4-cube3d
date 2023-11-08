/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minirt.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 13:40:49 by okraus            #+#    #+#             */
/*   Updated: 2023/11/08 11:09:53 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minirt.h"

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

int	ratio_in_range(char *str)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	if (str[i] == '-')
		i++;
	while (str[i] && ft_isdigit(str[i]))
		i++;
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
	if (!ratio_in_range(split[1]))
	{
		throw_error("A: Wrong ratio format, max. 5 decimal places allowed");
		return (1);
	}
	if (!is_rgb_format(split[2]))
	{
		throw_error("A: Wrong RGB format, expected three integers separated by commas");
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
		dprintf(2, "%f\n", ambient->ratio);
		throw_error("A: Ratio out of bounds, expected number between 0.0 and 1.0");
		return (1);
	}
	rgb = ft_split(split[2], ',');
	ambient->r = ft_atoi(rgb[0]);
	ambient->g = ft_atoi(rgb[1]);
	ambient->b = ft_atoi(rgb[2]);
	ft_free_split(&rgb);
	if (ambient->r > 255 || ambient->r < 0
	|| ambient->g > 255 || ambient->g < 0
	|| ambient->b > 255 || ambient->b < 0)
	{
		throw_error("A: RGB out of bounds, expected integers between 0 and 255");
		return (1);
	}
	return (0);
}

int	load_data(char *line, t_rt *rt)
{
	char	**split;
	char	*trimmed;
	int		i;
	//char	*id;

	i = 0;
	trimmed = ft_strtrim(line, " \n");
	split = ft_split(trimmed, ' ');
	free(trimmed);
	if (!ft_strncmp(split[0], "A", ft_strlen(split[0])))
		get_ambient(rt, split);
	/* else if (!ft_strncmp(split[0], "C", ft_strlen(split[0])))
		get_camera(rt, split);
	else if (!ft_strncmp(split[0], "L", ft_strlen(split[0])))
		get_light(rt, split);
	else if (!ft_strncmp(split[0], "sp", ft_strlen(split[0])))
		get_sphere(rt, split);
	else if (!ft_strncmp(split[0], "pl", ft_strlen(split[0])))
		get_plane(rt, split);
	else if (!ft_strncmp(split[0], "cy", ft_strlen(split[0])))
		get_cylinder(rt, split); */
	ft_free_split(&split);
	return (0);
}

int	load_file(char *file, t_rt *rt)
{
	int		fd;
	char	*line;

	fd = open(file, O_RDONLY);
	line = get_next_line(fd);
	while (line)
	{
		load_data(line, rt);
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
	if (ac != 2)
	{
		// error
		ft_dprintf(2, "Incorrect number of parameters\n");
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
