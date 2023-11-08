/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/04 15:43:08 by okraus            #+#    #+#             */
/*   Updated: 2023/11/08 09:20:13 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H

# define MINIRT_H

// INCLUDES

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdint.h>
# include <stdarg.h>
# include <sys/wait.h>
# include <time.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <stdbool.h>
# include <math.h>
# include "../.MLX42/include/MLX42/MLX42.h"
# include "../libft/header/libft.h"

typedef struct s_ambient
{
	double	ratio; // 0.0-1.0
	int		r;
	int		g;
	int		b;
}				t_ambient;

typedef struct s_camera
{
	double	viewpoint; // x,y,z coordinates
	double	nvect; // 3d normalized vector; [-1;1],[-1;1],[-1,1]
	int		fov; // 0-180
}				t_camera;

typedef struct s_light
{
	double	lightpoint; // x,y,z coordinates
	double	brightness; // 0.0-1.0
	int		r;
	int		g;
	int		b;
}				t_light;

typedef struct s_sphere
{
	double	center; // x,y,z coordinates
	double	diameter;
	int		r;
	int		g;
	int		b;
}				t_sphere;

typedef struct s_plane
{
	double	point; // x,y,z coordinates
	double	nvect; // 3d normalized vector; [-1;1],[-1;1],[-1,1]
	int		r;
	int		g;
	int		b;
}				t_plane;

typedef struct s_cylinder
{
	double	center; // x,y,z coordinates
	double	nvect; // 3d normalized vector of an axis; [-1;1],[-1;1],[-1,1]
	double	diameter;
	double	height;
	int		r;
	int		g;
	int		b;
}				t_cylinder;

typedef struct s_rt
{
	t_ambient	*ambient;
	t_camera	*camera;
	t_light		*light;
	t_sphere	*spheres;
	t_plane		*planes;
	t_cylinder	*cylinders;
}				t_rt;

typedef struct s_master
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	t_rt		*rt;
}				t_master;

double	ft_atof(char *str);
int	throw_error(char *str);



#endif
