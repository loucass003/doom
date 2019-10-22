/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_surface.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 22:28:48 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/22 00:52:34 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "image.h"

inline Uint32			get_surface_pixel(SDL_Surface *surface, int x, int y)
{
	return (*(Uint32 *)(surface->pixels + y * surface->pitch + x * 4));
}

inline t_bool			put_pixel(t_img *img, uint32_t x, uint32_t y, int color)
{
	if (img->secure && (x < 0 || x >= img->width || y < 0 || y >= img->height))
		return (FALSE);
	img->pixels[(y * img->width) + x] = color;
	return (TRUE);
}

void			apply_surface_blended(t_img *img, SDL_Surface *s, SDL_Rect src,
	SDL_Rect dst)
{
	int				i;
	int				j;
	const float		s_w = src.w / (float)dst.w;
	const float		s_h = src.h / (float)dst.h;

	i = -1;
	while (++i < dst.h)
	{
		j = -1;
		while (++j < dst.w)
		{
			if (!put_pixel(img, dst.x + j, dst.y + i, ft_color_i(ft_alpha_blend(
				ft_i_color(get_surface_pixel(s, (uint32_t)(j * s_w) + src.x,
					(uint32_t)(i * s_h) + src.y)),
				ft_i_color(img->pixels[
						((dst.y + i) * img->width) + (dst.x + j)])))))
				break;
		}
	}
}

void			apply_surface(t_img *img, SDL_Surface *s,
		SDL_Rect src, SDL_Rect dst)
{
	int				i;
	int				j;
	const SDL_Rect	d2 = dst;
	const t_vec2	s_v = (t_vec2){src.w / (float)dst.w, src.h / (float)dst.h};
	t_pixel			s_p;

	s_p = (t_pixel){ft_min(img->width - 1, dst.x + dst.w),
					ft_min(img->height - 1, dst.y + dst.h), 0};
	if (dst.y < 0)
		dst.y = 0;
	if (dst.x < 0)
		dst.x = 0;
	src.x = ft_max(0, ft_min(img->width - 1, src.x));
	src.y = ft_max(0, ft_min(img->height - 1, src.y));
	i = dst.y - 1;
	while (++i < s_p.y)
	{
		j = dst.x - 1;
		while (++j < s_p.x)
		{
			if (!put_pixel(img, j, i, get_surface_pixel(s,
			(int)((j - d2.x) * s_v.x) + src.x,
			(int)((i - d2.y) * s_v.y) + src.y)))
				break;
		}
	}
}

void			apply_image_to_image(t_img *img, t_img *s,
		SDL_Rect src, SDL_Rect dst)
{
	int				i;
	int				j;
	const SDL_Rect	d2 = dst;
	const t_vec2	s_v = (t_vec2){src.w / (float)dst.w, src.h / (float)dst.h};
	t_pixel			s_p;

	s_p = (t_pixel){ft_min(img->width - 1, dst.x + dst.w),
					ft_min(img->height - 1, dst.y + dst.h), 0};
	if (dst.y < 0)
		dst.y = 0;
	if (dst.x < 0)
		dst.x = 0;
	src.x = ft_max(0, ft_min(img->width - 1, src.x));
	src.y = ft_max(0, ft_min(img->height - 1, src.y));
	i = dst.y - 1;
	while (++i < s_p.y)
	{
		j = dst.x - 1;
		while (++j < s_p.x)
		{
			img->pixels[((i * img->width) + j)] = s->pixels[(((int)((i - d2.y) * s_v.y) + src.y) * s->width) + ((int)((j - d2.x) * s_v.x) + src.x)];
		}
	}
}