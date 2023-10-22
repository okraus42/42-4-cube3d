/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/04 15:43:08 by okraus            #+#    #+#             */
/*   Updated: 2023/10/22 13:59:36 by okraus           ###   ########.fr       */
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

//	DEFINITIONS

//	ENUMS

//	STRUCTURES

// typedef struct map_s
// {
// 	char	**m;		//map saved in array
// 	int		w;			//width of map
// 	int		h;			//height of map
// 	int		p;			//player status 1-alive, 0 dead
// 	int		px;			//player position on map the x axis
// 	int		py;			//player position on map the y axis
//						//add player direction?
// 	int		ct;			//total number of collectibles
//	int		cr;			//number of remaining collectibles
// 	int		*c;			//collectible status 
// 	int		*cx;		//collectible position on the x axis
// 	int		*cy;		//collectible position on the y axis
// 	int		et;			//total number of enemies
//						//add enemy remaining?
// 	int		*e;			//array of enemies 0 dead, 1 alive
// 	int		*ex;		//enemy position on map the x axis
// 	int		*ey;		//enemy position on map the y axis
//						//add enemy direction?
//	int		x;			// exit status 0 closed, 1 open
//	int		xx;			// exit X
//	int		xy;			// exit Y
//	int		steps;		// number of steps player did
// } map_t;

typedef struct s_map
{
	char	**m;
	char	*s;
	int		w;
	int		h;
	int		p;
	int		px;
	int		py;
	int		ct;
	int		cr;
	int		*c;
	int		*cx;
	int		*cy;
	int		et;
	int		*e;
	int		*ex;
	int		*ey;
	int		x;
	int		xx;
	int		xy;
	int		steps;
}	t_map;

typedef struct s_highscore
{
	char	*name;
	char	*coalition;
	int		index;
	int		score;
}	t_hs;

typedef struct s_control
{
	int		w;
	int		s;
	int		a;
	int		d;
	int		space;
	int		ctrl;
	int		time;
	int		t;
}	t_controls;

typedef struct s_imgs
{
	mlx_instance_t	*pi;
	mlx_instance_t	*ei;
	mlx_instance_t	*ci;
	mlx_instance_t	*c2i;
	mlx_instance_t	*dci;
	mlx_instance_t	*doi;
}	t_imgs;

typedef struct s_max
{
	mlx_t		*mlx;
	t_map		*map;
	t_controls	*key;
	t_imgs		*img;
	mlx_image_t	*str;
	mlx_image_t	*tmp;
	char		*player_name;
	char		*player_coalition;
	int			death;
	int			lives;
	int			score;
	int			exit;
	int			time;
}	t_max;

// PROTOTYPES



#endif
