/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light_sphere_shader.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 15:36:57 by plouda            #+#    #+#             */
/*   Updated: 2024/03/05 15:39:11 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	light_sphere_shader(t_rayfinder *rf, void *object_ptr)
{
	t_sphere	*sphere;
	int			rgb_light_arr[3];

	sphere = (t_sphere *)object_ptr;
	get_clr_components(rgb_light_arr, sphere->rgb, 1, sphere->brightness);
	rf->clr = get_clr_int(rgb_light_arr);
}
