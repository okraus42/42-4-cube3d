/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/04 15:43:08 by okraus            #+#    #+#             */
/*   Updated: 2023/11/09 15:40:32 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H

# define MINIRT_H
# define X 0
# define Y 1
# define Z 2
# define R 0
# define G 1
# define B 2

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
	int		*rgb;
}				t_ambient;

typedef struct s_camera
{
	double	*coords; // x,y,z coordinates of viewpoint
	double	*nvect; // 3d normalized vector; [-1;1],[-1;1],[-1;1]
	int		fov; // 0-180
}				t_camera;

typedef struct s_light
{
	double	*coords; // x,y,z coordinates of lightpoint
	double	brightness; // 0.0-1.0
	int		*rgb;
}				t_light;

typedef struct s_sphere
{
	double	*coords; // x,y,z coordinates of center
	double	diameter;
	int		*rgb;
}				t_sphere;

typedef struct s_plane
{
	double	*coords; // x,y,z coordinates of point
	double	*nvect; // 3d normalized vector; [-1;1],[-1;1],[-1;1]
	int		*rgb;
}				t_plane;

typedef struct s_cylinder
{
	double	*coords; // x,y,z coordinates of center
	double	*nvect; // 3d normalized vector of an axis; [-1;1],[-1;1],[-1;1]
	double	diameter;
	double	height;
	int		*rgb;
}				t_cylinder;

typedef struct s_rt
{
	t_ambient	*ambient;
	t_camera	*camera;
	t_light		*light;
	t_sphere	**spheres;
	int			n_spheres;
	t_plane		**planes;
	int			n_planes;
	t_cylinder	**cylinders;
	int			n_cylinders;
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
