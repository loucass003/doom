/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_ressource.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/24 01:51:49 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/24 02:22:59 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL_image.h>
#include <fcntl.h>
#include <libft.h>
#include "doom.h"
#include "ressource.h"

t_bool		load_texture(t_doom *doom, t_ressource *r, char *path)
{
	SDL_Surface	*s;
	t_img		*img;

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

t_bool		load_ressource(t_doom *doom, t_ressource *r, char *path)
{

	if (r->type == RESSOURCE_TEXTURE)
		return (load_texture(doom, r, path));
	else
		return (FALSE);
}