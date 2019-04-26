/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_editor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/24 15:50:09 by llelievr          #+#    #+#             */
/*   Updated: 2019/04/26 11:43:06 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

// ///////////////////////////////////////TOOLS PRINT+DRAW///////////////////////////////////////////////////////////////////////////

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

void    print_poly(t_doom *doom, t_line_list *list)
{
    t_line_list *tmp = list;

    while (tmp != NULL)
    {
        draw_line(&doom->screen, (t_pixel){tmp->line.a.x * 20 + 10, tmp->line.a.y * 20 + 10, 0xFFFFFF}, (t_pixel){tmp->line.b.x * 20 + 10, tmp->line.b.y * 20 + 10});
        tmp = tmp->next;
    }
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

void	check_poly(t_doom *doom, t_line_list *list)
{
	t_line_list *tmp = list;
	t_line_list *cmp = list;
	int i = 0;

	while (tmp)
	{
		cmp = tmp->next;
		while (cmp)
		{
			printf("%f = %f, %f = %f, %f = %f, %f = %f\n", cmp->line.a.x, tmp->line.a.x, cmp->line.a.y, tmp->line.a.y, cmp->line.b.x, tmp->line.b.x, cmp->line.b.y, tmp->line.b.y);
			if ((cmp->line.b.x == tmp->line.a.x && cmp->line.b.y == tmp->line.a.y ) || (cmp->line.b.x == tmp->line.b.x && cmp->line.b.y == tmp->line.b.y))
				i++;
			cmp = cmp->next;
			printf("FERMETURE : %d\n", i);
		}
		printf("FIN\n");
		tmp = tmp->next;
	}
	if (i > 0)
		doom->editor.alert = 1;
}

void	editor_mousebuttonup(t_doom *doom, SDL_Event *event)
{
	if (doom->editor.click == 0)
	{
		doom->editor.line.a.x = (int)event->button.x / 20;
		doom->editor.line.a.y = (int)event->button.y / 20;
	}
	else
	{
		if (doom->editor.click > 1)
		{
			doom->editor.line.a.x = doom->editor.line.b.x;
			doom->editor.line.a.y = doom->editor.line.b.y;
		}
		doom->editor.line.b.x = (int)event->button.x / 20;
		doom->editor.line.b.y = (int)event->button.y / 20;
		append_list(&doom->editor.list, doom->editor.line);
		print_lst(doom->editor.list);
		check_poly(doom, doom->editor.list);
	}
	doom->editor.click++;
}
// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void		action_performed(t_component *cmp, t_doom *doom)
{
	
}

void	g_editor_on_enter(t_gui *self, t_doom *doom)
{
	if (!(doom->editor.point = (uint8_t *)malloc(sizeof(uint8_t) * (((doom->screen.width - 200) * doom->screen.height) / 20))))
		return ; //TODO: ERROR
	ft_bzero(doom->editor.point, sizeof(uint8_t) * (((doom->screen.width - 200) * doom->screen.height) / 20));
	doom->editor.alert = 0;
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
	if (doom->editor.alert == 0)
	{
		text = TTF_RenderText_Blended(doom->fonts.helvetica, "you have to close the polygon", color);
	    apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h}, (SDL_Rect){S_WIDTH - 350, 5, text->w + 5, text->h + 5});
	}
	else 
	{
		text = TTF_RenderText_Blended(doom->fonts.helvetica, "great ! All your poly are good!", color);
	    apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h}, (SDL_Rect){S_WIDTH - 350, 5, text->w + 5, text->h + 5});
	}
	print_poly(doom, doom->editor.list);
}