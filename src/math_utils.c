/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 12:02:47 by plouda            #+#    #+#             */
/*   Updated: 2023/12/14 18:00:33 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minirt.h"

double	deg(double rad)
{
	double	deg;

	deg = rad * 180 / M_PI;
	return (deg);
}

double	rad(double deg)
{
	double	rad;

	rad = deg * M_PI / 180;
	return (rad);
}

double	absf(double n)
{
	if (n < 0)
		return (-n);
	else
		return (n);
}

int	two_roots(double *t, t_quadterms quad)
{
	double	q;
	double	res[2];

	if (quad.b > 0)
		q = -0.5 * (quad.b + sqrt(quad.discr));
	else
		q = -0.5 * (quad.b - sqrt(quad.discr));
	res[0] = q / quad.a;
	res[1] = quad.c / q;
	if (res[0] > res[1])
		ft_swapf(&res[0], &res[1]);
	if (res[0] < 0)
	{
		res[0] = res[1];
		if (res[0] < 0)
			return (0);
	}
	*t = res[0];
	return (1);
}

int	solve_quad(double *t, t_quadterms quad)
{
	double	res[2];

	quad.discr = pow(quad.b, 2) - 4 * quad.a * quad.c;
	if (quad.discr < 0)
		return (0);
	else if (quad.discr == 0)
	{
		res[0] = -0.5 * quad.b / quad.a;
		res[1] = res[0];
		*t = res[0];
		return (1);
	}
	else
		return (two_roots(t, quad));
}

int	check_caps(double *t, t_ray ray, t_cylinder *cylinder, double *res)
{
	if (is_between_caps(cylinder->botcap, cylinder->topcap, ray, res[0]))
	{
		*t = res[0];
		return (1);
	}
	if (is_between_caps(cylinder->botcap, cylinder->topcap, ray, res[1]))
	{
		*t = res[1];
		return (1);
	}
	return (0);
}

int	two_roots_cyl(double *t, t_quadterms quad, t_ray ray, t_cylinder *cylinder)
{
	double	q;
	double	res[2];

	if (quad.b > 0)
		q = -0.5 * (quad.b + sqrt(quad.discr));
	else
		q = -0.5 * (quad.b - sqrt(quad.discr));
	res[0] = q / quad.a;
	res[1] = quad.c / q;
	if (res[0] > res[1])
		ft_swapf(&res[0], &res[1]);
	if (res[0] < 0)
	{
		res[0] = res[1];
		if (res[0] < 0)
			return (0);
	}
	return (check_caps(t, ray, cylinder, res));
}

int	solve_quad_cyl(double *t, t_quadterms quad, t_ray ray, t_cylinder *cylinder)
{
	double	res[2];

	quad.discr = pow(quad.b, 2) - 4 * quad.a * quad.c;
	if (quad.discr < 0)
		return (0);
	else if (quad.discr == 0)
	{
		res[0] = -0.5 * quad.b / quad.a;
		res[1] = res[0];
		if (is_between_caps(cylinder->botcap, cylinder->topcap, ray, res[0]))
		{
			*t = res[0];
			return (1);
		}
		else
			return (0);
	}
	else
		return (two_roots_cyl(t, quad, ray, cylinder));
}
