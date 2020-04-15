/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ressource_texture_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/15 03:44:25 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/15 03:45:27 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "ressource.h"
#include <SDL_image.h>

t_bool	is_tga_ext(char *path)
{
	char	*start;

	start = ft_strrchr(path, '.');
	return (start && *start && ft_memcmp(start + 1, "tga", 3) == 0);
}

t_bool			free_load_texture_surface(SDL_Surface *surface)
{
	if (surface)
		SDL_FreeSurface(surface);
	return (TRUE);
}

t_ressource		*get_default_texture(t_ressource_manager *rm, t_bool use)
{
	if (use)
		rm->ressources->values[0]->used++;
	return (rm->ressources->values[0]);
}
