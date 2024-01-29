/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objlist.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 10:31:39 by plouda            #+#    #+#             */
/*   Updated: 2024/01/29 11:30:38 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minirt.h"

t_objlist	*ft_objlst_new(void *content, t_object flag)
{
	t_objlist	*lst;

	lst = malloc(sizeof(t_objlist));
	if (lst == NULL)
		return (NULL);
	lst->content = content;
	lst->next = NULL;
	lst->object = flag;
	return (lst);
}

void	ft_objlst_add_back(t_objlist **lst, t_objlist *new)
{
	t_objlist	*last;

	if (lst)
	{
		if (*lst == NULL)
			*lst = new;
		else
		{
			last = *lst;
			while (last && last->next)
				last = last->next;
			last->next = new;
		}
	}
}

void	ft_objlst_clear(t_objlist **lst, void (*del)(void *))
{
	t_objlist	*tmp;

	if (lst)
	{
		while (*lst)
		{
			tmp = (*lst)->next;
			ft_objlst_delone(*lst, del);
			(*lst) = tmp;
		}
	}
	lst = NULL;
}

void	ft_objlst_delone(t_objlist *lst, void (*del)(void *))
{
	if (lst && del)
	{
		(*del)(lst->content);
		free(lst);
	}
}

void	ft_objlst_iter(t_objlist *lst, void (*f)(void *, t_object))
{
	if (lst)
	{
		while (lst)
		{
			f(lst->content, lst->object);
			lst = lst->next;
		}
	}
}
