/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 13:40:49 by okraus            #+#    #+#             */
/*   Updated: 2023/12/11 18:14:51 by plouda           ###   ########.fr       */
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

void	shift_camera(t_master *master, mlx_key_data_t keydata)
{
	if (keydata.key == MLX_KEY_RIGHT)
		master->rt->camera->coords[X] += 5;
	if (keydata.key == MLX_KEY_LEFT)
		master->rt->camera->coords[X] -= 5;
	if (keydata.key == MLX_KEY_UP)
		master->rt->camera->coords[Y] += 5;
	if (keydata.key == MLX_KEY_DOWN)
		master->rt->camera->coords[Y] -= 5;
	if (keydata.key == MLX_KEY_KP_ADD)
		master->rt->camera->coords[Z] -= 5;
	if (keydata.key == MLX_KEY_KP_SUBTRACT)
		master->rt->camera->coords[Z] += 5;
	find_rays(master);
}

void	rotate_camera(t_master *master, mlx_key_data_t keydata)
{
	if (keydata.key == MLX_KEY_Q)
		master->rt->camera->normal->x += 0.05;
	if (keydata.key == MLX_KEY_E)
		master->rt->camera->normal->x -= 0.05;
	if (keydata.key == MLX_KEY_W)
		master->rt->camera->normal->y -= 0.05;
	if (keydata.key == MLX_KEY_S)
		master->rt->camera->normal->y += 0.05;
	normalize(master->rt->camera->normal, master->rt->camera->normal->x, \
	master->rt->camera->normal->y, master->rt->camera->normal->z);
	find_rays(master);
}

void	keyhook(mlx_key_data_t keydata, void *param)
{
	t_master	*master;

	master = param;
	if ((keydata.key == MLX_KEY_RIGHT
			|| keydata.key == MLX_KEY_LEFT
			|| keydata.key == MLX_KEY_UP
			|| keydata.key == MLX_KEY_DOWN
			|| keydata.key == MLX_KEY_KP_ADD
			|| keydata.key == MLX_KEY_KP_SUBTRACT)
		&& keydata.action != MLX_RELEASE)
		shift_camera(master, keydata);
	if ((keydata.key == MLX_KEY_Q
			|| keydata.key == MLX_KEY_E
			|| keydata.key == MLX_KEY_W
			|| keydata.key == MLX_KEY_S)
		&& keydata.action != MLX_RELEASE)
		rotate_camera(master, keydata);
}

static void	loop(mlx_t *mlx, t_master *master)
{
	mlx_key_hook(mlx, &keyhook, master);
	mlx_loop(mlx);
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
		
		mlx = mlx_init(WIDTH, HEIGHT, "miniRT", false);
		mlx_set_window_limit(mlx, 250, 250, 10000, 10000);
		img = mlx_new_image(mlx, mlx->width, mlx->height);
		mlx_image_to_window(mlx, img, 0, 0);
		master->mlx = mlx;
		master->img = img;
		master->rt = rt;
		find_rays(master);
		loop(mlx, master);
	}

	free_objects(rt);
	free(rt);
	free(master);
	mlx_delete_image(mlx, img);
	mlx_terminate(mlx);
	return (0);
}
