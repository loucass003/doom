/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tga.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/19 16:55:03 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/19 18:33:09 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TGA_H
# define TGA_H

# include <libft.h>

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

typedef struct	s_tga_color_map
{
	uint16_t	first_entry_index;
	uint16_t	color_map_length;
	uint8_t		color_map_entry_size;
}				t_tga_color_map;

typedef struct	s_tga_image
{
	uint16_t	x_origin;
	uint16_t	y_origin;
	uint16_t	width;
	uint16_t	height;
	uint8_t		pixel_depth;
	uint8_t		image_descriptor;
}				t_tga_image;

typedef struct	s_tga_header
{
	uint8_t		id_length;
	uint8_t		color_map_type;
	uint8_t		image_type;
	t_tga_image	image;
}				t_tga_header;

#endif