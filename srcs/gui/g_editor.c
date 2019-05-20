/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_editor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/24 15:50:09 by llelievr          #+#    #+#             */
/*   Updated: 2019/05/20 16:22:56 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

static void		action_performed(t_component *cmp, t_doom *doom)
{
	t_node	*node;

	if (cmp == doom->guis[doom->current_gui].components[0])
	{
		node = create_node(doom->editor.list);
		build_node(node);
		doom->bsp = node;
		set_gui(doom, GUI_INGAME);
	}
	if (cmp == doom->guis[doom->current_gui].components[1])
		doom->editor.set_start++;
	if (cmp == doom->guis[doom->current_gui].components[2])
		doom->editor.sup = 1;
}

void			g_editor_on_enter(t_gui *self, t_doom *doom)
{
	if (!(doom->editor.point = (uint8_t *)malloc(sizeof(uint8_t)
		* (((doom->screen.width - 200) * doom->screen.height) / 20))))
		return ; //TODO: ERROR
	ft_bzero(doom->editor.point, sizeof(uint8_t)
		* (((doom->screen.width - 200) * doom->screen.height) / 20));
	set_alert_message(doom);
	doom->editor.set_start = 0;
	doom->editor.sup = 0;
	doom->editor.set_start_pos[0] = 0;
	doom->editor.set_start_pos[1] = 0;
	if (!alloc_components(self, 5))
		return ;
	self->components[0] = create_button((SDL_Rect){5, 20, 200, 30 });
	self->components[0]->perform_action = action_performed;
	self->components[1] = create_button((SDL_Rect){S_WIDTH - 170, 50, 150, 35});
	self->components[1]->perform_action = action_performed;
	self->components[2] = create_button((SDL_Rect)
		{S_WIDTH - 170, 100, 150, 40});
	self->components[2]->perform_action = action_performed;
		self->components[3] = create_menu((SDL_Rect)
	{MT_X, MT_Y, MT_W, MT_H }, "textures", "textures/");
	self->components[3]->perform_action = action_performed;
	self->components[4] = create_menu((SDL_Rect)
	{W_WIDTH - 300, 30, 75, 75 }, "icones", "icones/");
	self->components[4]->perform_action = action_performed;
}

void			g_editor_on_leave(t_gui *self, t_doom *doom)
{
	free(doom->editor.point);
}

void			g_editor_on_event(t_gui *self, SDL_Event *event, t_doom *doom)
{
	if (event->type == SDL_MOUSEMOTION)
		editor_mouse_motion(doom, event);
	if (event->type == SDL_MOUSEBUTTONUP
		&& event->button.x < (doom->screen.width - 180))
	{
		if (doom->editor.set_start % 2 == 0 && doom->editor.sup == 0)
			editor_mousebuttonup(doom, event);
		else if (doom->editor.set_start % 2 != 0)
		{
			doom->editor.set_start_pos[0] = event->button.x / 20;
			doom->editor.set_start_pos[1] = event->button.y / 20;
		}
		else if (doom->editor.sup >= 1)
			save_line_to_erase(doom, event);
	}
}

void			g_editor_render(t_gui *self, t_doom *doom)
{
	int				x;
	int				y;

	y = -1;
	while (++y <= doom->screen.height / 20)
	{
		x = -1;
		while (++x <= doom->screen.width / 20)
		{
			if (x <= (doom->screen.width - 200) / 20)
				doom->screen.pixels[(y * 20 + 10) * doom->screen.width
				+ (x * 20) + 10] = doom->editor.point[(y * (doom->screen.width
				/ 20)) + x] == 1 ? 0xFF0000 : 0xFFFFFF;
		}
	}
	write_alert_message(doom);
	print_poly(doom, doom->editor.polygon);
	render_components(doom, self);
	write_text_button(doom);
	show_new_components(doom);
	//FREE POUR RESET
}
