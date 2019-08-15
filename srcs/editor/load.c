/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 13:20:27 by louali            #+#    #+#             */
/*   Updated: 2019/08/15 17:59:58 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

t_texture	*try1(char *name, int count)
{
	t_texture	*tmp;

	tmp = malloc(sizeof(t_texture));
	tmp->name = name;
	tmp->text = IMG_Load(tmp->name);
	tmp->text = SDL_ConvertSurfaceFormat(tmp->text,
		SDL_PIXELFORMAT_ARGB8888, 0);
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
