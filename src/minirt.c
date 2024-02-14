/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 13:40:49 by okraus            #+#    #+#             */
/*   Updated: 2024/02/14 11:54:06 by plouda           ###   ########.fr       */
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

void	get_texture_data_from_line(char *line, t_rt *rt, int *flag)
{
	char	**split;
	char	*trimmed;

	trimmed = ft_strtrim(line, " \n");
	split = ft_split(trimmed, ' ');
	free(trimmed);
	if (ft_strlen(split[0]) > 0)
	{
		if (!ft_strncmp(split[0], ".ch/", 4))
			*flag = fill_checkerboard(rt, split);
	}
	ft_free_split(&split);
}

void	get_texture_data(t_rt *rt, char *file, int *flag)
{
	int		fd;
	char	*line;

	fd = open(file, O_RDONLY);
	line = get_next_line(fd);
	while (line && !(*flag))
	{
		get_texture_data_from_line(line, rt, flag);
		free(line);
		line = get_next_line(fd);
	}
	free(line);
	close(fd);
}

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
		load_data(rt, file, &flag);
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

int	iterate_highlighted_object(t_rt *rt)
{
	int	i;

	i = 0;
	while (i < rt->n_spheres)
	{
		if (rt->spheres[i]->mode == HIGHLIGHT)
		{
			return (SPHERE | (i << 8));
		}
		i++;
	}
	i = 0;
	while (i < rt->n_planes)
	{
		if (rt->planes[i]->mode == HIGHLIGHT)
		{
			return (PLANE | (i << 8));
		}
		i++;
	}
	i = 0;
	while (i < rt->n_cylinders)
	{
		if (rt->cylinders[i]->mode == HIGHLIGHT)
		{
			return (CYLINDER | (i << 8));
		}
		i++;
	}
	i = 0;
	while (i < rt->n_cones)
	{
		if (rt->cones[i]->mode == HIGHLIGHT)
		{
			return (CONE | (i << 8));
		}
		i++;
	}
	while (i < rt->n_lights)
	{
		if (rt->light_spheres[i]->mode == HIGHLIGHT)
		{
			return (LIGHT | (i << 8));
		}
		i++;
	}
	return (0);
}

