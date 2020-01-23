/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tga.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/19 16:55:03 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/23 02:19:16 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TGA_H
# define TGA_H

# include <libft.h>
# include <stdio.h>
# include <SDL.h>
# include "io.h"

typedef enum	s_tga_image_type
{
	TGA_IMAGE_TYPE_NODATA = 0,
	TGA_IMAGE_TYPE_COLORS = 1,
	TGA_IMAGE_TYPE_TRUECOLORS = 2,
	TGA_IMAGE_TYPE_MONOCHROME = 3,
	TGA_IMAGE_TYPE_INDEXED_COMPRESS = 9,
	TGA_IMAGE_TYPE_TRUECOLOR_COMPRESS = 10,
	TGA_IMAGE_TYPE_MONOCHROME_COMPRESS = 11, 
}				t_tga_image_type;


typedef struct	s_img_descriptor
{
	uint8_t		meh : 3;
	uint8_t		reserved: 1;
	uint8_t		origin: 1;
	uint8_t		interleaving: 2;
}				t_img_descriptor;

# pragma pack(push, 1)

typedef struct	s_tga_header
{
	uint8_t				id_lenght;
	uint8_t				color_map_type;
	uint8_t				image_type;
	uint16_t			cm_first_entry;
	uint16_t			cm_length;
	uint8_t				cm_size;
	uint16_t			x_origin;
	uint16_t			y_origin;
	uint16_t			width;
	uint16_t			height;
	uint8_t				pixel_depth;
	t_img_descriptor	image_descriptor;
}				t_tga_header;

# pragma pack(pop)

typedef struct	s_tga_format
{
	t_tga_header	header;
	uint8_t			bpp;
	int				format; 
	uint8_t			*data;
	uint8_t			*colormap;
}				t_tga_format;

t_bool			tga_return_error(char *error);
t_bool			load_tga(char *path, SDL_Surface **surface);
t_bool			free_tga(t_tga_format *tga);
t_bool			tga_move_to_start(t_reader *r, t_tga_format *tga);
void			flip_image(t_tga_format *tga, SDL_Surface *s);
t_bool			read_tga_data(t_reader *r, t_tga_format *tga);
t_bool			read_tga_32bits(t_reader *r, t_tga_format *tga);
t_bool			read_tga_24bits(t_reader *r, t_tga_format *tga);
t_bool			read_tga_16bits(t_reader *r, t_tga_format *tga);
t_bool			read_tga_8bits(t_reader *r, t_tga_format *tga);

#endif