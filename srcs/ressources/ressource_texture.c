/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ressource_texture.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/12 17:27:40 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/22 07:30:20 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL_image.h>
#include <libft.h>
#include "doom.h"
#include "ressource.h"
#include "write_structs.h"

t_bool		load_texture_file(t_doom *doom, t_ressource *r, char *path)
{
	SDL_Surface	*s;
	t_img		*img;

	// if (!load_tga(path, &s))
	// 	return (FALSE);
	if (!(s = IMG_Load(path)))
		return (FALSE);
	if (!(s = SDL_ConvertSurfaceFormat(s, SDL_PIXELFORMAT_ARGB8888, 0)))
		return (FALSE);
	if (!(img = malloc(sizeof(t_img))))
		return (FALSE);
	img->ignore_texture = TRUE;
	if (!create_image(doom->renderer, s->w, s->h, img))
		return (FALSE);
	ft_memcpy(img->pixels, s->pixels, s->w * s->h * sizeof(uint32_t));
	r->loaded = TRUE;
	r->data.texture = img;
	return (TRUE);
}

t_bool		read_texture(t_ressource_manager *rm, t_img **img)
{
	t_wr_img wr_img;

	if (!io_memcpy(&rm->reader, &wr_img, sizeof(t_wr_img)))
		return (FALSE);
	if (!(*img = malloc(sizeof(t_img))))
		return (FALSE);
	(*img)->ignore_texture = TRUE;
	if (!create_image(rm->doom->renderer, wr_img.width, wr_img.height, *img))
		return (FALSE);
	if ((*img)->size * sizeof(uint32_t) != wr_img.size)
		return (FALSE);
	if (!io_memcpy(&rm->reader, (*img)->pixels, wr_img.size))
		return (FALSE);
	return (TRUE);
}

t_bool		write_texture(t_ressource_manager *rm, t_img *img)
{
	const t_wr_img	wr_img = (t_wr_img){ 
		.size = img->size * sizeof(uint32_t),
		.width = img->width,
		.height = img->height
	};

	dp_write(rm, &wr_img, sizeof(t_wr_img));
	dp_write(rm, img->pixels, wr_img.size);
	return (TRUE);
}

t_ressource		*get_default_texture(t_ressource_manager *rm, t_bool use)
{
	if (use)
		rm->ressources->values[0]->used++;
	return (rm->ressources->values[0]);
}
