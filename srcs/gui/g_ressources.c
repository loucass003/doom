/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_ressources.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/24 12:20:04 by lloncham          #+#    #+#             */
/*   Updated: 2020/01/28 14:27:56 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gui.h"
#include "doom.h"

static t_bool		action_performed(t_component *cmp, t_doom *doom)
{
	t_components *comp;

	comp = doom->guis[doom->current_gui].components;
	if (cmp == comp->values[PAGE_SIZE * 3 + 1])
		doom->res_manager.page--;
	else if (cmp == comp->values[PAGE_SIZE * 3 + 2])
		doom->res_manager.page++;
	if (doom->res_manager.page < 0)
		doom->res_manager.page = get_pages_count(&doom->res_manager);
	if (doom->res_manager.page > get_pages_count(&doom->res_manager))
		doom->res_manager.page = 0;
	if (cmp == comp->values[PAGE_SIZE * 3 + 3])
		a(doom, "", RESSOURCE_UNSET, FALSE);
	update_selects(&doom->guis[doom->current_gui], &doom->res_manager);
	if (cmp == comp->values[PAGE_SIZE * 3] && !check_ressources_errors(doom))
	{
		set_gui(doom, GUI_MAIN_MENU);
		return (FALSE);
	}
	return (TRUE);
}

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

void				g_ressources_on_enter(t_gui *self, t_doom *doom)
{
	int i;

	SDL_EventState(SDL_DROPFILE, SDL_ENABLE);
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
	append_components_array(&self->components, create_button((SDL_Rect)
		{S_WIDTH - 210, S_HEIGHT - 50, 200, 40}, NULL, "CONTINUE"));
	append_components_array(&self->components, create_button((SDL_Rect)
		{S_WIDTH_2 - 140, S_HEIGHT - 50, 40, 40}, NULL, "<"));
	append_components_array(&self->components, create_button((SDL_Rect)
		{S_WIDTH_2 + 100, S_HEIGHT - 50, 40, 40}, NULL, ">"));
	append_components_array(&self->components, create_button((SDL_Rect)
		{S_WIDTH - 215, 15, 200, 40}, NULL, "ADD RESSOURCE"));
	i = -1;
	while (++i < 4)
		self->components->values[(PAGE_SIZE * 3) + i]->perform_action =
			action_performed;
	update_selects(self, &doom->res_manager);
}

void				g_ressources_on_event(t_gui *self,
	SDL_Event *event, t_doom *doom)
{
	t_ressource		*r;
	t_vec2			pos;
	int				index;

	(void)self;
	if (event->type == SDL_DROPFILE)
	{
		pos = get_mouse_pos(doom);
		index = ((int)(pos.y - 53) / 30) + (doom->res_manager.page * PAGE_SIZE);
		if (index >= 0 && index < doom->res_manager.ressources->len)
		{
			r = doom->res_manager.ressources->values[index];
			if (r->type == RESSOURCE_UNSET)
				return ;
			load_ressource(doom, r, event->drop.file);
			SDL_free(event->drop.file);
			update_selects(&doom->guis[doom->current_gui], &doom->res_manager);
		}
	}
}

void				g_ressources_on_leave(t_gui *self, t_doom *doom)
{
	(void)self;
	SDL_EventState(SDL_DROPFILE, SDL_DISABLE);
	doom->screen.secure = FALSE;
}
