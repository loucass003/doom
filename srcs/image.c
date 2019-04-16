/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 21:24:49 by llelievr          #+#    #+#             */
/*   Updated: 2019/04/16 22:49:41 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

t_bool	create_image(SDL_Renderer *renderer, uint32_t w, uint32_t h, t_img *img)
{
	img->size = w * h;
	img->width = w;
	img->height = h;
	img->renderer = renderer;
	if (img->ignore_texture == FALSE && !(img->texture = SDL_CreateTexture(renderer,
		SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, w, h)))
		return (FALSE);
	if (!(img->pixels = (uint32_t *)malloc(img->size * 4)))
		return (FALSE);
	return (TRUE);
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
	ft_bzero(img->pixels, img->size * 4);
}

void	update_image(t_img *img)
{
	if (!img->ignore_texture && img->texture)
		SDL_UpdateTexture(img->texture, NULL, img->pixels, img->width * 4);
}

void	apply_image(t_img *img, SDL_Rect *dst)
{
	if (!img->ignore_texture && img->texture)
		SDL_RenderCopy(img->renderer, img->texture, NULL, dst);
}
