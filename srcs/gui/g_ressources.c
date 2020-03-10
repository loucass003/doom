/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_ressources.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/24 12:20:04 by lloncham          #+#    #+#             */
/*   Updated: 2020/03/10 21:33:03 by llelievr         ###   ########.fr       */
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

void				g_ressources_on_enter(t_gui *self, t_doom *doom)
{
	int i;

	g_ressources_row_cmps(self, doom);
	append_components_array(&self->components, create_button((SDL_Rect)
		{S_WIDTH - 210, S_HEIGHT - 50, 200, 40}, NULL, "CONTINUE"));
	append_components_array(&self->components, create_button((SDL_Rect)
		{S_WIDTH_2 - 140, S_HEIGHT - 50, 40, 40}, NULL, "<"));
	append_components_array(&self->components, create_button((SDL_Rect)
		{S_WIDTH_2 + 100, S_HEIGHT - 50, 40, 40}, NULL, ">"));
	append_components_array(&self->components, create_button((SDL_Rect)
		{S_WIDTH - 215, 15, 200, 40}, NULL, "ADD RESSOURCE"));
	append_components_array(&self->components, create_button((SDL_Rect)
		{15, 15, 220, 40}, NULL, "MAPPING (JSON)"));
	i = -1;
	while (++i < 4)
		self->components->values[(PAGE_SIZE * 3) + i]->perform_action =
			action_performed;
	update_selects(self, &doom->res_manager);
}

void				g_ressources_on_event(t_gui *s,
	SDL_Event *event, t_doom *doom)
{
	t_ressource		*r;
	t_vec2			pos;
	int				index;

	(void)s;
	if (event->type == SDL_DROPFILE)
	{
		pos = get_mouse_pos(doom);
		index = ((int)(pos.y - 53) / 30) + (doom->res_manager.page * PAGE_SIZE);
		if (index >= 0 && index < doom->res_manager.ressources->len
			&& pos.x >= S_WIDTH_2 - 356 && pos.x <= S_WIDTH_2 + 228)
		{
			r = doom->res_manager.ressources->values[index];
			if (r->type == RESSOURCE_UNSET)
				return ;
			if (!load_ressource(doom, r, event->drop.file))
				free_ressource(&r);
		}
		else if (in_bounds(s->components->values[(PAGE_SIZE * 3) + 4]->
			bounds, pos))
			ressource_mapper(&doom->res_manager, event->drop.file);
		update_selects(&doom->guis[doom->current_gui], &doom->res_manager);
	}
}

void				g_ressources_on_leave(t_gui *self, t_doom *doom)
{
	(void)self;
	doom->screen.secure = FALSE;
}
