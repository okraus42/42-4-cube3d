/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 13:40:49 by okraus            #+#    #+#             */
/*   Updated: 2024/03/05 11:10:06 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minirt.h"

static void	loop(mlx_t *mlx, t_master *master)
{
	mlx_key_hook(mlx, &keyhook, master);
	mlx_mouse_hook(mlx, &mousehook, master);
	mlx_loop(mlx);
}

static void	init_options(t_master *master)
{
	t_options	*options;

	options = ft_calloc(1, sizeof(t_options));
	options->mode = DEFAULT;
	options->spec_highlight_size = 512;
	master->options = options;
}

static void	minirt2(t_master *master, t_rt *rt)
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	int			i;

	mlx = mlx_init(WIDTH, HEIGHT, "miniRT", true);
	mlx_set_window_limit(mlx, 100, 100, WIDTH, HEIGHT);
	img = mlx_new_image(mlx, mlx->width, mlx->height);
	mlx_image_to_window(mlx, img, 0, 0);
	master->mlx = mlx;
	master->img = img;
	master->rt = rt;
	i = 0;
	while (i < STRINGS)
	{
		ft_sprintf(master->str[i], "");
		master->string[i] = mlx_put_string(master->mlx,
				master->str[i], 10, 5 + 20 * i);
		++i;
	}
	find_rays(master);
	ft_draw_string(master);
	loop(mlx, master);
	mlx_delete_image(mlx, img);
	mlx_terminate(mlx);
}

static void	minirt(t_master *master, t_rt *rt, char *s)
{
	int	fd;

	ft_printf("Should open map: %s\n", s);
	fd = open_file(s);
	if (fd < 0)
	{
		free(rt);
		free(master->options);
		free(master);
		exit(EXIT_FAILURE);
	}
	master->w_height = HEIGHT;
	master->w_width = WIDTH;
	if (!load_file(s, rt, fd))
		minirt2(master, rt);
}

int	main(int ac, char *av[])
{
	t_rt		*rt;
	t_master	*master;

	master = ft_calloc(1, sizeof(t_master));
	init_options(master);
	rt = ft_calloc(1, sizeof(t_rt));
	if (!rt || !master)
		return (EXIT_FAILURE);
	if (ac != 2)
	{
		id_err(NULL, "Incorrect number of parameters", NULL);
		free(rt);
		free(master->options);
		free(master);
		return (1);
	}
	else
		minirt(master, rt, av[1]);
	free_objects(rt);
	free(rt);
	free(master->options);
	free(master);
	return (EXIT_SUCCESS);
}
