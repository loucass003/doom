/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_gameover.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/15 20:19:09 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/15 15:37:46 by lloncham         ###   ########.fr       */
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
	t_img	*background;
	int		i;

	i = -1;
	printf("%d kill\n ", doom->gameover.kill);
	printf("%d bullets\n ", doom->gameover.bullets);
	printf("%d weapon\n ", doom->gameover.weapon);
	printf("%f damage\n ", doom->gameover.totaldamage);
	background = doom->res_manager.ressources->values[1]->data.texture;
	apply_image_to_image(&doom->screen, background, (SDL_Rect) {0, 0, background->width, background->height}, (SDL_Rect) {0, 0, S_WIDTH, S_HEIGHT});
	render_components(doom, self);
}
