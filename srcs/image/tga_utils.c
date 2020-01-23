/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tga_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/22 03:04:16 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/23 02:21:07 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tga.h"
#include "io.h"

t_bool		tga_return_error(char *error)
{
	ft_putstr("TGA Loader error: ");
	ft_putendl(error);
	return (FALSE);
}

void		flip_image(t_tga_format *tga, SDL_Surface *s)
{
	int		pos[2];
	int		i;
	int		j;

	pos[0] = -1;
	while (++pos[0] < s->h && !!(pos[1] = -1))
	{
		while (++pos[1] < s->w)
		{
			if (tga->header.pixel_depth == 32)
			{ 
				i = (s->h - 1 - pos[0]) * s->w + pos[1];
				j = pos[0] * s->w + pos[1];
				((uint32_t *)s->pixels)[i] = ((uint32_t *)tga->data)[j];
			}
			else
			{
				i = (s->h - 1 - pos[0]) * s->pitch + pos[1] * tga->bpp;
				j = pos[0] * s->pitch + pos[1] * tga->bpp;
				((uint8_t *)s->pixels)[i] = tga->data[j];
				((uint8_t *)s->pixels)[i + 1] = tga->data[j + 1];
				((uint8_t *)s->pixels)[i + 2] = tga->data[j + 2];
			}
		}
	}
}

t_bool		tga_move_to_start(t_reader *r, t_tga_format *tga)
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