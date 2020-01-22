/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tga_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/22 03:03:10 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/22 08:10:17 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <SDL.h>
#include "io.h"
#include "tga.h"

t_bool		move_to_start(t_reader *r, t_tga_format *tga)
{
	int		i;

	i = -1;
	while (++i < tga->header.id_lenght)
	{
		if (io_peek(r, NULL))
			io_next(r);
		else
			return (tga_return_error("Invalid read"));
	}
	return (TRUE);
}

t_bool		tga_colormap(t_reader *r, t_tga_format *tga)
{
	const ssize_t	cm_size = sizeof(uint8_t) * tga->header.cm_length * (tga->header.cm_size >> 3);

	if (!(tga->colormap = malloc(cm_size)))
		return (tga_return_error("Unable to alloc colormap"));
	if (!io_memcpy(r, tga->colormap, cm_size))
		return (tga_return_error("Unable to read colormap"));
	return (TRUE);
}

t_bool		free_tga(t_tga_format *tga)
{
	if (tga->colormap)
		ft_memdel((void **)&tga->colormap);
	if (tga->data)
		ft_memdel((void **)&tga->data);
	return (TRUE);
}

t_bool		read_tga_8bits(t_reader *r, t_tga_format *tga)
{
	int		i;
	uint8_t	color;
	
	i = -1;
	tga->format = SDL_PIXELFORMAT_RGB24;
	while (++i < tga->header.width * tga->header.height)
	{
		if (!io_peek2(r, &color))
			return (FALSE);
		io_next(r);
		tga->data[(i * 3) + 2] = tga->colormap[(color * 3) + 0];
		tga->data[(i * 3) + 1] = tga->colormap[(color * 3) + 1];
		tga->data[(i * 3) + 0] = tga->colormap[(color * 3) + 2];
	}
	return (TRUE);
}

t_bool		read_tga_16bits(t_reader *r, t_tga_format *tga)
{
	int			i;
	uint8_t		color_a;
	uint8_t		color_b;
	uint16_t	color;
	
	i = -1;
	tga->format = SDL_PIXELFORMAT_RGB24;
	while (++i < tga->header.width * tga->header.height)
	{
		if (!io_peek2(r, &color_a))
			return (FALSE);
		io_next(r);
		if (!io_peek2(r, &color_b))
			return (FALSE);
		io_next(r);
		color = color_a + (color_b << 8);
		tga->data[(i * 3) + 0] = (uint8_t)(((color & 0x7C00) >> 10) << 3);
		tga->data[(i * 3) + 1] = (uint8_t)(((color & 0x03E0) >>  5) << 3);
		tga->data[(i * 3) + 2] = (uint8_t)(((color & 0x001F) >>  0) << 3);
	}
	return (TRUE);
}

t_bool		read_tga_24bits(t_reader *r, t_tga_format *tga)
{
	int		i;
	uint8_t	color;
	
	i = -1;
	tga->format = SDL_PIXELFORMAT_RGB24;
	while (++i < tga->header.width * tga->header.height)
	{
		if (!io_peek2(r, &color))
			return (FALSE);
		io_next(r);
		tga->data[(i * 3) + 2] = color;
		if (!io_peek2(r, &color))
			return (FALSE);
		io_next(r);
		tga->data[(i * 3) + 1] = color;
		if (!io_peek2(r, &color))
			return (FALSE);
		io_next(r);
		tga->data[(i * 3) + 0] = color;
	}
	return (TRUE);
}

t_bool		read_tga_32bits(t_reader *r, t_tga_format *tga)
{
	int		i;
	uint8_t	color;
	
	i = -1;
	tga->format = SDL_PIXELFORMAT_RGBA32;
	while (++i < tga->header.width * tga->header.height)
	{
		if (!io_peek2(r, &color))
			return (FALSE);
		io_next(r);
		tga->data[(i * 4) + 2] = color;
		if (!io_peek2(r, &color))
			return (FALSE);
		io_next(r);
		tga->data[(i * 4) + 1] = color;
		if (!io_peek2(r, &color))
			return (FALSE);
		io_next(r);
		tga->data[(i * 4) + 0] = color;
		if (!io_peek2(r, &color))
			return (FALSE);
		io_next(r);
		tga->data[(i * 4) + 3] = color;
	}
	return (TRUE);
}


