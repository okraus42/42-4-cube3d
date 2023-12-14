/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/04 15:43:08 by okraus            #+#    #+#             */
/*   Updated: 2023/12/14 09:40:13 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H

# define MINIRT_H
# define HEIGHT 720
# define WIDTH 1280
# define X 0
# define Y 1
# define Z 2
# define R 0
# define G 1
# define B 2
# define E_SPEC "Invalid number of specifiers"
# define E_RATIO "Invalid ratio format"
# define E_RGB "Invalid RGB format"
# define E_COORD "Invalid coordinate format"
# define E_VECT "Invalid vector format"
# define E_FOV "Invalid FOV format"
# define E_BRIGHT "Invalid brightness format"
# define E_DIA "Invalid diameter format"
# define E_HEIGHT "Invalid height format"
# define E_TRIAD_INTFLOAT "3 integers/floats separated by commas"
# define E_MAX_DIGITS "max. 4 integer digits and 5 decimals"
# define E_TRIAD_INT "3 integers, separated by commas"
# define E_INTFLOAT "int or float"
# define E_RATIO_RANGE "Ratio value out of bounds"
# define E_RGB_RANGE "RGB value out of bounds"
# define E_VECT_RANGE "Normalized vector coordinate value out of bounds"
# define E_FOV_RANGE "FOV value out of bounds"
# define E_BRIGHT_RANGE "Brightness value out of bounds"
# define E_DIA_RANGE "Diameter value too low"
# define E_HEIGHT_RANGE "Height value too low"
# define E_RANGE_INT "integers in range (0;255)"
# define E_RANGE_POS "a value in range (0.0;1.0)"
# define E_RANGE_NORM "values in range (-1.0;1.0)"
# define E_RANGE_STRICT "a strictly positive value"

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

typedef enum e_object
{
	EMPTY,
	SPHERE,
	PLANE,
	CYLINDER,
	DISC
}				t_object;

typedef struct s_vect2f
{
	double	x;
	double	y;
}				t_vect2f;

typedef struct s_vect3f
{
	double	x;
	double	y;
	double	z;
}				t_vect3f;

typedef struct s_quat
{
	double	q0;
	double	q1;
	double	q2;
	double	q3;
}				t_quat;

typedef struct s_ray
{
	t_vect3f	origin;
	t_vect3f	direction;
}				t_ray;

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
	t_vect3f	*normal;
	t_vect3f	*right;
	t_vect3f	*up;
	double		**matrix;
	t_quat		*quat;
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
	t_vect3f	*normal;
}				t_plane;

typedef	struct s_disc
{
	double	*coords; // x,y,z coordinates of center
	double	*nvect; // 3d normalized vector; [-1;1],[-1;1],[-1;1]
	double	radius;
	int		*rgb;
	t_vect3f	*normal;
}				t_disc;

typedef struct s_cylinder
{
	double	*coords; // x,y,z coordinates of center
	double	*nvect; // 3d normalized vector of an axis; [-1;1],[-1;1],[-1;1]
	double	diameter;
	double	height;
	int		*rgb;
	t_disc	*botcap;
	t_disc	*topcap;
	t_vect3f	*normal;
}				t_cylinder;

typedef struct s_rt
{
	t_ambient	*ambient;
	t_camera	*camera;
	t_light		*light;
	t_sphere	**spheres;
	t_plane		**planes;
	t_cylinder	**cylinders;
	int			n_spheres;
	int			n_planes;
	int			n_cylinders;
}				t_rt;

typedef struct s_master
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	t_rt		*rt;
}				t_master;

// Initialize objects
void	init_ambient(t_rt *rt);
void	init_light(t_rt *rt);
void	init_camera(t_rt *rt);
void	init_spheres(t_rt *rt, int *ids);
void	init_planes(t_rt *rt, int *ids);
void	init_cylinders(t_rt *rt, int *ids);

// Identifier check
int	*init_ids(void);
int	count_identifiers(char *line, int *ids, int *flag);
int	check_identifiers(int fd, int *ids, int *flag);
void	check_missing(int *ids, int *flag);
void	check_duplicates(int *ids, int *flag);
void	get_identifiers(char **split, int *ids, int *flag);

// Format check
int	is_float_triad(char *str);
int	is_rgb_format(char *str);
int	is_floatable(char *str);
int	is_integer(char *str);
int	float_in_range(char *str); // format 9999.99999 allowed
int	triad_in_range(char *triad);
int	has_spec_count(char **split, int target);

int	check_format_ambient(char **split);
int	check_format_camera(char **split);
int	check_format_light(char **split);
int	check_format_sphere(char **split);
int	check_format_plane(char **split);
int	check_format_cylinder(char **split);

// Data loading
void	get_data_from_line(char *line, t_rt *rt, int *flag);
void	fill_objects(t_rt *rt, char **split, int *flag);
int		fill_ambient(t_rt *rt, char **split);
int		fill_camera(t_rt *rt, char **split);
int		fill_light(t_rt *rt, char **split);
int		fill_plane(t_rt *rt, char **split);
int		fill_sphere(t_rt *rt, char **split);
int		fill_cylinder(t_rt *rt, char **split);

double	ft_atof(char *str);
int		id_err(char *id, char *err_str, char *details);
void	free_objects(t_rt *rt);

int	get_rgb(int *rgb, char *triad);
int	get_coords(double *coords, char *triad);
int	get_nvect(double *nvect, char *triad);

void	init_objects(t_rt *rt, int *ids);
int	print_contents(t_rt *rt);

// Free objects
void	free_ambient(t_rt *rt);
void	free_light(t_rt *rt);
void	free_camera(t_rt *rt);
void	free_spheres(t_rt *rt);
void	free_planes(t_rt *rt);
void	free_cylinders(t_rt *rt);

// Ray casting
//void	cast_rays(t_master *master);
void	find_rays(t_master *master);

double	deg(double rad);
double	rad(double deg);
void	normalize(t_vect3f *vect, double px, double py, double pz);

// Cylinder caps functions
void	init_discs(t_cylinder *cylinder);
void	define_botcap(t_cylinder *cylinder);
void	define_topcap(t_cylinder *cylinder);
void	get_discs(t_cylinder *cylinder);
void	free_discs(t_cylinder *cylinder);

// Camera
void	set_camera(t_camera *camera);
t_vect3f	shift_origin(double **cam);
void	change_ray_direction(double **cam, t_vect3f *direction, t_vect3f temp);
void	shift_camera(t_master *master, mlx_key_data_t keydata);
void	rotate_camera(t_master *master, mlx_key_data_t keydata);
void	update_camera_matrix(t_camera *camera);

// Quaternions
t_quat	get_rot_quat(t_vect3f axis, double angle);
t_quat	get_point_quat(t_vect3f axis);
t_quat	get_inverse_quat(t_quat quat);
t_quat	mult_quat(t_quat i, t_quat j);

t_vect3f	cross_product(t_vect3f vect1, t_vect3f vect2);

#endif
