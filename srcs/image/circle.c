/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   circle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/06 13:06:49 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/20 15:01:02 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "image.h"

static inline void	put_cpixel(t_img *img, t_pixel p)
{
	if (p.y * img->width + p.x < 0 || p.y * img->width + p.x > img->width * img->height)
		return ;
	img->pixels[p.y * img->width + p.x] = p.color;
}

static void		fill(t_img *img, t_pixel center, t_pixel p) 
{ 
	put_cpixel(img, (t_pixel){ center.x + p.x, center.y + p.y, center.color }); 
	put_cpixel(img, (t_pixel){ center.x - p.x, center.y + p.y, center.color }); 
	put_cpixel(img, (t_pixel){ center.x + p.x, center.y - p.y, center.color }); 
	put_cpixel(img, (t_pixel){ center.x - p.x, center.y - p.y, center.color }); 
	put_cpixel(img, (t_pixel){ center.x + p.y, center.y + p.x, center.color }); 
	put_cpixel(img, (t_pixel){ center.x - p.y, center.y + p.x, center.color }); 
	put_cpixel(img, (t_pixel){ center.x + p.y, center.y - p.x, center.color }); 
	put_cpixel(img, (t_pixel){ center.x - p.y, center.y - p.x, center.color }); 
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
