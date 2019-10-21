/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rect.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/12 02:55:10 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/21 20:38:48 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "image.h"

void		fill_rect(t_img *img, SDL_Rect bounds, int color)
{
	int		i;
	int		j;

	j = -1;
	while (++j < bounds.h && !!(i = -1))
		while (++i < bounds.w)
			if (!put_pixel(img, bounds.x + i, bounds.y + j, color))
				return ;
}

void		draw_rect(t_img *img, SDL_Rect rect, int color)
{
	draw_line(img,
		(t_pixel) {rect.x, rect.y, color},
		(t_pixel) {rect.x + rect.w, rect.y, color});
	draw_line(img,
		(t_pixel) {rect.x + rect.w, rect.y, color},
		(t_pixel) {rect.x + rect.w, rect.y + rect.h, color});
	draw_line(img,
		(t_pixel) {rect.x + rect.w, rect.y + rect.h, color},
		(t_pixel) {rect.x, rect.y + rect.h, color});
	draw_line(img,
		(t_pixel) {rect.x, rect.y + rect.h, color},
		(t_pixel) {rect.x, rect.y, color});
}
