/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_editor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/24 15:50:09 by llelievr          #+#    #+#             */
/*   Updated: 2019/05/07 16:35:49 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

static void		action_performed(t_component *cmp, t_doom *doom)
{
	if (cmp == doom->guis[doom->current_gui].components[0])
	{
		t_node *node = create_node(doom->editor.list);
		build_node(node);
		doom->bsp = node;
		set_gui(doom, GUI_INGAME);
	}
	if (cmp == doom->guis[doom->current_gui].components[1])
		doom->editor.set_start++;
}

void			g_editor_on_enter(t_gui *self, t_doom *doom)
{
	if (!(doom->editor.point = (uint8_t *)malloc(sizeof(uint8_t) * (((doom->screen.width - 200) * doom->screen.height) / 20))))
		return ; //TODO: ERROR
	ft_bzero(doom->editor.point, sizeof(uint8_t) * (((doom->screen.width - 200) * doom->screen.height) / 20));
	set_alert_message(doom);
	doom->editor.set_start = 0;
	doom->editor.set_start_pos[0] = 0;
	doom->editor.set_start_pos[1] = 0;
	if (!alloc_components(self, 2))
		return;
	self->components[0] = create_button((SDL_Rect) { 5, 20, 200, 30 });
	self->components[0]->perform_action = action_performed;
	self->components[1] = create_button((SDL_Rect) { S_WIDTH - 350, 40, 200, 40});
	self->components[1]->perform_action = action_performed;
}

void			g_editor_on_leave(t_gui *self, t_doom *doom)
{
	free(doom->editor.point);
}

void			g_editor_on_event(t_gui *self, SDL_Event *event, t_doom *doom)
{
	if (event->type == SDL_MOUSEMOTION)
		editor_mouse_motion(doom, event);
	if (doom->editor.set_start % 2 == 0)
	{
		if (event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT && event->button.x < (doom->screen.width - 180))
			editor_mousebuttonup(doom, event);
	}
	else 
	{
		if (event->type == SDL_MOUSEBUTTONUP && event->button.x < (doom->screen.width - 180))
		{
			doom->editor.set_start_pos[0] = event->button.x / 20;
			doom->editor.set_start_pos[1] = event->button.y / 20;
		}
	}
}

void			g_editor_render(t_gui *self, t_doom *doom)
{
	const SDL_Color	color = {255, 255, 255, 0};
	SDL_Surface		*text;
	SDL_Surface		*image;
	int x;
	int y;

	y = -1;
	while (++y <= doom->screen.height / 20)
	{
		x = -1;
		while (++x <= doom->screen.width / 20)
		{
			if (x <= (doom->screen.width - 200) / 20)
				doom->screen.pixels[(y * 20 + 10) * doom->screen.width + (x * 20) + 10] = doom->editor.point[(y * (doom->screen.width / 20) ) + x] == 1 ? 0xFF0000 : 0xFFFFFF;
		}
	}
	write_alert_message(doom);
	print_poly(doom, doom->editor.polygon);
	render_components(doom, self);
	text = TTF_RenderText_Blended(doom->fonts.helvetica, "set start pos", color);
	apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h}, (SDL_Rect){S_WIDTH - 340, 50, text->w, text->h});
	if (doom->editor.set_start_pos[0] != 0)
	{
		image = IMG_Load("pin.png");
		apply_surface(&doom->screen, image, (SDL_Rect){0, 0, 500, 500}, (SDL_Rect){doom->editor.set_start_pos[0] * 20 - 5, doom->editor.set_start_pos[1] * 20 - 20, 30, 30});
	} //RESET!
}