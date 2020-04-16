/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_inventory.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lisa <Lisa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/16 17:34:28 by Lisa              #+#    #+#             */
/*   Updated: 2020/04/16 18:33:21 by Lisa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "player.h"

void				inventory_item_grenada(t_doom *doom, t_weapon *weapon)
{
	if (weapon->type != WEAPON_GRENADA)
		apply_image_blended(doom->main_context.image,
			weapon->animation->data.texture, weapon->curr_image,
			(SDL_Rect){ S_WIDTH_2 - 80 / 2, S_HEIGHT - 300, 300, 300 });
}

float				inventory_item_weapon(t_doom *doom, float ticks)
{
	t_itemstack	*is;
	t_weapon	*weapon;

	is = &doom->player.item[doom->player.selected_slot];
	if (is->of && is->of->type == ITEM_WEAPON)
	{
		weapon = &is->of->data.weapon;
		if (ticks > 1 && weapon->fireing)
		{
			ticks = 0;
			weapon->current_step++;
			if (weapon->current_step == weapon->steps_count)
			{
				weapon->current_step = (SDL_GetMouseState(NULL, NULL)
				& SDL_BUTTON(SDL_BUTTON_LEFT) ? 0 : weapon->idle_step);
				weapon->fireing = FALSE;
			}
			set_current_animation_step(weapon,
				weapon->animation_seq[weapon->current_step]);
		}
		inventory_item_grenada(doom, weapon);
	}
	return (ticks);
}

t_bool				apply_img(t_doom *doom, t_itemstack *is, int i)
{
	const SDL_Color	color = {255, 255, 0, 0};
	SDL_Surface		*text;

	apply_image_blended(doom->main_context.image,
		is->of->image->data.texture, is->of->bounds,
		(SDL_Rect){ 10, 50 + i * 60, 50, 50 });
	if (is->amount <= 1)
		return (TRUE);
	text = TTF_RenderText_Blended(doom->fonts.helvetica,
		ft_int_to_str(is->amount).str, color);
	apply_surface_blended(doom->main_context.image, text,
		(SDL_Rect){0, 0, text->w, text->h},
		(SDL_Rect){ 40, 50 + i * 60, 20, 20 });
	SDL_FreeSurface(text);
	return (FALSE);
}

t_bool				fill_inventory(t_doom *doom, int i)
{
	t_itemstack		*is;

	is = &doom->player.item[i];
	if (i == doom->player.selected_slot)
		fill_rect(doom->main_context.image, (SDL_Rect){ 8, 48 + i
			* 60, 54, 54 }, 0xFFFFFF00);
	fill_rect(doom->main_context.image, (SDL_Rect){ 10, 50 + i
		* 60, 50, 50 }, 0xFFFF0000);
	if (is->of)
		if (apply_img(doom, is, i))
			return (TRUE);
	return (FALSE);
}

t_bool				draw_player_inventory(t_doom *doom, t_gui *self)
{
	static float	ticks = 0;
	int				i;

	if (doom->main_context.type == CTX_NORMAL)
	{
		i = -1;
		while (++i < PLAYER_INV_SIZE)
		{
			if (fill_inventory(doom, i))
				continue;
		}
		ticks += doom->stats.delta * 30.;
		ticks = inventory_item_weapon(doom, ticks);
		((t_progress *)self->components->values[0])->value = doom->player.\
			entity.life * (1 / doom->player.entity.max_life) * 100;
		if (doom->player.entity.life <= 0 || (doom->closer_boss
		&& doom->closer_boss->dead))
		{
			set_gui(doom, GUI_GAMEOVER);
			return (FALSE);
		}
	}
	return (TRUE);
}
