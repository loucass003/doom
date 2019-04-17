/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 21:21:40 by llelievr          #+#    #+#             */
/*   Updated: 2019/04/17 15:38:45 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IMAGE_H
# define IMAGE_H

# include "doom.h"

typedef struct		s_img
{
	uint32_t		*pixels;
	SDL_Texture		*texture;
	uint32_t		size;
	uint32_t		width;
	uint32_t		height;
	SDL_Renderer	*renderer;
	t_bool			ignore_texture;
}					t_img;

t_bool				create_image(SDL_Renderer *renderer, uint32_t width,
						uint32_t height, t_img *img);
t_bool				destroy_image(t_img *img);
void				clear_image(t_img *img);
void				update_image(t_img *img);
void				apply_image(t_img *img, SDL_Rect *dst);
void				fill_color(t_img *img, uint32_t color);

#endif

