/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_object_vectors.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 12:17:42 by plouda            #+#    #+#             */
/*   Updated: 2024/03/05 14:00:01 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	set_sphere_vects(t_sphere *sphere)
{
	t_vect3f	tmp;
	t_vect33f	o;

	o.normal = sphere->normal;
	o.right = sphere->right;
	o.up = sphere->up;
	tmp = (t_vect3f){0, 1, 0};
	if (o.normal->x == 0. && o.normal->y == 1. && o.normal->z == 0.)
	{
		*o.right = (t_vect3f){1, 0, 0};
		*o.up = (t_vect3f){0, 0, -1};
	}
	else if (o.normal->x == 0. && o.normal->y == -1 && o.normal->z == 0.)
	{
		*o.right = (t_vect3f){1, 0, 0};
		*o.up = (t_vect3f){0, 0, 1};
	}
	else
	{
		*o.right = cross_product(tmp, *o.normal);
		normalize(o.right);
		*o.up = cross_product(*o.normal, *o.right);
		normalize(o.up);
	}
	sphere->q = get_obj_quat(*(sphere->normal), *(sphere->up));
}

void	set_plane_vects(t_plane *plane)
{
	t_vect3f	tmp;
	t_vect33f	o;

	o.normal = plane->normal;
	o.right = plane->right;
	o.up = plane->up;
	tmp = (t_vect3f){0, 1, 0};
	if (o.normal->x == 0. && o.normal->y == 1. && o.normal->z == 0.)
	{
		*o.right = (t_vect3f){1, 0, 0};
		*o.up = (t_vect3f){0, 0, -1};
	}
	else if (o.normal->x == 0. && o.normal->y == -1 && o.normal->z == 0.)
	{
		*o.right = (t_vect3f){1, 0, 0};
		*o.up = (t_vect3f){0, 0, 1};
	}
	else
	{
		*o.right = cross_product(tmp, *o.normal);
		normalize(o.right);
		*o.up = cross_product(*o.normal, *o.right);
		normalize(o.up);
	}
	plane->q = get_obj_quat(*(plane->normal), *(plane->up));
}

void	set_disc_vects(t_disc *disc)
{
	t_vect3f	tmp;
	t_vect33f	o;

	o.normal = disc->normal;
	o.right = disc->right;
	o.up = disc->up;
	tmp = (t_vect3f){0, 1, 0};
	if (o.normal->x == 0. && o.normal->y == 1. && o.normal->z == 0.)
	{
		*o.right = (t_vect3f){1, 0, 0};
		*o.up = (t_vect3f){0, 0, -1};
	}
	else if (o.normal->x == 0. && o.normal->y == -1 && o.normal->z == 0.)
	{
		*o.right = (t_vect3f){1, 0, 0};
		*o.up = (t_vect3f){0, 0, 1};
	}
	else
	{
		*o.right = cross_product(tmp, *o.normal);
		normalize(o.right);
		*o.up = cross_product(*o.normal, *o.right);
		normalize(o.up);
	}
}

void	set_cylinder_vects(t_cylinder *cylinder)
{
	t_vect3f	tmp;
	t_vect33f	o;

	set_cylinder_vects_ptrs(cylinder, &o);
	tmp = (t_vect3f){0, 1, 0};
	if (o.normal->x == 0. && o.normal->y == 1. && o.normal->z == 0.)
	{
		*o.right = (t_vect3f){1, 0, 0};
		*o.up = (t_vect3f){0, 0, -1};
	}
	else if (o.normal->x == 0. && o.normal->y == -1 && o.normal->z == 0.)
	{
		*o.right = (t_vect3f){1, 0, 0};
		*o.up = (t_vect3f){0, 0, 1};
	}
	else
	{
		*o.right = cross_product(tmp, *o.normal);
		normalize(o.right);
		*o.up = cross_product(*o.normal, *o.right);
		normalize(o.up);
	}
	cylinder->q = get_obj_quat(*(cylinder->normal), *(cylinder->up));
	set_cylinder_cap_inversion(cylinder);
}

void	set_cone_vects(t_cone *cone)
{
	t_vect3f	tmp;
	t_vect33f	o;

	set_cone_vects_ptrs(cone, &o);
	tmp = (t_vect3f){0, 1, 0};
	if (o.normal->x == 0. && o.normal->y == 1. && o.normal->z == 0.)
	{
		*o.right = (t_vect3f){1, 0, 0};
		*o.up = (t_vect3f){0, 0, -1};
	}
	else if (o.normal->x == 0. && o.normal->y == -1 && o.normal->z == 0.)
	{
		*o.right = (t_vect3f){1, 0, 0};
		*o.up = (t_vect3f){0, 0, 1};
	}
	else
	{
		*o.right = cross_product(tmp, *o.normal);
		normalize(o.right);
		*o.up = cross_product(*o.normal, *o.right);
		normalize(o.up);
	}
	cone->q = get_obj_quat(*(cone->normal), *(cone->up));
	set_cone_cap_inversion(cone);
}
