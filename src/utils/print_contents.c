/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_contents.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 12:02:07 by plouda            #+#    #+#             */
/*   Updated: 2024/03/02 16:14:04 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

int	print_contents(t_rt *rt)
{
	int	i;

	i = 0;
	printf("=================================================\n");
	printf("AMBIENT\n");
	printf("%-11s %.2f\n%-11s %i,%i,%i\n", "Ratio:", rt->ambient->ratio,
		"RGB:", rt->ambient->rgb[0], rt->ambient->rgb[1], rt->ambient->rgb[2]);
	printf("=================================================\n");
	printf("CAMERA\n");
	printf("%-11s %.2f,%.2f,%.2f\n%-11s %.2f,%.2f,%.2f\n%-11s %i\n",
		"Coords:", rt->camera->coords[0], rt->camera->coords[1],
		rt->camera->coords[2],
		"Vector:", rt->camera->nvect[0], rt->camera->nvect[1],
		rt->camera->nvect[2],
		"FoV:", rt->camera->fov);
	printf("=================================================\n");
	while (rt->n_spheres > i)
	{
		printf("SPHERE %i\n", i + 1);
		printf("%-11s %.2f,%.2f,%.2f\n%-11s %.2f\n%-11s %i,%i,%i\n%-11s %p\n",
			"Coords:", rt->spheres[i]->coords[0],
			rt->spheres[i]->coords[1], rt->spheres[i]->coords[2],
			"Diameter:", rt->spheres[i]->radius * 2,
			"RGB:", rt->spheres[i]->rgb[0], rt->spheres[i]->rgb[1],
			rt->spheres[i]->rgb[2],
			"CHECKERBOARD:", rt->spheres[i]->checkerboard);
		printf("=================================================\n");
		i++;
	}
	i = 0;
	while (rt->n_planes > i)
	{
		printf("PLANE %i\n", i + 1);
		printf("%-11s %.2f,%.2f,%.2f\n%-11s %.2f,%.2f,%.2f\n%-11s %i,%i,%i\n",
			"Coords:", rt->planes[i]->coords[0],
			rt->planes[i]->coords[1], rt->planes[i]->coords[2],
			"Vector:", rt->planes[i]->nvect[0],
			rt->planes[i]->nvect[1], rt->planes[i]->nvect[2],
			"RGB:", rt->planes[i]->rgb[0],
			rt->planes[i]->rgb[1], rt->planes[i]->rgb[2]);
		printf("%-11s %p\n", "CHECKERBOARD:", rt->planes[i]->checkerboard);
		printf("=================================================\n");
		i++;
	}
	i = 0;
	while (rt->n_cylinders > i)
	{
		printf("CYLINDER %i\n", i + 1);
		printf("%-11s %.2f,%.2f,%.2f\n%-11s %.2f,%.2f,%.2f\n%-11s %.2f\n",
			"Coords:", rt->cylinders[i]->coords[0],
			rt->cylinders[i]->coords[1], rt->cylinders[i]->coords[2],
			"Vector:", rt->cylinders[i]->nvect[0],
			rt->cylinders[i]->nvect[1], rt->cylinders[i]->nvect[2],
			"Diameter:", rt->cylinders[i]->radius * 2);
		printf("%-11s %.2f\n%-11s %i,%i,%i\n%-11s %p\n",
			"Height:", rt->cylinders[i]->height,
			"RGB:", rt->cylinders[i]->rgb[0],
			rt->cylinders[i]->rgb[1], rt->cylinders[i]->rgb[2],
			"CHECKERBOARD:", rt->cylinders[i]->checkerboard);
		printf("=================================================\n");
		i++;
	}
	i = 0;
	while (rt->n_cylinders > i)
	{
		printf("DISC BOTCAP %i\n", i + 1);
		printf("%-11s %.2f,%.2f,%.2f\n%-11s %.2f,%.2f,%.2f\n%-11s %.2f\n",
			"Coords:", rt->cylinders[i]->botcap->coords[0],
			rt->cylinders[i]->botcap->coords[1],
			rt->cylinders[i]->botcap->coords[2],
			"Vector:", rt->cylinders[i]->botcap->nvect[0],
			rt->cylinders[i]->botcap->nvect[1],
			rt->cylinders[i]->botcap->nvect[2],
			"Radius:", rt->cylinders[i]->botcap->radius);
		printf("%-11s %i,%i,%i\n%-11s %p\n",
			"RGB:", rt->cylinders[i]->rgb[0],
			rt->cylinders[i]->rgb[1], rt->cylinders[i]->rgb[2],
			"CHECKERBOARD:", rt->cylinders[i]->checkerboard);
		printf("=================================================\n");
		printf("DISC TOPCAP %i\n", i + 1);
		printf("%-11s %.2f,%.2f,%.2f\n%-11s %.2f,%.2f,%.2f\n%-11s %.2f\n",
			"Coords:", rt->cylinders[i]->topcap->coords[0],
			rt->cylinders[i]->topcap->coords[1],
			rt->cylinders[i]->topcap->coords[2],
			"Vector:", rt->cylinders[i]->topcap->nvect[0],
			rt->cylinders[i]->topcap->nvect[1],
			rt->cylinders[i]->topcap->nvect[2],
			"Radius:", rt->cylinders[i]->topcap->radius);
		printf("%-11s %i,%i,%i\n%-11s %p\n",
			"RGB:", rt->cylinders[i]->rgb[0],
			rt->cylinders[i]->rgb[1], rt->cylinders[i]->rgb[2],
			"CHECKERBOARD:", rt->cylinders[i]->checkerboard);
		printf("=================================================\n");
		i++;
	}
	i = 0;
	while (rt->n_cones > i)
	{
		printf("CONE %i\n", i + 1);
		printf("%-11s %.2f,%.2f,%.2f\n%-11s %.2f,%.2f,%.2f\n%-11s %.2f\n",
			"Coords:", rt->cones[i]->coords[0],
			rt->cones[i]->coords[1], rt->cones[i]->coords[2],
			"Vector:", rt->cones[i]->nvect[0],
			rt->cones[i]->nvect[1], rt->cones[i]->nvect[2],
			"Diameter:", rt->cones[i]->radius * 2);
		printf("%-11s %.2f\n%-11s %i,%i,%i\n%-11s %p\n",
			"Height:", rt->cones[i]->height,
			"RGB:", rt->cones[i]->rgb[0],
			rt->cones[i]->rgb[1], rt->cones[i]->rgb[2],
			"CHECKERBOARD:", rt->cones[i]->checkerboard);
		printf("=================================================\n");
		i++;
	}
	i = 0;
	while (rt->n_checkerboards > i)
	{
		printf("CHECKERBOARD %i\n", i + 1);
		printf("%-11s %i\n%-11s %i,%i,%i\n%-11s %i,%i,%i\n%-11s %.2f\n",
			"ID:", rt->checkerboards[i]->id,
			"RGB1:", rt->checkerboards[i]->rgb1[0],
			rt->checkerboards[i]->rgb1[1], rt->checkerboards[i]->rgb1[2],
			"RGB2:", rt->checkerboards[i]->rgb2[0],
			rt->checkerboards[i]->rgb2[1], rt->checkerboards[i]->rgb2[2],
			"Magnitude:", rt->checkerboards[i]->magnitude);
		printf("=================================================\n");
		i++;
	}
	return (0);
}
