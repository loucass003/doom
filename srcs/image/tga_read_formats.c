/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tga_read_formats.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/23 02:15:58 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/19 17:48:01 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tga.h"
#include "io.h"

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
		tga->data[(i * 3) + 1] = (uint8_t)(((color & 0x03E0) >> 5) << 3);
		tga->data[(i * 3) + 2] = (uint8_t)(((color & 0x001F) >> 0) << 3);
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
