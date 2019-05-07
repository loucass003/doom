/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cliping.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/07 17:03:32 by llelievr          #+#    #+#             */
/*   Updated: 2019/05/07 18:41:57 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

static int	intersect_x(t_line a, t_line b)
{
	const int	n = (a.a.x * a.b.y - a.a.y * a.b.x) * (b.a.x - b.b.x)
					- (a.a.x - a.b.x) * (b.a.x * b.b.y - b.a.y * b.b.x);
	const int	d = (a.a.x - a.b.x) * (b.a.y - b.b.y)
					- (a.a.y - a.b.y) * (b.a.x - b.b.x);
	
	return (n / d);
}

static int	intersect_y(t_line a, t_line b)
{
	const int	n = (a.a.x * a.b.y - a.a.y * a.b.x) * (b.a.y - b.b.y)
					- (a.a.y - a.b.y) * (b.a.x * b.b.y - b.a.y * b.b.x);
	const int	d = (a.a.x - a.b.x) * (b.a.y - b.b.y)
					- (a.a.y - a.b.y) * (b.a.x - b.b.x);
	
	return (n / d);
}

void		clip(t_polygon *poly, SDL_Rect area)
{
	int		i;
}
