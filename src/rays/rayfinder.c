/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rayfinder.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 15:23:12 by plouda            #+#    #+#             */
/*   Updated: 2024/03/05 15:23:37 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	reset_rayfinder(t_rayfinder *rf)
{
	rf->t = (double)INT_MAX;
	rf->t_near = (double)INT_MAX;
	rf->object_flag = EMPTY;
	rf->object_ptr = NULL;
	rf->clr = 0x000000FF;
}

t_rayfinder	init_rayfinder(t_master	*master)
{
	t_rayfinder	rf;

	rf.ratio = (double)master->w_width / (double)master->w_height;
	rf.fov = master->rt->camera->fov;
	rf.scale = tan(rad(rf.fov * 0.5));
	rf.t_near = (double)INT_MAX;
	rf.t = (double)INT_MAX;
	rf.object_flag = EMPTY;
	rf.object_ptr = NULL;
	rf.clr = 0x000000FF;
	rf.origin = shift_origin(master->rt->camera->matrix);
	rf.cam_mat = master->rt->camera->matrix;
	rf.shadowray.direction = (t_vect3f){};
	rf.shadowray.origin = (t_vect3f){};
	rf.shadow_inter = (t_vect3f){};
	rf.inter_dist = 0;
	rf.w_height = master->w_height;
	rf.w_width = master->w_width;
	return (rf);
}
