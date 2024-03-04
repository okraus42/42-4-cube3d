/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 09:15:13 by plouda            #+#    #+#             */
/*   Updated: 2024/03/04 15:32:29 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

int	id_err(char *id, char *err_str, char *details)
{
	if (id == NULL)
		dprintf(2, "%sError: %s\n%s", RED, err_str, CRESET);
	else if (details == NULL)
		dprintf(2, "%sError: %s: %s\n%s", RED, id, err_str, CRESET);
	else
		dprintf(2, "%sError: %s: %s, expected %s\n%s", RED,
			id, err_str, details, CRESET);
	return (1);
}

int	id_warn(char *id, char *err_str, char *details, int comma)
{
	if (id == NULL)
		dprintf(2, "%sWarning: %s\n%s", YEL, err_str, CRESET);
	else if (details == NULL)
		dprintf(2, "%sWarning: %s: %s\n%s", YEL, id, err_str, CRESET);
	else if (comma)
		dprintf(2, "%sWarning: %s: %s, %s\n%s", YEL,
			id, err_str, details, CRESET);
	else
		dprintf(2, "%sWarning: %s: %s %s\n%s", YEL,
			id, err_str, details, CRESET);
	return (1);
}
