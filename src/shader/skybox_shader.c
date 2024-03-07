/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skybox_shader.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 13:14:34 by plouda            #+#    #+#             */
/*   Updated: 2024/03/07 15:11:09 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	skybox_shader(t_rayfinder *rf, t_vect3f intersection,
	void *object_ptr, t_master *master)
{
	t_sphere	*sphere;
	t_shader	shader;

	(void)master;
	sphere = (t_sphere *)object_ptr;
	set_sphere_rgb(&shader, sphere, intersection);
	rf->clr = get_clr_int(shader.rgb_object);
}
