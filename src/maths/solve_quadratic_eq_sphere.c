/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solve_quadratic_eq_sphere.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 14:29:06 by plouda            #+#    #+#             */
/*   Updated: 2024/03/05 14:29:45 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

int	two_roots_sphere(double *t, t_quadterms quad)
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
	*t = res[0];
	return (1);
}

int	solve_quad_sphere(double *t, t_quadterms quad)
{
	double	res[2];

	quad.discr = pow(quad.b, 2) - 4 * quad.a * quad.c;
	if (quad.discr < -PRECISION)
		return (0);
	else if (quad.discr > -PRECISION && quad.discr < PRECISION)
	{
		res[0] = -0.5 * quad.b / quad.a;
		res[1] = res[0];
		*t = res[0];
		return (1);
	}
	else
		return (two_roots_sphere(t, quad));
}
