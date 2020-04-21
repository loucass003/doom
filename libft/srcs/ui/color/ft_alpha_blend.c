/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_alpha_blend.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/03 18:42:17 by llelievr          #+#    #+#             */
/*   Updated: 2019/03/03 18:50:36 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_color	ft_alpha_blend(t_color c1, t_color c2)
{
	float	a1;
	float	a2;
	float	a;

	a1 = c1.a / 255.0;
	a2 = c2.a / 255.0;
	a = a2 * (1 - a1);
	return ((t_color) {
		.r = c1.r * a1 + c2.r * a,
		.g = c1.g * a1 + c2.g * a,
		.b = c1.b * a1 + c2.b * a,
		.a = (a1 + a) * 0xFF
	});
}
