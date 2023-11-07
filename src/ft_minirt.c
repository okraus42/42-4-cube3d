/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minirt.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 13:40:49 by okraus            #+#    #+#             */
/*   Updated: 2023/11/07 13:58:22 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minirt.h"

int	get_ambient(t_rt *rt, char **split)
{
	t_ambient	*ambient;
	char		**rgb;

	ambient = malloc(sizeof(t_ambient));
 	rt->ambient = ambient;
	ambient->ratio = ft_atof(split[1]);
	printf("Ratio: %f\n", ambient->ratio);
	rgb = ft_split(split[2], ',');
	ambient->r = ft_atoi(rgb[0]);
	ambient->g = ft_atoi(rgb[1]);
	ambient->b = ft_atoi(rgb[2]);
	free(rgb[0]);
	free(rgb[1]);
	free(rgb[2]);
	free(rgb);
	return (0);
}

int	load_data(char *line, t_rt *rt)
{
	char	**split;
	//char	*id;

	split = ft_split(line, ' ');
	if (!ft_strncmp(split[0], "A", ft_strlen(split[0])))
	{
		//ft_printf("Loading ambience...\n");
		get_ambient(rt, split);
	}
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
	else
		return (1);
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
		printf("Ambience ratio: %f\n", rt->ambient->ratio);
		printf("R, G, B: %d, %d, %d\n", rt->ambient->r, rt->ambient->g, rt->ambient->b);
	}
	free(rt);
	return (0);
}