void	ft_draw_string(t_master *master)
{
	int	i;

	char	*s[STRINGS];

	if (master->options->mode == DEFAULT)
		s[0] = ft_strdup("MODE: DEFAULT");
	else if (master->options->mode == OBJECT_CHOICE)
		s[0] = ft_strdup("MODE: OBJECT_CHOICE");
	else if (master->options->mode == HIGHLIGHT)
		s[0] = ft_strdup("MODE: HIGHLIGHT");
	else if (master->options->mode == LIGHTING)
		s[0] = ft_strdup("MODE: LIGHTING");
	else if (master->options->mode == CAMERA)
		s[0] = ft_strdup("MODE: CAMERA");
	else
		s[0] = ft_strdup("UNKNOWN MODE");
	i = iterate_highlighted_object(master->rt);
	if ((i & 0xFF) == SPHERE)
		ft_sprintf(master->str[1], "HIGHLIGHTED: SPHERE: %i", i >> 8);
	else if ((i & 0xFF) == PLANE)
		ft_sprintf(master->str[1], "HIGHLIGHTED: PLANE: %i", i >> 8);
	else if ((i & 0xFF) == CYLINDER)
		ft_sprintf(master->str[1], "HIGHLIGHTED: CYLINDER: %i", i >> 8);
	else if ((i & 0xFF) == CONE)
		ft_sprintf(master->str[1], "HIGHLIGHTED: CONE: %i", i >> 8);
	else if ((i & 0xFF) == LIGHT)
		ft_sprintf(master->str[1], "HIGHLIGHTED: LIGHT: %i", i >> 8);
	else
		ft_sprintf(master->str[1], " ");
	//ft_sprintf(master->str[2], "");
	/* if (master->rt->cones[0])
		sprintf(master->str[2], "Cone normal   x %8f y %8f z %8f", master->rt->cones[0]->normal->x, master->rt->cones[0]->normal->y, master->rt->cones[0]->normal->z);
	if (master->rt->cones[0])
		sprintf(master->str[3], "Cone right    x %8f y %8f z %8f", master->rt->cones[0]->right->x, master->rt->cones[0]->right->y, master->rt->cones[0]->right->z);
	if (master->rt->cones[0])
		sprintf(master->str[4], "Cone up       x %8f y %8f z %8f", master->rt->cones[0]->up->x, master->rt->cones[0]->up->y, master->rt->cones[0]->up->z);
	if (master->rt->cones[0])
		sprintf(master->str[5], "Camera normal x %8f y %8f z %8f", master->rt->camera->normal->x, master->rt->camera->normal->y, master->rt->camera->normal->z);
	if (master->rt->cones[0])
		sprintf(master->str[6], "Camera right  x %8f y %8f z %8f", master->rt->camera->right->x, master->rt->camera->right->y, master->rt->camera->right->z);
	if (master->rt->cones[0])
		sprintf(master->str[7], "Camera up     x %8f y %8f z %8f", master->rt->camera->up->x, master->rt->camera->up->y, master->rt->camera->up->z);
	//ft_sprintf(master->str[8], "WHAT IS GOING ON???"); */
	mlx_delete_image(master->mlx, master->string[0]);
	mlx_delete_image(master->mlx, master->string[1]);
	/* mlx_delete_image(master->mlx, master->string[2]);
	mlx_delete_image(master->mlx, master->string[3]);
	mlx_delete_image(master->mlx, master->string[4]);
	mlx_delete_image(master->mlx, master->string[5]);
	mlx_delete_image(master->mlx, master->string[6]);
	mlx_delete_image(master->mlx, master->string[7]); */
	//mlx_delete_image(master->mlx, master->string[8]);
	master->string[0] = mlx_put_string(master->mlx, s[0], 10, 5);
	master->string[1] = mlx_put_string(master->mlx, master->str[1], 10, 25);
	/* master->string[2] = mlx_put_string(master->mlx, master->str[2], 10, 45);
	master->string[3] = mlx_put_string(master->mlx, master->str[3], 10, 65);
	master->string[4] = mlx_put_string(master->mlx, master->str[4], 10, 85);
	master->string[5] = mlx_put_string(master->mlx, master->str[5], 10, 105);
	master->string[6] = mlx_put_string(master->mlx, master->str[6], 10, 125);
	master->string[7] = mlx_put_string(master->mlx, master->str[7], 10, 145); */
	//master->string[8] = mlx_put_string(master->mlx, master->str[8], 10, 165);
	free (s[0]);
}

