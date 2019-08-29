/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 21:21:40 by llelievr          #+#    #+#             */
/*   Updated: 2019/08/29 16:24:25 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IMAGE_H
# define IMAGE_H

#include <SDL.h>
#include <libft.h>

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
void				draw_line(t_img *img, t_pixel p0, t_pixel p1);
void				draw_circle(t_img *img, t_pixel center, int r);
Uint32				get_surface_pixel(SDL_Surface *surface, int x, int y);
void				apply_surface_blended(t_img *img, SDL_Surface *s,
						SDL_Rect src, SDL_Rect dst);
void				apply_surface(t_img *img, SDL_Surface *s,
						SDL_Rect src, SDL_Rect dst);

#endif

