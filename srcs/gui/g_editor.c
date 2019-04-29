/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_editor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/24 15:50:09 by llelievr          #+#    #+#             */
/*   Updated: 2019/04/26 19:16:38 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

// ///////////////////////////////////////SUPPRIMER///////////////////////////////////////////////////////////////////////////

// void       print_lst(t_line_list *lst)
// {
//     t_line_list *tmp = lst;

//     while (tmp != NULL)
//     {
//         printf("x = %f - y = %f  ||  ", tmp->line.a.x, tmp->line.a.y);
//         printf("x = %f - y = %f\n", tmp->line.b.x, tmp->line.b.y);
//         tmp = tmp->next;
//     }
// 	printf("FIN LISTE\n");
// }
// ///////////////////////////////////////A METTRE DANS UN AUTRE FICHIER .C (editor_event.c)///////////////////////////////////////////////////////////////////////////

void	write_alert_message(t_doom *doom)
{
	const SDL_Color	color = {255, 255, 255, 0};
	SDL_Surface		*text;

	if (doom->editor.alert[0] == 0)
	{
		text = TTF_RenderText_Blended(doom->fonts.helvetica, "you have to close the polygon", color);
	    apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h}, (SDL_Rect){S_WIDTH - 350, 5, text->w + 5, text->h + 5});
	}
	else if (doom->editor.alert[0] == 1)
	{
		text = TTF_RenderText_Blended(doom->fonts.helvetica, "Great! your polygon is good", color);
	    apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h}, (SDL_Rect){S_WIDTH - 350, 5, text->w + 5, text->h + 5});
	}
	if (doom->editor.alert[1] == 1)
	{
		text = TTF_RenderText_Blended(doom->fonts.helvetica, "Non non non ! Pas cool!", color);
	    apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h}, (SDL_Rect){S_WIDTH - 350, 30, text->w + 5, text->h + 5});
	}
	if (doom->editor.alert[2] == 1)
	{
		text = TTF_RenderText_Blended(doom->fonts.helvetica, "Non non non ! Pas sur les lignes", color);
	    apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h}, (SDL_Rect){S_WIDTH - 350, 30, text->w + 5, text->h + 5});
	}
}

// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void		action_performed(t_component *cmp, t_doom *doom)
{
	if (cmp == doom->guis[doom->current_gui].components[0])
	{
		t_node *node = create_node(doom->editor.list);
		build_node(node);
		doom->bsp = node;
		set_gui(doom, GUI_INGAME);
	}
}

void	g_editor_on_enter(t_gui *self, t_doom *doom)
{
	if (!(doom->editor.point = (uint8_t *)malloc(sizeof(uint8_t) * (((doom->screen.width - 200) * doom->screen.height) / 20))))
		return ; //TODO: ERROR
	ft_bzero(doom->editor.point, sizeof(uint8_t) * (((doom->screen.width - 200) * doom->screen.height) / 20));
	doom->editor.alert[0] = 0;
	if (!alloc_components(self, 1))
		return;
	self->components[0] = create_button((SDL_Rect) { 5, 20, 200, 30 });
	self->components[0]->perform_action = action_performed;
}

void	g_editor_on_leave(t_gui *self, t_doom *doom)
{
	free(doom->editor.point);
}

void	g_editor_on_event(t_gui *self, SDL_Event *event, t_doom *doom)
{
	if (event->type == SDL_MOUSEMOTION)
		editor_mouse_motion(doom, event);
	if (event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT && event->button.x < (doom->screen.width - 180))
		editor_mousebuttonup(doom, event);
}

void	g_editor_render(t_gui *self, t_doom *doom)
{
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
}