void	keyhook(mlx_key_data_t keydata, void *param)
{
	t_master	*master;

	master = param;
	if (master->options->mode == DEFAULT
			&& !keydata.modifier && keydata.action != MLX_RELEASE
			&& (keydata.key == MLX_KEY_RIGHT
			|| keydata.key == MLX_KEY_LEFT
			|| keydata.key == MLX_KEY_UP
			|| keydata.key == MLX_KEY_DOWN
			|| keydata.key == MLX_KEY_MINUS
			|| keydata.key == MLX_KEY_EQUAL
			|| keydata.key == MLX_KEY_PAGE_UP
			|| keydata.key == MLX_KEY_PAGE_DOWN))
		shift_camera(master, keydata);
	else if (master->options->mode == DEFAULT
			&& !keydata.modifier && keydata.action != MLX_RELEASE
			&& (keydata.key == MLX_KEY_A
			|| keydata.key == MLX_KEY_D
			|| keydata.key == MLX_KEY_W
			|| keydata.key == MLX_KEY_S
			|| keydata.key == MLX_KEY_Q
			|| keydata.key == MLX_KEY_E))
		rotate_camera(master, keydata);
	else if (keydata.modifier == MLX_CONTROL 
			&& keydata.key == MLX_KEY_O
			&& keydata.action != MLX_RELEASE
			&& master->options->mode == DEFAULT)
		{
			master->options->mode = OBJECT_CHOICE;
			choose_object(master, keydata);
		}
	else if ((master->options->mode == OBJECT_CHOICE 
			|| master->options->mode == HIGHLIGHT)
			&& keydata.modifier == MLX_SHIFT
			&& keydata.key == MLX_KEY_UP
			&& keydata.action != MLX_RELEASE)
		choose_object(master, keydata);
	else if (master->options->mode == HIGHLIGHT
			&& keydata.action != MLX_RELEASE
			&& !keydata.modifier
			&& (keydata.key == MLX_KEY_RIGHT
			|| keydata.key == MLX_KEY_LEFT
			|| keydata.key == MLX_KEY_UP
			|| keydata.key == MLX_KEY_DOWN
			|| keydata.key == MLX_KEY_PAGE_UP
			|| keydata.key == MLX_KEY_PAGE_DOWN
			|| keydata.key == MLX_KEY_A
			|| keydata.key == MLX_KEY_D
			|| keydata.key == MLX_KEY_W
			|| keydata.key == MLX_KEY_S
			|| keydata.key == MLX_KEY_Q
			|| keydata.key == MLX_KEY_E))
		rotate_objects(master, keydata);
	else if (keydata.modifier == MLX_CONTROL 
			&& keydata.key == MLX_KEY_L
			&& keydata.action != MLX_RELEASE
			&& master->options->mode == DEFAULT)
			{
				master->options->mode = LIGHTING;
				if (master->rt->n_lights > 0)
					master->rt->light_spheres[0]->mode = HIGHLIGHT;
			}
	else if ((master->options->mode == LIGHTING)
			&& keydata.modifier == MLX_SHIFT
			&& keydata.key == MLX_KEY_UP
			&& keydata.action != MLX_RELEASE)
		choose_object(master, keydata);
	else if (master->options->mode == LIGHTING
			&& keydata.action != MLX_RELEASE
			&& !keydata.modifier
			&& (keydata.key == MLX_KEY_RIGHT
			|| keydata.key == MLX_KEY_LEFT
			|| keydata.key == MLX_KEY_UP
			|| keydata.key == MLX_KEY_DOWN
			|| keydata.key == MLX_KEY_PAGE_UP
			|| keydata.key == MLX_KEY_PAGE_DOWN
			|| keydata.key == MLX_KEY_COMMA
			|| keydata.key == MLX_KEY_PERIOD
			|| keydata.key == MLX_KEY_G
			|| keydata.key == MLX_KEY_H))
		manipulate_light(master, keydata);
	else if (keydata.key == MLX_KEY_BACKSPACE && keydata.action != MLX_RELEASE)
		reset_to_default(master);
	else if ((keydata.key == MLX_KEY_ESCAPE) && keydata.action != MLX_RELEASE)
		mlx_close_window(master->mlx);
	ft_draw_string(master);
}

t_rayfinder	trace_singular_object_ray(t_master *master, int32_t xpos, int32_t ypos)
{
	t_rayfinder	rf;
	t_ray	*ray;

	ray = malloc(sizeof(t_ray));
	update_camera_matrix(master->rt->camera);
	rf = init_rayfinder(master);
	update_ray_direction(&rf, ray, xpos, ypos);
	find_intersections(master, *ray, &rf, PRIMARY);
	printf("object ptr: %p\n", rf.object_ptr);
	free(ray);
	return (rf);
}

void	mousehook(mouse_key_t button, action_t action, modifier_key_t mods, void *param)
{
	t_master	*master;
	int32_t		xpos;
	int32_t		ypos;
	t_rayfinder	rf;

	master = param;
	(void)mods;
	if (button == MLX_MOUSE_BUTTON_LEFT
		&& (action != MLX_RELEASE && action != MLX_REPEAT))
	{
		mlx_get_mouse_pos(master->mlx, &xpos, &ypos);
		rf = trace_singular_object_ray(master, xpos, ypos);
		reset_to_default(master);
		set_highlight_from_reference(master, rf);
		ft_draw_string(master);
	}
}

static void	loop(mlx_t *mlx, t_master *master)
{
	mlx_key_hook(mlx, &keyhook, master);
	mlx_mouse_hook(mlx, &mousehook, master);
	mlx_loop(mlx);
}

void	init_options(t_master *master)
{
	t_options	*options;

	options = ft_calloc(1, sizeof(t_options));
	options->mode = DEFAULT;
	options->light_intensity = 100;
	options->spec_highlight_size = 512;
	options->glossiness = 0.5;
	master->options = options;
}

