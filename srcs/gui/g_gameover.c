/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_gameover.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lisa <Lisa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/15 20:19:09 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/19 14:50:09 by Lisa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gui.h"
#include "doom.h"

static void		action_performed(t_component *cmp, t_doom *doom)
{
	if (cmp == doom->guis[doom->current_gui].components->values[0])
	{
		set_gui(doom, GUI_MAIN_MENU);
	}
}

void	g_gameover_on_enter(t_gui *self, t_doom *doom)
{
	// if (!alloc_components(self, 1))
	// 	return ;
	SDL_SetRelativeMouseMode(SDL_FALSE);
//	printf("init %d\n", self->component_count);

	append_components_array(&self->components, create_button((SDL_Rect){ (S_WIDTH - 250) / 2, (S_HEIGHT - 150) / 2, 250, 50 }, NULL, "MAIN MENU"));
	((t_button *)self->components->values[0])->color_default = 0xFF8d1506;
	((t_button *)self->components->values[0])->color_hover = 0;
	self->components->values[0]->perform_action = action_performed;
}

void	g_gameover_on_leave(t_gui *self, t_doom *doom)
{
}

void	g_gameover_render(t_gui *self, t_doom *doom)
{
	t_img			*background;
	SDL_Surface		*text;
	float			totalscore;
	const SDL_Color	color = {255, 255, 255, 0};
	
	background = doom->res_manager.ressources->values[1]->data.texture;
	apply_image_to_image(&doom->screen, background, (SDL_Rect) {0, 0, background->width, background->height}, (SDL_Rect) {0, 0, S_WIDTH, S_HEIGHT});
	
	text = TTF_RenderText_Blended(doom->fonts.helvetica, ft_int_to_str(doom->gameover.kill).str, color);
	apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h}, (SDL_Rect){355, 104, text->w, text->h});
	SDL_FreeSurface(text);

	text = TTF_RenderText_Blended(doom->fonts.helvetica, ft_int_to_str(doom->gameover.bullets).str, color);
	apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h}, (SDL_Rect){355, 155, text->w, text->h});
	SDL_FreeSurface(text);

	text = TTF_RenderText_Blended(doom->fonts.helvetica, ft_int_to_str(doom->gameover.weapon).str, color);
	apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h}, (SDL_Rect){355, 206, text->w, text->h});
	SDL_FreeSurface(text);

	text = TTF_RenderText_Blended(doom->fonts.helvetica, ft_int_to_str((int)doom->gameover.totaldamage).str, color);
	apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h}, (SDL_Rect){355, 260, text->w, text->h});
	SDL_FreeSurface(text);
	
	if (doom->gameover.kill == 0 && doom->gameover.bullets == 0)
		totalscore = (doom->gameover.weapon * 30) + (doom->gameover.totaldamage * 15);
	else
		totalscore = ((doom->gameover.kill * 20) / doom->gameover.bullets) + (doom->gameover.weapon * 30) + (doom->gameover.totaldamage * 15);
	
	text = TTF_RenderText_Blended(doom->fonts.helvetica, ft_int_to_str((int)totalscore).str, color);
	apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h}, (SDL_Rect){215, 60, text->w, text->h});
	SDL_FreeSurface(text);

	render_components(doom, self);
}
