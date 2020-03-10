/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_ressources_components.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/10 21:32:24 by llelievr          #+#    #+#             */
/*   Updated: 2020/03/10 21:32:55 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "editor.h"

static t_bool		del_res_performed(t_component *cmp, t_doom *doom)
{
	int					i;
	int					index;
	t_ressource_manager res;

	i = -1;
	res = doom->res_manager;
	while (++i < PAGE_SIZE)
	{
		if (cmp == doom->guis[doom->current_gui].components->values[i * 3 + 2])
		{
			index = i + (res.page * PAGE_SIZE);
			if (index >= 0 && index < res.ressources->len
				&& res.ressources->values[index]->used == 0)
			{
				splice_ressources_array(res.ressources, index, 1);
				if (res.page > get_pages_count(&res))
					doom->res_manager.page = get_pages_count(&res);
				update_selects(&doom->guis[doom->current_gui],
					&doom->res_manager);
			}
			return (TRUE);
		}
	}
	return (TRUE);
}

void				g_ressources_row_cmps(t_gui *self, t_doom *doom)
{
	int i;

	i = -1;
	while (++i < PAGE_SIZE)
	{
		append_components_array(&self->components, create_select((SDL_Rect)
			{S_WIDTH_2, 55 + i * 30, 200, 25}, "RESSOURCE TYPE"));
		self->components->values[i * 3]->visible = FALSE;
		((t_select *)self->components->values[i * 3])->items =
			doom->res_manager.ressources_types;
		append_components_array(&self->components, create_textfield((SDL_Rect)
			{S_WIDTH_2 - 353, 55 + i * 30, 350, 25}, "RESSOURCE NAME", FALSE));
		self->components->values[i * 3 + 1]->visible = FALSE;
		append_components_array(&self->components, create_button((SDL_Rect)
			{S_WIDTH_2 + 202, 55 + i * 30, 25, 25}, NULL, "X"));
		self->components->values[i * 3 + 2]->visible = FALSE;
		self->components->values[i * 3 + 2]->perform_action = del_res_performed;
	}
}
