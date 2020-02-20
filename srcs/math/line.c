/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/08 14:40:55 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/20 16:58:46 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <math.h>
#include "maths/line.h"

t_vec2		line_normal(t_line l)
{
	return ((t_vec2){
		l.b.y - l.a.y,
		l.a.x - l.b.x
	});
}

float		line_get_distance(t_line l, t_vec2 p)
{
	const	t_vec2		len = ft_vec2_sub(l.b, l.a);

	return (ft_abs(len.y * p.x - len.x * p.y + l.b.x * l.a.y - l.b.y * l.a.x)
		/ sqrtf(len.y * len.y + len.x * len.x));
}
