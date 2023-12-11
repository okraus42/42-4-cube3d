/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 13:40:49 by okraus            #+#    #+#             */
/*   Updated: 2023/12/11 09:40:58 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minirt.h"

void	get_data_from_line(char *line, t_rt *rt, int *flag)
{
	char	**split;
	char	*trimmed;

	trimmed = ft_strtrim(line, " \n");
	split = ft_split(trimmed, ' ');
	free(trimmed);
	if (ft_strlen(split[0]) > 0)
		fill_objects(rt, split, flag);
	ft_free_split(&split);
}

void	load_data(t_rt *rt, char *file, int *flag)
{
	int		fd;
	char	*line;

	fd = open(file, O_RDONLY);
	line = get_next_line(fd);
	while (line && !(*flag))
	{
		get_data_from_line(line, rt, flag);
		free(line);
		line = get_next_line(fd);
	}
	free(line);
	close(fd);
}

int	load_file(char *file, t_rt *rt)
{
	int		fd;
	int		flag;
	int		*ids;

	flag = 0;
	fd = open(file, O_RDONLY);
	ids = init_ids();
	check_identifiers(fd, ids, &flag);
	close(fd);
	init_objects(rt, ids);
	free(ids);
	if (!flag)
		load_data(rt, file, &flag);
	return (flag);
}

int	main(int ac, char *av[])
{
	t_rt		*rt;
	t_master	*master;
	mlx_t		*mlx;
	mlx_image_t	*img;

	master = malloc(sizeof(t_master));
	rt = NULL;
	rt = malloc(sizeof(t_rt));
	if (!rt)
		return (1);
	if (ac != 2)
	{
		id_err(NULL, "Incorrect number of parameters", NULL);
		free(rt);
		return (1);
	}
	else
	{
		ft_printf("Should open map: %s\n", av[1]);
		if (!load_file(av[1], rt))
			print_contents(rt);
		//wrong map name segfaults!!
		
		mlx = mlx_init(WIDTH, HEIGHT, "miniRT", true);
		mlx_set_window_limit(mlx, 250, 250, 10000, 10000);
		img = mlx_new_image(mlx, mlx->width, mlx->height);
		mlx_image_to_window(mlx, img, 0, 0);
		master->mlx = mlx;
		master->img = img;
		master->rt = rt;
		//cast_rays(master);
		find_rays(master);
		mlx_loop(mlx);
	}

	free_objects(rt);
	free(rt);
	free(master);
	mlx_delete_image(mlx, img);
	mlx_terminate(mlx);
	return (0);
}
