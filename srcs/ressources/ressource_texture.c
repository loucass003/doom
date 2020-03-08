/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ressource_texture.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/12 17:27:40 by llelievr          #+#    #+#             */
/*   Updated: 2020/03/08 18:58:34 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL_image.h>
#include <libft.h>
#include "doom.h"
#include "ressource.h"
#include "write_structs.h"
#include "tga.h"

static t_bool	is_tga_ext(char *path)
{
	char	*start;

	start = ft_strrchr(path, '.');
	return (start && *start && ft_memcmp(start + 1, "tga", 3) == 0);
}

t_bool			free_load_texture_surface(SDL_Surface *surface)
{
	if (surface)
		SDL_FreeSurface(surface);
	return (TRUE);
}

t_bool			load_texture_file(t_doom *doom, t_ressource *r, char *path)
{
	SDL_Surface	*s_src;
	SDL_Surface	*s;
	t_img		*img;

	if (is_tga_ext(path) && !load_tga(path, &s_src))
		return (FALSE);
	else if (!(s_src = IMG_Load(path)))
		return (FALSE);
	if (!(s = SDL_ConvertSurfaceFormat(s_src, SDL_PIXELFORMAT_ARGB8888, 0)))
	{
		SDL_FreeSurface(s_src);
		return (!free_load_texture_surface(s));
	}
	SDL_FreeSurface(s_src);
	if (!(img = malloc(sizeof(t_img))))
		return (!free_load_texture_surface(s));
	img->ignore_texture = TRUE;
	if (!create_image(doom->renderer, s->w, s->h, img))
		return (!free_load_texture_surface(s));
	ft_memcpy(img->pixels, s->pixels, s->w * s->h * sizeof(uint32_t));
	r->loaded = TRUE;
	r->data.texture = img;
	return (free_load_texture_surface(s));
}

t_bool			read_texture(t_ressource_manager *rm, t_img **img)
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

t_bool			write_texture(t_ressource_manager *rm, t_img *img)
{
	t_wr_img	wr_img;

	wr_img = (t_wr_img){
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
