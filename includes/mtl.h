/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mtl.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/04 22:13:55 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/07 18:45:40 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MTL_H
# define MTL_H

#include <libft.h>
#include <SDL.h>

# define MATERIAL_FILE_LEN 255
# define MATERIAL_TEXTURE_LEN 255
# define MATERIAL_NAME_LEN 255

typedef struct	s_mtl
{
	char		name[MATERIAL_NAME_LEN];
	t_bool		texture_map_set;
	SDL_Surface	*texture_map;
	uint8_t		*lightmap;
	t_bool		material_color_set;
	t_bool		transparent;
	int			material_color;
	char		*working_dir; //TODO: NEED TO BE LINKED
	t_bool		wireframe;
}				t_mtl;

typedef struct	s_mtllist
{
	int				len;
	int				capacity;
	t_mtl			values[];
}				t_mtllist;

t_mtllist		*create_mtllist(int capacity);
t_mtllist		*append_mtllist(t_mtllist **arr, t_mtl v);

#endif