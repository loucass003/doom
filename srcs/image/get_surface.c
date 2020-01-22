/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_surface.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/22 15:00:14 by louali            #+#    #+#             */
/*   Updated: 2020/01/22 15:38:58 by louali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stdlib.h>
#include <SDL_image.h>
#include <SDL.h>
#include "doom.h"
#include "image.h"

inline Uint32			get_surface_pixel(SDL_Surface *surface, int x, int y)
{
	return (*(Uint32 *)(surface->pixels + y * surface->pitch + x * 4));
}

void					fill_color(t_img *img, uint32_t color)
{
	uint32_t		i;
	const uint64_t	c = ((uint64_t)color << 32 | (uint64_t)color);

	i = 0;
	while (i < img->size)
	{
		if (img->size - 2 >= 2)
		{
			((uint64_t *)img->pixels)[i / 2] = c;
			i += 2;
		}
		else
			img->pixels[i++] = color;
	}
}

t_img					*surface_to_image(struct s_doom *doom, SDL_Surface *s)
{
	t_img	*img;

	if (!(img = malloc(sizeof(t_img))))
		return (NULL);
	img->ignore_texture = TRUE;
	if (!create_image(doom ? doom->renderer : NULL, s->w, s->h, img))
		return (NULL);
	ft_memcpy(img->pixels, s->pixels, s->w * s->h * sizeof(uint32_t));
	return (img);
}
