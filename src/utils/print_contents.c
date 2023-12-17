/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_contents.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 12:02:07 by plouda            #+#    #+#             */
/*   Updated: 2023/12/17 17:56:08 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

int	print_contents(t_rt *rt)
{
	int	i;

	i = 0;
	printf("=================================================\n");
	printf("AMBIENT\n");
	printf("%-11s %.2f\n%-11s %i,%i,%i\n", "Ratio:", rt->ambient->ratio, \
		"RGB:", rt->ambient->rgb[0], rt->ambient->rgb[1], rt->ambient->rgb[2]);
	printf("=================================================\n");
	printf("CAMERA\n");
	printf("%-11s %.2f,%.2f,%.2f\n%-11s %.2f,%.2f,%.2f\n%-11s %i\n", \
			"Coords:", rt->camera->coords[0], rt->camera->coords[1], rt->camera->coords[2], \
			"Vector:", rt->camera->nvect[0], rt->camera->nvect[1], rt->camera->nvect[2], \
			"FoV:", rt->camera->fov);
	printf("=================================================\n");
	printf("LIGHT\n");
	printf("%-11s %.2f,%.2f,%.2f\n%-11s %.2f\n%-11s %i,%i,%i\n", \
			"Coords:", rt->light->coords[0], rt->light->coords[1], rt->light->coords[2], \
			"Brightness:", rt->light->brightness,
			"RGB:", rt->light->rgb[0], rt->light->rgb[1], rt->light->rgb[2]);
	printf("=================================================\n");
	while (rt->n_spheres > i)
	{
		printf("SPHERE %i\n", i + 1);
		printf("%-11s %.2f,%.2f,%.2f\n%-11s %.2f\n%-11s %i,%i,%i\n", \
				"Coords:", rt->spheres[i]->coords[0], rt->spheres[i]->coords[1], rt->spheres[i]->coords[2], \
				"Diameter:", rt->spheres[i]->diameter, \
				"RGB:", rt->spheres[i]->rgb[0], rt->spheres[i]->rgb[1], rt->spheres[i]->rgb[2]);
		printf("=================================================\n");
		i++;
	}
	i = 0;
	while (rt->n_planes > i)
	{
		printf("PLANE %i\n", i + 1);
		printf("%-11s %.2f,%.2f,%.2f\n%-11s %.2f,%.2f,%.2f\n%-11s %i,%i,%i\n", \
				"Coords:", rt->planes[i]->coords[0], rt->planes[i]->coords[1], rt->planes[i]->coords[2], \
				"Vector:", rt->planes[i]->nvect[0], rt->planes[i]->nvect[1], rt->planes[i]->nvect[2], \
				"RGB:", rt->planes[i]->rgb[0], rt->planes[i]->rgb[1], rt->planes[i]->rgb[2]);
		printf("=================================================\n");
		i++;
	}
	i = 0;
	while (rt->n_cylinders > i)
	{
		printf("CYLINDER %i\n", i + 1);
		printf("%-11s %.2f,%.2f,%.2f\n%-11s %.2f,%.2f,%.2f\n%-11s %.2f\n%-11s %.2f\n%-11s %i,%i,%i\n", \
				"Coords:", rt->cylinders[i]->coords[0], rt->cylinders[i]->coords[1], rt->cylinders[i]->coords[2], \
				"Vector:", rt->cylinders[i]->nvect[0], rt->cylinders[i]->nvect[1], rt->cylinders[i]->nvect[2], \
				"Diameter:", rt->cylinders[i]->diameter, \
				"Height:", rt->cylinders[i]->height, \
				"RGB:", rt->cylinders[i]->rgb[0], rt->cylinders[i]->rgb[1], rt->cylinders[i]->rgb[2]);
		printf("=================================================\n");
		i++;
	}
	i = 0;
	while (rt->n_cylinders > i)
	{
		printf("DISC BOTCAP %i\n", i + 1);
		printf("%-11s %.2f,%.2f,%.2f\n%-11s %.2f,%.2f,%.2f\n%-11s %.2f\n%-11s %i,%i,%i\n", \
				"Coords:", rt->cylinders[i]->botcap->coords[0], rt->cylinders[i]->botcap->coords[1], rt->cylinders[i]->botcap->coords[2], \
				"Vector:", rt->cylinders[i]->botcap->nvect[0], rt->cylinders[i]->botcap->nvect[1], rt->cylinders[i]->botcap->nvect[2], \
				"Radius:", rt->cylinders[i]->botcap->radius, \
				"RGB:", rt->cylinders[i]->rgb[0], rt->cylinders[i]->rgb[1], rt->cylinders[i]->rgb[2]);
		printf("=================================================\n");
		printf("DISC TOPCAP %i\n", i + 1);
		printf("%-11s %.2f,%.2f,%.2f\n%-11s %.2f,%.2f,%.2f\n%-11s %.2f\n%-11s %i,%i,%i\n", \
				"Coords:", rt->cylinders[i]->topcap->coords[0], rt->cylinders[i]->topcap->coords[1], rt->cylinders[i]->topcap->coords[2], \
				"Vector:", rt->cylinders[i]->topcap->nvect[0], rt->cylinders[i]->topcap->nvect[1], rt->cylinders[i]->topcap->nvect[2], \
				"Radius:", rt->cylinders[i]->topcap->radius, \
				"RGB:", rt->cylinders[i]->rgb[0], rt->cylinders[i]->rgb[1], rt->cylinders[i]->rgb[2]);
		printf("=================================================\n");
		i++;
	}
	return (0);
}
