/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_color_gradient.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/22 03:23:55 by llelievr          #+#    #+#             */
/*   Updated: 2019/03/03 18:40:48 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_color				ft_color_gradient(t_color a, t_color b, float percent)
{
	return (ft_color((int)(a.r + (b.r - a.r) * percent),
		(int)(a.g + (b.g - a.g) * percent),
		(int)(a.b + (b.b - a.b) * percent)));
}
