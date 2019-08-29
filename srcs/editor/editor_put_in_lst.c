/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_put_in_lst.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 13:51:56 by lloncham          #+#    #+#             */
/*   Updated: 2019/08/29 03:08:02 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"
#include "doom.h"

void    save_object(t_doom *doom, int x, int y, SDL_Surface *obj)
{
	if (check_obj_legit(doom, x/20, y/20) == FALSE)
		return ;
	append_objects_array(&doom->editor.objects, 
		(t_object){ .pos = {x / 20, y / 20}, .img = obj });
}

// t_line_list	*complete_list(t_doom *doom, t_line line)
// {
// 	t_line_list		*elem;

// 	if (!(elem = (t_line_list*)malloc(sizeof(t_line_list))))
// 		return (0);
// 	elem->line = line;
// 	elem->texture = doom->editor.texture;
// 	elem->next = NULL;
// 	return (elem);
// }

void save_in_lst(t_doom *doom)
{
	const t_wall wall = (t_wall){
		.line = doom->editor.line,
		.img = doom->editor.texture
	};

	if (doom->editor.poly == 1)
		append_walls_array(&doom->editor.polygon, wall);
	if (doom->editor.lignes == 1)
		append_walls_array(&doom->editor.lines, wall);
	// if (doom->editor.porte == 1)
	// 	append_walls_array(doom, &doom->editor.door, wall);
	// if (doom->editor.secteur == 1)
	// 	append_walls_array(doom, &doom->editor.sector, wall);
}
