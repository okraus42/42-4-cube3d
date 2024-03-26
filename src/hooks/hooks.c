/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 10:45:29 by plouda            #+#    #+#             */
/*   Updated: 2024/03/05 11:00:48 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	keyhook(mlx_key_data_t keydata, void *param)
{
	t_master	*master;

	master = param;
	camera_movements(master, keydata);
	object_movements(master, keydata);
	light_movements(master, keydata);
	if (keydata.key == MLX_KEY_BACKSPACE && keydata.action != MLX_RELEASE)
		reset_to_default(master);
	else if (keydata.key == MLX_KEY_ESCAPE && keydata.action != MLX_RELEASE)
		mlx_close_window(master->mlx);
	ft_draw_string(master);
}

void	mousehook(mouse_key_t button,
	action_t action, modifier_key_t mods, void *param)
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
		if (xpos <= WIDTH && ypos <= HEIGHT)
		{
			rf = trace_singular_object_ray(master, xpos, ypos);
			reset_to_default(master);
			set_highlight_from_reference(master, rf);
			ft_draw_string(master);
		}
		else
		{
			reset_to_default(master);
			ft_draw_string(master);
		}
	}
}
