/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_gameover.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/15 20:19:09 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/31 14:15:56 by louali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gui.h"
#include "doom.h"

int					total_score(t_doom *doom)
{
	int	totalscore;

	if (doom->gameover.kill == 0 || doom->gameover.bullets == 0)
		totalscore = (doom->gameover.weapon * 30)
			+ (doom->gameover.totaldamage * 100.);
	else
		totalscore = ((doom->gameover.kill * 20) / doom->gameover.bullets)
			+ (doom->gameover.weapon * 30)
			+ (doom->gameover.totaldamage * 100.);
	return (totalscore);
}

t_bool				add_score(char *text, int score, t_doom *doom)
{
	t_score		*s;
	int		i;

	i = -1;
	while (++i < 5)
	{
		s = &doom->scores[i];
		if (s->score >= score)
			continue;
		add_score(s->text, s->score, doom);
		s->score = score;
		ft_bzero(s->text, 255);
		ft_memcpy(s->text, text, ft_strlen(text));
		break;
	}
	return (TRUE);
}

static t_bool		action_performed(t_component *cmp, t_doom *doom)
{
	char	*text;

	if (cmp == doom->guis[doom->current_gui].components->values[0])
	{
		set_gui(doom, GUI_MAIN_MENU);
		return (FALSE);
	}
	if (cmp == doom->guis[doom->current_gui].components->values[2])
	{
		if (((t_textfield *)doom->guis[doom->current_gui].components->values[1])->text_len > 0)
		{
			text = ((t_textfield *)doom->guis[doom->current_gui].components->values[1])->text;
			if (!add_score(text, total_score(doom), doom))
				return (FALSE);
			set_gui(doom, GUI_LEADERBOARD);
		}
		return (FALSE);
	}
	return (TRUE);
}

void				g_gameover_on_enter(t_gui *self, t_doom *doom)
{
	doom->mouse_focus = FALSE;
	append_components_array(&self->components, create_button((SDL_Rect){
		(S_WIDTH - 250) / 2, (S_HEIGHT - 150) / 2,
		250, 50 }, NULL, "MAIN MENU"));
	((t_button *)self->components->values[0])->color_default = 0xFF8d1506;
	((t_button *)self->components->values[0])->color_hover = 0;
	self->components->values[0]->perform_action = action_performed;
	if (doom->player.entity.life > 0)
	{
		append_components_array(&self->components,
			create_textfield((SDL_Rect){(S_WIDTH - 250) / 2,
			(S_HEIGHT) / 2, 250, 50}, "Enter your name", TRUE));
		append_components_array(&self->components,
			create_button((SDL_Rect){ (S_WIDTH - 250) / 2,
			(S_HEIGHT + 150) / 2, 250, 50 }, NULL, "Save"));
		((t_button *)self->components->values[2])->color_default = 0xFF8d1506;
		((t_button *)self->components->values[2])->color_hover = 0;
		self->components->values[2]->perform_action = action_performed;
	}
}

void				g_gameover_on_leave(t_gui *self, t_doom *doom)
{
	(void)self;
	(void)doom;
}

void				text_score(t_doom *doom, char *str, int x, int y)
{
	SDL_Surface		*text;
	const SDL_Color	color = {255, 255, 255, 0};

	text = TTF_RenderText_Blended(doom->fonts.helvetica, str, color);
	apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h}, (SDL_Rect){x, y, text->w, text->h});
	SDL_FreeSurface(text);
}

void				g_gameover_render(t_gui *self, t_doom *doom)
{
	t_img			*background;

	if (doom->player.entity.life <= 0)
		background = doom->res_manager.ressources->values[1]->data.texture;
	else
		background = doom->res_manager.ressources->values[10]->data.texture;
	apply_image_to_image(&doom->screen, background,
		(SDL_Rect) {0, 0, background->width, background->height},
		(SDL_Rect) {0, 0, S_WIDTH, S_HEIGHT});
	text_score(doom, ft_int_to_str(doom->gameover.kill).str, 355, 104);
	text_score(doom, ft_int_to_str(doom->gameover.totaldamage * 100.).str, 355, 155);
	text_score(doom, ft_int_to_str(doom->gameover.bullets).str, 355, 206);
	text_score(doom, ft_int_to_str((int)doom->gameover.weapon).str, 355, 260);
	text_score(doom, ft_int_to_str((int)total_score(doom)).str, 215, 60);
	render_components(doom, self);
}
