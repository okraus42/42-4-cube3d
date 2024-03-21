/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solve_quadratic_eq_cone.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 14:25:28 by plouda            #+#    #+#             */
/*   Updated: 2024/03/05 14:26:26 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

int	check_cone_caps(double *t, t_ray ray, t_cone *cone, double *res)
{
	if (is_between_caps(cone->base, cone->pinnacle, ray, res[0]))
	{
		*t = res[0];
		return (1);
	}
	if (is_between_caps(cone->base, cone->pinnacle, ray, res[1]))
	{
		*t = res[1];
		return (1);
	}
	return (0);
}

int	two_roots_cone(double *t, t_quadterms quad, t_ray ray, t_cone *cone)
{
	double	q;
	double	res[2];

	if (quad.b > PRECISION)
		q = -0.5 * (quad.b + sqrt(quad.discr));
	else
		q = -0.5 * (quad.b - sqrt(quad.discr));
	res[0] = q / quad.a;
	res[1] = quad.c / q;
	if (res[0] > res[1])
		ft_swapf(&res[0], &res[1]);
	if (res[0] < -PRECISION)
	{
		res[0] = res[1];
		if (res[0] < -PRECISION)
			return (0);
	}
	return (check_cone_caps(t, ray, cone, res));
}

int	solve_quad_cone(double *t, t_quadterms quad, t_ray ray, t_cone *cone)
{
	double	res[2];

	quad.discr = pow(quad.b, 2) - 4 * quad.a * quad.c;
	if (quad.discr < -PRECISION)
		return (0);
	else if (quad.discr > -PRECISION && quad.discr < PRECISION)
	{
		res[0] = -0.5 * quad.b / quad.a;
		res[1] = res[0];
		if (is_between_caps(cone->base, cone->pinnacle, ray, res[0]))
		{
			*t = res[0];
			return (1);
		}
		else
			return (0);
	}
	else
		return (two_roots_cone(t, quad, ray, cone));
}
