/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 13:20:27 by louali            #+#    #+#             */
/*   Updated: 2019/08/29 02:39:12 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL_image.h>
#include "editor.h"
#include "doom.h"

t_texture	*try1(char *name, int count)
{
	t_texture	*tmp;

	if (!(tmp = malloc(sizeof(t_texture))))
		return NULL;
	tmp->name = name;
	if (!(tmp->text = IMG_Load(tmp->name)) 
		|| !(tmp->text = SDL_ConvertSurfaceFormat(tmp->text, SDL_PIXELFORMAT_ARGB8888, 0)))
	{
		free(tmp);
		return NULL;
	}
	tmp->count = count;
	tmp->next = NULL;
	return (tmp);
}

void	load(t_texture **texture, char *name, char *path)
{
	t_files		*file;
	t_texture	*start;
	t_texture	*tmp;
	t_texture	*stock;
	int			count = 0;
	
	start = *texture;
	file = NULL;
	find_files(&file, &count, name, path);
	t_files *b = file;
	while(b)
	{
		tmp = try1(b->s, count);
		if (!*texture)
			*texture = tmp;
		else
		{
			stock = *texture;
			while (stock && stock->next)
				stock = stock->next;
			stock->next = tmp;
		}
		tmp = tmp->next;
		b = b->next;
	}
}

void	load_all(t_doom *doom)
{
	load(&doom->icons, "icons", "icons/");
	load(&doom->obj, "objects", "objects/");
	load(&doom->wall, "walls", "walls/");
}
