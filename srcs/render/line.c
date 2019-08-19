/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/02 00:28:50 by llelievr          #+#    #+#             */
/*   Updated: 2019/08/19 15:10:31 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"



void	draw_line(t_img *img, t_pixel p0, t_pixel p1)
{
	t_pixel			d;
	t_pixel			s;
	int				e[2];
	unsigned int	index;

	d = (t_pixel){ ft_abs(p1.x - p0.x), ft_abs(p1.y - p0.y), 0 };
	s = (t_pixel){ (p0.x < p1.x ? 1 : -1), (p0.y < p1.y ? 1 : -1), 0 };
	e[0] = (d.x > d.y ? d.x : -d.y) / 2;
	while (p0.x != p1.x || p0.y != p1.y)
	{
		if (p0.x >= 0 && p0.x < img->width && p0.y >= 0 && p0.y < img->height)
			img->pixels[index = p0.y * img->width + p0.x] = p0.color;
		e[1] = e[0];
		if (e[1] > -d.x)
		{
			e[0] -= d.y;
			p0.x += s.x;
		}
		if (e[1] < d.y)
		{
			e[0] += d.x;
			p0.y += s.y;
		}
	}
}
