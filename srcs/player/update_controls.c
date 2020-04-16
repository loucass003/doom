/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_controls.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lisa <Lisa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/16 17:28:32 by Lisa              #+#    #+#             */
/*   Updated: 2020/04/16 19:48:28 by Lisa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "player.h"

float				update_speed(t_doom *doom, const Uint8 *s)
{
	float			move_speed;

	if (doom->main_context.type == CTX_EDITOR)
		move_speed = 10;
	else if (doom->player.entity.jetpack)
		move_speed = 15;
	else if (s[SDL_SCANCODE_LSHIFT] && !doom->player.entity.jetpack
		&& !doom->player.entity.jump)
	{
		doom->player.entity.run = TRUE;
		move_speed = 15;
	}
	else
		move_speed = !doom->player.entity.grounded ? 1.2 : 10;
	return (move_speed);
}

void				update_direction(t_doom *doom, const Uint8 *s,
	float move_speed)
{
	if (s[SDL_SCANCODE_W] || s[SDL_SCANCODE_S])
	{
		doom->player.entity.velocity.x += sinf(doom->player.entity.rotation.y)
			* (s[SDL_SCANCODE_W] ? 1 : -1) * move_speed;
		doom->player.entity.velocity.z += cosf(doom->player.entity.rotation.y)
			* (s[SDL_SCANCODE_W] ? 1 : -1) * move_speed;
	}
	if (s[SDL_SCANCODE_A] || s[SDL_SCANCODE_D])
	{
		doom->player.entity.velocity.x += -cosf(doom->player.entity.rotation.y)
			* (s[SDL_SCANCODE_D] ? 1 : -1) * move_speed;
		doom->player.entity.velocity.z += sinf(doom->player.entity.rotation.y)
			* (s[SDL_SCANCODE_D] ? 1 : -1) * move_speed;
	}
}

void				update_up_down(t_doom *doom, const Uint8 *s)
{
	if (doom->main_context.type == CTX_NORMAL)
	{
		if (s[SDL_SCANCODE_SPACE] && !doom->player.entity.jump
		&& (doom->player.entity.grounded || doom->player.entity.jetpack))
			doom->player.entity.jump = TRUE;
		if (s[SDL_SCANCODE_LSHIFT] && doom->player.entity.jetpack)
		{
			doom->player.entity.grounded = FALSE;
			doom->player.entity.velocity.y -= 8;
		}
	}
	if (doom->main_context.type == CTX_EDITOR)
	{
		if (s[SDL_SCANCODE_SPACE])
			doom->player.entity.velocity.y += 8;
		if (s[SDL_SCANCODE_LSHIFT])
			doom->player.entity.velocity.y -= 8;
	}
}

void				update_mouse_focus(t_doom *doom)
{
	int				m_x;
	int				m_y;
	float			rot;
	const double	ms = doom->stats.delta * 2.;

	rot = 0;
	SDL_GetRelativeMouseState(&m_x, &m_y);
	if (m_x != 0)
		doom->player.entity.rotation.y -= m_x * ms * 0.01;
	if (m_y != 0)
	{
		rot = m_y * ms * 0.01;
		if (doom->player.entity.rotation.x - rot < M_PI_2
		&& doom->player.entity.rotation.x - rot > -M_PI_2)
			doom->player.entity.rotation.x -= rot;
	}
}

void				update_controls(t_doom *doom)
{
	const Uint8		*s = SDL_GetKeyboardState(NULL);
	float			move_speed;

	doom->player.entity.run = FALSE;
	if (!doom->mouse_focus && is_settings_open(&doom->editor))
		return ;
	move_speed = update_speed(doom, s);
	update_up_down(doom, s);
	update_direction(doom, s, move_speed);
	if (doom->mouse_focus)
		update_mouse_focus(doom);
	update_player_camera(&doom->player);
}
