/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   circle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/06 13:06:49 by llelievr          #+#    #+#             */
/*   Updated: 2019/06/06 13:25:17 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

static inline	put_pixel(t_img *img, t_pixel p)
{
	img->pixels[p.y * img->width + p.x] = p.color;
}

static void		fill(t_img *img, t_pixel center, t_pixel p) 
{ 
	put_pixel(img, (t_pixel){ center.x + p.x, center.y + p.y, center.color }); 
	put_pixel(img, (t_pixel){ center.x - p.x, center.y + p.y, center.color }); 
	put_pixel(img, (t_pixel){ center.x + p.x, center.y - p.y, center.color }); 
	put_pixel(img, (t_pixel){ center.x - p.x, center.y - p.y, center.color }); 
	put_pixel(img, (t_pixel){ center.x + p.y, center.y + p.x, center.color }); 
	put_pixel(img, (t_pixel){ center.x - p.y, center.y + p.x, center.color }); 
	put_pixel(img, (t_pixel){ center.x + p.y, center.y - p.x, center.color }); 
	put_pixel(img, (t_pixel){ center.x - p.y, center.y - p.x, center.color }); 
} 
  
void			draw_circle(t_img *img, t_pixel center, int r) 
{ 
	t_pixel p;
	int d; 
	
	d = 3 - 2 * r;
	p = (t_pixel) { 0, r, 0 };
	fill(img, center, p);
	while (p.y >= p.x) 
	{ 
		p.x++; 
		if (d > 0) 
		{ 
			p.y--;  
			d = d + 4 * (p.x - p.y) + 10; 
		} 
		else
			d = d + 4 * p.x + 6; 
		fill(img, center, p); 
	} 
} 
