/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rect.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/12 02:55:10 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/12 03:09:49 by llelievr         ###   ########.fr       */
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
			img->pixels[(j + bounds.y) * img->width + i + bounds.x] = color;
}