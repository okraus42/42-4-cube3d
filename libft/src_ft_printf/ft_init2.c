/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 17:48:39 by okraus            #+#    #+#             */
/*   Updated: 2024/02/26 17:27:34 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/libft.h"

void	ft_init_precision(int *i, int *err, t_pf_info *data, t_pf_vargs *varg)
{
	int			n;
	long long	num;

	if (data->orig[*i] == '*')
	{
		data->precision = va_arg(varg->arg, unsigned int);
		++(*i);
	}
	else
	{
		n = 0;
		num = ft_latoi(&data->orig[*i]);
		while (ft_isdigit(data->orig[*i]))
		{
			++n;
			++(*i);
		}
		if (n > 15 || num > 0xFFFFFFFFLL)
			*err = 1;
		else if (num > 0)
			data->precision = (unsigned int)num;
		else
			data->precision = 0;
	}
}

void	ft_init_modifiers(int *i, t_pf_info *data)
{
	if (data->orig[*i] == 'h' && data->orig[(*i) + 1] == 'h')
	{
		data->type_flag |= LOWERCASE_HH;
		++(*i);
	}
	else if (data->orig[*i] == 'l' && data->orig[(*i) + 1] == 'l')
	{
		data->type_flag |= LOWERCASE_LL;
		++(*i);
	}
	else if (data->orig[*i] == 'h')
		data->type_flag |= LOWERCASE_H;
	else if (data->orig[*i] == 'l')
		data->type_flag |= LOWERCASE_L;
	else if (data->orig[*i] == 'L')
		data->type_flag |= UPPERCASE_L;
	else if (data->orig[*i] == 'z')
		data->type_flag |= LOWERCASE_Z;
	else if (data->orig[*i] == 'j')
		data->type_flag |= LOWERCASE_J;
	else if (data->orig[*i] == 't')
		data->type_flag |= LOWERCASE_T;
	++(*i);
}

static void	ft_get_int_value_helper(t_pf_info *data, t_pf_vargs *varg, int flag)
{
	data->value_flag = flag;
	if (flag & SIGNED_CHAR)
		data->value.ll = (long long)va_arg(varg->arg, int);
	else if (flag & SIGNED_SHORT)
		data->value.ll = (long long)va_arg(varg->arg, int);
	else if (flag & LONG)
		data->value.ll = (long long)va_arg(varg->arg, long);
	else if (flag & LONG_LONG)
		data->value.ll = (long long)va_arg(varg->arg, long long);
	else if (flag & INT)
		data->value.ll = (long long)va_arg(varg->arg, int);
}

int	ft_get_int_value(t_pf_info *data, t_pf_vargs *varg)
{
	if (data->type_flag & LENGTH_MODIFIER)
	{
		if (data->type_flag & LOWERCASE_HH)
			ft_get_int_value_helper(data, varg, SIGNED_CHAR);
		else if (data->type_flag & LOWERCASE_H)
			ft_get_int_value_helper(data, varg, SIGNED_SHORT);
		else if (data->type_flag & LOWERCASE_L)
			ft_get_int_value_helper(data, varg, LONG);
		else if (data->type_flag & LLL)
			ft_get_int_value_helper(data, varg, LONG_LONG);
		else
			return (1);
	}
	else
		ft_get_int_value_helper(data, varg, INT);
	return (0);
}

int	ft_init_int(char c, t_pf_info *data, t_pf_vargs *varg)
{
	if (c == 'c')
		data->type_flag |= LOWERCASE_C;
	else if (c == 'd')
		data->type_flag |= LOWERCASE_D;
	else if (c == 'i')
		data->type_flag |= LOWERCASE_I;
	return (ft_get_int_value(data, varg));
}