void	print_list(void *content, t_object flag)
{
	printf("Address of obj %i: %p\n", flag, content);
}

void	create_object_list(t_master *master)
{
	int			i;
	t_objlist	*object;
	//t_objlist	*tmp;

	//master->obj_list = ft_calloc(1, sizeof(t_objlist));
	master->obj_list = NULL;
	i = 0;
	while (i < master->rt->n_spheres)
	{
		object = ft_objlst_new(master->rt->spheres[i], SPHERE);
		ft_objlst_add_back(&master->obj_list, object);
		i++;
	}
	i = 0;
	while (i < master->rt->n_planes)
	{
		object = ft_objlst_new(master->rt->planes[i], PLANE);
		ft_objlst_add_back(&master->obj_list, object);
		i++;
	}
	i = 0;
	while (i < master->rt->n_cylinders)
	{
		object = ft_objlst_new(master->rt->cylinders[i], CYLINDER);
		ft_objlst_add_back(&master->obj_list, object);
		object = ft_objlst_new(master->rt->cylinders[i]->botcap, DISC);
		ft_objlst_add_back(&master->obj_list, object);
		object = ft_objlst_new(master->rt->cylinders[i]->topcap, DISC);
		ft_objlst_add_back(&master->obj_list, object);
		i++;
	}
	i = 0;
	while (i < master->rt->n_cones)
	{
		object = ft_objlst_new(master->rt->cones[i], CONE);
		ft_objlst_add_back(&master->obj_list, object);
		object = ft_objlst_new(master->rt->cones[i]->base, DISC);
		ft_objlst_add_back(&master->obj_list, object);
		object = ft_objlst_new(master->rt->cones[i]->pinnacle, DISC);
		ft_objlst_add_back(&master->obj_list, object);
		i++;
	}
	i = 0;
	while (i < master->rt->n_lights)
	{
		object = ft_objlst_new(master->rt->light_spheres[i], LIGHT);
		ft_objlst_add_back(&master->obj_list, object);
		i++;
	}

	//tmp = master->obj_list;
	//ft_objlst_iter(tmp, &print_list);

	/* i = 0;
	if (tmp)
	{
		while (tmp)
		{
			printf("Flag no. %i of object: %i\n", i++, tmp->object);
			tmp = tmp->next;
		}
	} */
	//printf("Flag no. %i of object: %i\n", i++, master->obj_list->next->object);
}

int	main(int ac, char *av[])
{
	t_rt		*rt;
	t_master	*master;
	mlx_t		*mlx;
	mlx_image_t	*img;
	int			fd;
	int			i;
	//int32_t	xpos;
	//int32_t	ypos;

	fd = 0;
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
	{
		ft_printf("Should open map: %s\n", av[1]);
		fd = open_file(av[1]);
		if (fd < 0)
		{
			free(rt);
			free(master);
			return (EXIT_FAILURE);
		}
		if (!load_file(av[1], rt, fd))
		{
			print_contents(rt);
			mlx = mlx_init(WIDTH, HEIGHT, "miniRT", false);
			mlx_set_window_limit(mlx, 250, 250, 10000, 10000);
			img = mlx_new_image(mlx, mlx->width, mlx->height);
			mlx_image_to_window(mlx, img, 0, 0);
			master->mlx = mlx;
			master->img = img;
			master->rt = rt;
			i = 0;
			while (i < STRINGS)
			{
				ft_sprintf(master->str[i], "");
				master->string[i] = mlx_put_string(master->mlx, master->str[i], 10, 5 + 20 * i);
				++i;
			}
			create_object_list(master);
			find_rays(master);
			ft_draw_string(master); // maybe here?
			//mlx_get_window_pos(master->mlx, &xpos, &ypos);
			//printf("xpos: %i\nypos: %i\n", xpos, ypos);
			loop(mlx, master);
			mlx_delete_image(mlx, img);
			mlx_terminate(mlx);
		}
	}
	free_objects(rt);
	free(rt);
	free(master->options);
	free(master);
	return (EXIT_SUCCESS);
}
