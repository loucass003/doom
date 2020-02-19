/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tga_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/22 03:03:10 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/19 17:41:44 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <SDL.h>
#include "io.h"
#include "tga.h"

static t_bool		tga_colormap(t_reader *r, t_tga_format *tga)
{
	const ssize_t	cm_size = sizeof(uint8_t) * tga->header.cm_length * (tga->header.cm_size >> 3);

	if (!(tga->colormap = malloc(cm_size)))
		return (tga_return_error("Unable to alloc colormap"));
	if (!io_memcpy(r, tga->colormap, cm_size))
		return (tga_return_error("Unable to read colormap"));
	return (TRUE);
}

static t_bool		create_tga_image(t_reader *r, t_tga_format *tga,
	SDL_Surface **s)
{
	if (!(tga->data = malloc(sizeof(uint8_t) * tga->header.width
		* tga->header.height * tga->bpp)))
		return (tga_return_error("Unable to alloc buffer"));
	if (tga->header.color_map_type && !tga_colormap(r, tga))
		return (tga_return_error("Unable to process colormap"));
	if (!read_tga_data(r, tga))
		return (tga_return_error("Unable to read tga data"));
	if (!(*s = SDL_CreateRGBSurfaceWithFormat(0, tga->header.width,
		tga->header.height, tga->header.pixel_depth == 32 ? 32 : 24,
		tga->format)))
		return (tga_return_error("Unable to create Surface"));
	if (tga->header.image_descriptor.origin == 0)
		flip_image(tga, *s);
	else
		(*s)->pixels = tga->data;
	return (TRUE);
}

t_bool				free_tga(t_tga_format *tga)
{
	if (tga->colormap)
		ft_memdel((void **)&tga->colormap);
	if (tga->data)
		ft_memdel((void **)&tga->data);
	return (TRUE);
}

t_bool				load_tga(char *path, SDL_Surface **surface)
{
	t_reader		r;
	t_tga_format	tga;

	r = (t_reader) { .pos = 0 };
	if ((r.fd = open(path, O_RDONLY, 0666)) == -1)
		return (tga_return_error("Unable to open file"));
	ft_bzero(&tga, sizeof(t_tga_format));
	if (!io_memcpy(&r, &tga.header, sizeof(t_tga_header)))
		return (tga_return_error("Unable to read tga header"));
	if (!tga_move_to_start(&r, &tga))
		return (FALSE);
	tga.bpp = tga.header.pixel_depth == 32 ? 4 : 3;
	if (!create_tga_image(&r, &tga, surface))
	{
		free_tga(&tga);
		return (FALSE);
	}
	free_tga(&tga);
	close(r.fd);
	return (TRUE);
}
