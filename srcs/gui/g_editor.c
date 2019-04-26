/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_editor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/24 15:50:09 by llelievr          #+#    #+#             */
/*   Updated: 2019/04/26 19:14:57 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

// ///////////////////////////////////////SUPPRIMER///////////////////////////////////////////////////////////////////////////

void       print_lst(t_line_list *lst)
{
    t_line_list *tmp = lst;

    while (tmp != NULL)
    {
        printf("x = %f - y = %f  ||  ", tmp->line.a.x, tmp->line.a.y);
        printf("x = %f - y = %f\n", tmp->line.b.x, tmp->line.b.y);
        tmp = tmp->next;
    }
	printf("FIN LISTE\n");
}
// ///////////////////////////////////////TOOLS PRINT+DRAW///////////////////////////////////////////////////////////////////////////

void    print_poly(t_doom *doom, t_line_list *list)
{
    t_line_list *tmp = list;

    while (tmp != NULL)
    {
        draw_line(&doom->screen, (t_pixel){tmp->line.a.x * 20 + 10, tmp->line.a.y * 20 + 10, 0xFFFFFF}, (t_pixel){tmp->line.b.x * 20 + 10, tmp->line.b.y * 20 + 10});
        tmp = tmp->next;
    }
}

void	check_poly_close(t_doom *doom, t_line_list *list)
{
	t_line_list *cmp = list;
	int i = 0;

	while (cmp)
	{
		if ((cmp->line.b.x == doom->editor.first[0] && cmp->line.b.y == doom->editor.first[1]) || (cmp->line.b.x == doom->editor.first[0] && cmp->line.b.y == doom->editor.first[1]))
			i++;
		cmp = cmp->next;
	}
	if (i > 0)
		doom->editor.alert[0] = 1;
}

t_bool	check_multi_point(t_doom *doom, t_line_list *list, int x, int y)
{
	t_line_list *cmp = list;
	int i = 0;

	while (cmp)
	{
		if ((x == cmp->line.b.x && y == cmp->line.b.y) || (x == cmp->line.a.x && y == cmp->line.a.y))
			i++;
		if (i > 1)
		{
			doom->editor.alert[1] = 1;
			return FALSE;
		}
		cmp = cmp->next;
	}
	return TRUE;
}

// ///////////////////////////////////////A METTRE DANS UN AUTRE FICHIER .C (editor_event.c)///////////////////////////////////////////////////////////////////////////

void	editor_mouse_motion(t_doom *doom, SDL_Event *event)
{
	int x;
	int y;
	unsigned int index;

	x = event->motion.x;
    y = event->motion.y;
	if (x < (doom->screen.width - 180))
	{
		x /= 20;
		y /= 20;
		if (doom->editor.point[(y *  doom->screen.width / 20 ) + x] == 0)
		{
			ft_bzero(doom->editor.point, sizeof(uint8_t) * (((doom->screen.width - 200) * doom->screen.height) / 20));
			doom->editor.point[(y *  doom->screen.width / 20 ) + x] = 1;
		}
	}
}

void	editor_mousebuttonup(t_doom *doom, SDL_Event *event)
{
	if (doom->editor.click == 0)
	{
		doom->editor.line.a.x = (int)event->button.x / 20;
		doom->editor.line.a.y = (int)event->button.y / 20;
		if (doom->editor.first[0] == 0 && doom->editor.first[1] == 0)
		{
			doom->editor.first[0] = doom->editor.line.a.x;
			doom->editor.first[1] = doom->editor.line.a.y;
		}
	}
	else if (check_multi_point(doom, doom->editor.list, (int)event->button.x / 20, (int)event->button.y / 20) == TRUE)
	{
		doom->editor.alert[1] = 0;
		if (doom->editor.click > 1)
		{
			doom->editor.line.a.x = doom->editor.line.b.x;
			doom->editor.line.a.y = doom->editor.line.b.y;
		}
		doom->editor.line.b.x = (int)event->button.x / 20;
		doom->editor.line.b.y = (int)event->button.y / 20;
		append_list(&doom->editor.list, doom->editor.line);
		print_lst(doom->editor.list);
		check_poly_close(doom, doom->editor.list);
	}
	doom->editor.click++;
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
	doom->editor.alert = 0;
	if (!alloc_components(self, 1))
		return; 
	self->components[0] = create_button((SDL_Rect) { 5, 20, 200, 30 });
	self->components[0]->perform_action = action_performed;
	doom->editor.alert[0] = 0;
	// doom->editor.alert[1] = 0;
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
	if (event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_RIGHT)
		doom->editor.click = 0;
}

void	g_editor_render(t_gui *self, t_doom *doom)
{
	const SDL_Color	color = {255, 255, 255, 0};
	SDL_Surface		*text;
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
	if (doom->editor.alert[0] == 0)
	{
		text = TTF_RenderText_Blended(doom->fonts.helvetica, "you have to close the polygon", color);
	    apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h}, (SDL_Rect){S_WIDTH - 350, 5, text->w + 5, text->h + 5});
	}
	else if (doom->editor.alert[0] == 1)
	{
		text = TTF_RenderText_Blended(doom->fonts.helvetica, "Great!, your polygon is good", color);
	    apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h}, (SDL_Rect){S_WIDTH - 350, 5, text->w + 5, text->h + 5});
	}
	if (doom->editor.alert[1] == 1)
	{
		text = TTF_RenderText_Blended(doom->fonts.helvetica, "Non non non ! Pas bien!", color);
	    apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h}, (SDL_Rect){S_WIDTH - 350, 30, text->w + 5, text->h + 5});
	}
	print_poly(doom, doom->editor.list);
	render_components(doom, self);
}
