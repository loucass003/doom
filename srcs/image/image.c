/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 21:24:49 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/16 22:58:46 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <SDL_image.h>
#include <SDL.h>
#include "doom.h"
#include "image.h"

t_bool	create_image(SDL_Renderer *renderer, uint32_t w, uint32_t h, t_img *img)
{
	
	img->size = w * h;
	img->width = w;
	img->height = h;
	img->renderer = renderer;
	img->secure = FALSE;
	if (img->ignore_texture == FALSE && !(img->texture = SDL_CreateTexture(renderer,
		SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, w, h)))
		return (FALSE);
	if (img->ignore_texture == FALSE && img->texture)
		SDL_SetTextureBlendMode(img->texture, SDL_BLENDMODE_ADD);
	if (!(img->pixels = (uint32_t *)malloc(img->size * sizeof(uint32_t))))
		return (FALSE);
	return (TRUE);
}

t_img	*surface_to_image(struct s_doom *doom, SDL_Surface *s)
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

t_bool	destroy_image(t_img *img)
{
	free(img->pixels);
	if (!img->ignore_texture && img->texture)
		SDL_DestroyTexture(img->texture);
	img = NULL;
	return (TRUE);
}

void	clear_image(t_img *img)
{
	ft_bzero(img->pixels, img->size * sizeof(uint32_t));
}

void	fill_color(t_img *img, uint32_t color)
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

void	update_image(t_img *img)
{
	if (!img->ignore_texture && img->texture)
		SDL_UpdateTexture(img->texture, NULL, img->pixels, img->width * sizeof(uint32_t));
}

void	apply_image(t_img *img, SDL_Rect *dst)
{
	if (!img->ignore_texture && img->texture)
		SDL_RenderCopy(img->renderer, img->texture, NULL, dst);
}