t_bool		read_tga_data(t_reader *r, t_tga_format *tga)
{
	t_tga_image_type	type;

	type = (t_tga_image_type)tga->header.image_type;
	if (type == TGA_IMAGE_TYPE_NODATA)
		return (tga_return_error("Image type = 0 -> NODATA"));
	else if (type == TGA_IMAGE_TYPE_COLORS)
		return (read_tga_8bits(r, tga));
	else if (type == TGA_IMAGE_TYPE_TRUECOLORS)
	{
		if (tga->header.pixel_depth == 16)
			return (read_tga_16bits(r, tga));
		else if (tga->header.pixel_depth == 24)
			return (read_tga_24bits(r, tga));
		else if (tga->header.pixel_depth == 32)
			return (read_tga_32bits(r, tga));
		tga_return_error("Unknown pixel depth");
	}
	return (tga_return_error("Unknown TGA image type"));
} 

t_bool		load_tga(char *path, SDL_Surface **surface)
{
	t_reader		r;
	t_tga_format	tga;

	r = (t_reader) { .pos = 0 };
	if ((r.fd = open(path, O_RDONLY, 0666)) == -1)
		return (tga_return_error("Unable to open file"));
	ft_bzero(&tga, sizeof(t_tga_format));
	if (!io_memcpy(&r, &tga.header, sizeof(t_tga_header)))
		return (tga_return_error("Unable to read tga header"));
	if (!move_to_start(&r, &tga))
		return (FALSE);
	tga.bpp = tga.header.pixel_depth / 8;
	if (!(tga.data = malloc(sizeof(uint8_t) * tga.header.width * tga.header.height * tga.bpp)))
	{
		free_tga(&tga);
		return (tga_return_error("Unable to alloc buffer"));
	}
	if (tga.header.color_map_type && !tga_colormap(&r, &tga))
	{
		free_tga(&tga);
		return (tga_return_error("Unable to process colormap"));
	}
	if (!read_tga_data(&r, &tga))
	{
		free_tga(&tga);
		return (tga_return_error("Unable to read tga data"));
	}

	*surface = SDL_CreateRGBSurfaceWithFormat(
		0,
		tga.header.width,
		tga.header.height,
		tga.header.pixel_depth,
		tga.format
	);
	if (*surface)
	{
		if (tga.header.image_descriptor.origin == 0)
		{
			int y = -1;
			while (++y < (*surface)->h)
			{
				int x = -1;
				while (++x < (*surface)->w)
				{
					if (tga.format == SDL_PIXELFORMAT_ARGB32)
					{
						int index = ((*surface)->h - 1 - y) * (*surface)->w + x;
						int index2 = y * (*surface)->w + x;
						((uint32_t *)(*surface)->pixels)[index] = ((uint32_t *)tga.data)[index2];
					}
					else
					{
						int index = ((*surface)->h - 1 - y) * (*surface)->pitch + x * (*surface)->format->BytesPerPixel;
						int index2 = y * (*surface)->pitch + x * (*surface)->format->BytesPerPixel;
						((uint8_t *)(*surface)->pixels)[index] = tga.data[index2];
						((uint8_t *)(*surface)->pixels)[index + 1] = tga.data[index2 + 1];
						((uint8_t *)(*surface)->pixels)[index + 2] = tga.data[index2 + 2];
					}
				}
			}
		}
		else
			(*surface)->pixels = tga.data;
	}
	
	if (tga.colormap)
		ft_memdel((void **)&tga.colormap);
	close(r.fd);
	return (TRUE);
}
