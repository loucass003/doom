/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_ingame.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/24 11:22:28 by llelievr          #+#    #+#             */
/*   Updated: 2019/12/19 18:59:07 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "doom.h"
#include <limits.h>
#include "gui.h"
#include "octree.h"
#include <math.h>
#include "render.h"
#include "editor.h"

void static	action_performed(t_component *cmp, t_doom *doom)
{
	
}

void	g_ingame_on_enter(t_gui *self, t_doom *doom)
{
	enter_gui(doom, doom->guis, GUI_EDITOR_SETTINGS);
	doom->screen.secure = FALSE;
	doom->mouse_focus = TRUE;
	append_components_array(&self->components, create_progress((SDL_Rect){ 5, 5, 200, 30 }));
	((t_progress *)self->components->values[0])->value = 50;
	((t_progress *)self->components->values[0])->bg_color = 0xFFFF0000;
	((t_progress *)self->components->values[0])->fg_color = 0xFF00FF00;
	append_components_array(&self->components, create_progress((SDL_Rect){ S_WIDTH_2 - 200, 5, 400, 20 }));
	((t_progress *)self->components->values[1])->value = 50;
	((t_progress *)self->components->values[1])->bg_color = 0xFFFF0000;
	((t_progress *)self->components->values[1])->fg_color = 0xFF00FF00;
}

void	g_ingame_on_leave(t_gui *self, t_doom *doom)
{
	
	leave_gui(doom, doom->guis, GUI_EDITOR_SETTINGS);
	doom->mouse_focus = FALSE;
}

void	unselect_all(t_doom *doom)
{
	if (doom->editor.current_object != -1)
				doom->editor.objects->values[doom->editor.current_object].r->show_hitbox = FALSE;
	doom->editor.current_object = -1;
	doom->editor.wall_section = -1;
	select_room(&doom->editor, -1);
}

char		get_side_thin(t_line partition, t_vec2 v, t_vec2 n)
{
	const float		side = (v.x - partition.a.x) * n.x
		+ (v.y - partition.a.y) * n.y;
	if (side < 0)
		return (-1);
	else if (side > 0)
		return (1);
	else 
		return (0);
}



void	g_ingame_on_events(t_gui *self, SDL_Event *event, t_doom *doom)
{
	const SDL_Scancode	key = event->key.keysym.scancode;

	if (doom->main_context.type == CTX_NORMAL)
	{
		if (event->type == SDL_KEYDOWN && (key == SDL_SCANCODE_SEMICOLON))
			doom->mouse_focus = !doom->mouse_focus;

		if (event->type == SDL_MOUSEBUTTONDOWN)
		{
			t_itemstack	*is = &doom->player.item[doom->player.selected_slot];
			if (is->of && is->of->on_use)
				is->of->on_use(doom, is);
		}

		if (event->type == SDL_KEYDOWN && (key == SDL_SCANCODE_P))
		{
			
			t_ray ray = create_shoot_ray(doom->player, (t_vec3){0, 0, 1});
			t_collision hit = ray_hit_world(doom, doom->renderables, ray);
			if (hit.collide)
			{
				t_renderable *r = hit.renderable;
				if (r && r->of.type != RENDERABLE_ENTITY)
				{
					printf("CALL\n");
					t_renderable enemy;
					
					create_enemy_renderable(doom, &enemy);
					enemy.of.data.entity->position = hit.point;
					enemy.of.data.entity->position.y += enemy.of.data.entity->radius.y;
					append_renderables_array(&doom->renderables, enemy);
				}
			}
		}

		if (event->type == SDL_KEYDOWN && key == SDL_SCANCODE_G)
		{
			t_renderable grenada;
			grenada = *doom->res_manager.ressources->values[7]->data.model;
			create_grenada(&grenada, doom);
			grenada.of.data.entity->position = doom->player.entity.position;
			t_vec3 forward = vec3_rotate((t_vec3){ 0, 0, 1 }, (t_vec3){-doom->player.entity.rotation.x, doom->player.entity.rotation.y, 0});
			printf("%f %f %f\n", forward.x, forward.y, forward.z);
			forward.y *= 20;
			forward.x *= 14;
			forward.z *= 14;
			grenada.of.data.entity->velocity = forward;
			append_renderables_array(&doom->renderables, grenada);
		}

		
		if (event->type == SDL_KEYDOWN && (key == SDL_SCANCODE_UP || key == SDL_SCANCODE_DOWN))
		{
			doom->player.selected_slot += (key == SDL_SCANCODE_DOWN ? 1 : -1);
			if (doom->player.selected_slot < 0)
				doom->player.selected_slot = 7;
			if (doom->player.selected_slot > 7)
				doom->player.selected_slot = 0;
		}
	}
	else if (doom->main_context.type == CTX_EDITOR)
	{
		if (event->type == SDL_KEYDOWN && (key == SDL_SCANCODE_TAB))
			doom->mouse_focus = !doom->mouse_focus;
		if (!doom->mouse_focus && is_settings_open(&doom->editor))
		{
			g_editor_settings_on_event(&doom->guis[GUI_EDITOR_SETTINGS], event, doom);
			return ;
		}
		
		if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT)
		{
			t_ray ray = create_shoot_ray(doom->player, (t_vec3){0, 0, 1});
			t_collision hit = ray_hit_world(doom, doom->renderables, ray);
			
			if (hit.collide)
			{
				if (doom->editor.current_object != -1)
					doom->editor.objects->values[doom->editor.current_object].r->show_hitbox = FALSE;
				select_floor_ceil(&doom->editor, -1, FALSE);
				select_room(&doom->editor, -1);
				if (hit.renderable->of.type == RENDERABLE_MAP)
				{
					t_face face = hit.renderable->faces->values[hit.who.data.triangle.face];
					t_room *room = &doom->editor.rooms->values[face.room_index];
					
					if (face.wall_index == -1)
					{
						doom->editor.wall_section = -1;
						doom->editor.current_seg.x = -1;
					}
					else
					{
						doom->editor.current_seg.x = room->walls->values[face.wall_index].indice;
						doom->editor.current_seg.y = room->walls->values[(face.wall_index + 1) % room->walls->len].indice;
						doom->editor.wall_section = face.wall_section;
						printf("%d section\n", face.wall_section);
					}
					select_room(&doom->editor, face.room_index);
					const Uint8		*s = SDL_GetKeyboardState(NULL);
					if (s[SDL_SCANCODE_LCTRL] && hit.who.data.triangle.face >= room->floor_start && hit.who.data.triangle.face < room->walls_start)
					{
						// printf("CALL\n");
						select_floor_ceil(&doom->editor, face.room_index, hit.who.data.triangle.face < room->ceilling_start);
					}
				}
				else if (hit.renderable->object_index != -1)
				{
					doom->editor.current_object = hit.renderable->object_index;
					doom->editor.objects->values[doom->editor.current_object].r->show_hitbox = TRUE;
					editor_settings_update(&doom->editor);
				}

			}
		}
		else if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_RIGHT)
			unselect_all(doom);

		if (event->type == SDL_KEYDOWN && doom->editor.selected_floor_ceil != -1 && doom->editor.current_room != -1)
		{
			t_room	*room = &doom->editor.rooms->values[doom->editor.current_room];
			// if (!room->r)
			// 	return ;
			if (key == SDL_SCANCODE_KP_4 || key == SDL_SCANCODE_KP_6 || key == SDL_SCANCODE_KP_8 || key == SDL_SCANCODE_KP_5)
			{
				t_vec3	rot = doom->editor.selected_floor_ceil == 0 ? room->floor_rot : room->ceil_rot;

				if (key == SDL_SCANCODE_KP_4 || key == SDL_SCANCODE_KP_6)
					rot.x += 0.01 * (key == SDL_SCANCODE_KP_4 ? 1 : -1);
				if (key == SDL_SCANCODE_KP_8 || key == SDL_SCANCODE_KP_5)
					rot.z += 0.01 * (key == SDL_SCANCODE_KP_8 ? 1 : -1);
				rot.x = clamp(-M_PI / 4, M_PI / 4, rot.x);
				rot.z = clamp(-M_PI / 4, M_PI / 4, rot.z);
				t_vec2 first_point = doom->editor.points->vertices[room->walls->values[0].indice];
				t_mat4 m_rot = ft_mat4_mul(ft_mat4_translation(ft_vec3_inv((t_vec3){ first_point.x, 0, first_point.y })), ft_mat4_rotation(rot));
				int i = -1;
				while (++i < room->walls->len)
				{
					t_wall	*wall = &room->walls->values[i];
					int point_index = i * 2 + doom->editor.selected_floor_ceil;
					t_vec2 v = doom->editor.points->vertices[wall->indice];
					t_vec3 point = ft_mat4_mulv(m_rot, editor_to_world((t_vec3){ v.x, 0, v.y }));
					// doom->editor.map_renderable.vertices->vertices[room->room_vertices_start + point_index].y = point.y;
					if (doom->editor.selected_floor_ceil == 0)
						wall->floor_height = point.y;
					else
						wall->ceiling_height = point.y;
				}
				if (doom->editor.selected_floor_ceil == 0)
					room->floor_rot = rot;
				else
					room->ceil_rot = rot;
				create_map(&doom->renderables->values[doom->editor.map_renderable], &doom->editor);
				select_floor_ceil(&doom->editor, doom->editor.current_room, doom->editor.selected_floor_ceil == 0);
			}
			else if (key == SDL_SCANCODE_KP_PLUS || key == SDL_SCANCODE_KP_MINUS)
			{
				int		i;

				i = -1;
				while (++i < room->walls->len)
				{
					t_wall	*wall = &room->walls->values[i];
					float add = 0.1 * (key == SDL_SCANCODE_KP_PLUS ? -1 : 1);					
					// room->r->vertices->vertices[point_index].y = point.y;
					if (doom->editor.selected_floor_ceil == 0)
						wall->floor_height += add;
					else
						wall->ceiling_height += add;
				}
				create_map(&doom->renderables->values[doom->editor.map_renderable], &doom->editor);
				select_floor_ceil(&doom->editor, doom->editor.current_room, doom->editor.selected_floor_ceil == 0);
			}
		}
	}
	components_events(doom, doom->guis, event, GUI_EDITOR_SETTINGS);
}

void	update_controls(t_doom *doom)
{
	const double	ms = doom->stats.delta * 2.;
	const Uint8		*s = SDL_GetKeyboardState(NULL);

	float dt = 1.0 / 60.;
	float move_speed;
	if (!doom->mouse_focus && is_settings_open(&doom->editor))
		return ;
	//long start = getMicrotime();
	entity_update(doom, &doom->player.entity, doom->stats.delta);
	//printf("delay %luus\n", getMicrotime() - start);
	if (doom->main_context.type == CTX_EDITOR)
		move_speed = 10;
	else
		move_speed = !doom->player.entity.grounded ? 1.2 : 10;
	if (s[SDL_SCANCODE_W] || s[SDL_SCANCODE_S])
	{
		doom->player.entity.velocity.x += sinf(doom->player.entity.rotation.y) * (s[SDL_SCANCODE_W] ? 1 : -1) * move_speed;
		doom->player.entity.velocity.z += cosf(doom->player.entity.rotation.y) * (s[SDL_SCANCODE_W] ? 1 : -1) * move_speed;
	}
	if (s[SDL_SCANCODE_A] || s[SDL_SCANCODE_D])
	{
		doom->player.entity.velocity.x += -cosf(doom->player.entity.rotation.y) * (s[SDL_SCANCODE_D] ? 1 : -1) * move_speed;
		doom->player.entity.velocity.z += sinf(doom->player.entity.rotation.y) * (s[SDL_SCANCODE_D] ? 1 : -1) * move_speed;
	}
	if (doom->main_context.type == CTX_NORMAL)
	{
		if (s[SDL_SCANCODE_SPACE] && !doom->player.entity.jump && doom->player.entity.grounded)
		{ 
			doom->player.entity.jump = TRUE;
			//doom->player.entity.velocity.y += 50;
		}
		if (s[SDL_SCANCODE_LSHIFT])
		{ 
			doom->player.entity.grounded = FALSE;
			doom->player.entity.velocity.y -= 50;
		}
	}

	if (doom->main_context.type == CTX_EDITOR)
	{
		if (s[SDL_SCANCODE_SPACE] )
		{ 
			doom->player.entity.velocity.y += 8;
		}
		if (s[SDL_SCANCODE_LSHIFT])
		{ 
			doom->player.entity.velocity.y -= 8;
		}
	}
	if (s[SDL_SCANCODE_J] || s[SDL_SCANCODE_L])
		doom->player.entity.rotation.y += (s[SDL_SCANCODE_J] ? 1 : -1) * ms;
	if (s[SDL_SCANCODE_I] || s[SDL_SCANCODE_K])
		doom->player.entity.rotation.x += (s[SDL_SCANCODE_I] ? 1 : -1) * ms;

	if (doom->mouse_focus)
	{
		int m_x, m_y;
		SDL_GetRelativeMouseState(&m_x, &m_y);
		if (m_x != 0)
			doom->player.entity.rotation.y -= m_x * ms * 0.01;
		if (m_y != 0)
		{
			float rot = m_y * ms * 0.01;
			if (doom->player.entity.rotation.x - rot < M_PI_2 && doom->player.entity.rotation.x - rot > -M_PI_2 )
				doom->player.entity.rotation.x -= rot;
		}
	}
	update_player_camera(&doom->player);
}

void	g_ingame_render(t_gui *self, t_doom *doom)
{
	static float ticks = 0;
	
	
	update_controls(doom);
	doom->main_context.image = &doom->screen;
	for (int i = 0; i < S_WIDTH * S_HEIGHT; i++)
		doom->main_context.buffer[i] = 0;
	if (doom->skybox_index != -1)
	{
		doom->renderables->values[doom->skybox_index].position = doom->main_context.camera->pos;
		doom->renderables->values[doom->skybox_index].dirty = TRUE;
	}
	//printf("START FAME ------------\n");
	if (doom->main_context.type == CTX_EDITOR)
	{
		t_renderable *sphere = &doom->sphere_primitive;
		sphere->position = editor_to_world(doom->editor.player_pos);
		sphere->position.y += doom->player.entity.radius.y;
		sphere->scale = doom->player.entity.radius;
		sphere->wireframe = TRUE;
		sphere->wireframe_color = 0xFF5C5C5C;
		sphere->dirty = TRUE;
	//	sphere->double_faced = FALSE;
		render_renderable(&doom->main_context, sphere);
	}
	doom->closer_boss = NULL;
	for (int i = 0; i < doom->renderables->len; i++)
	{
		t_renderable	*r = doom->renderables->values + i;
		if (r->of.type == RENDERABLE_ENTITY)
		{
			entity_update(doom, r->of.data.entity, doom->stats.delta);
			if (r->of.data.entity->type == ENTITY_BOSS && ft_vec3_len(ft_vec3_sub(doom->player.entity.position, r->of.data.entity->position)) <= 50)
				doom->closer_boss = r->of.data.entity;
		}
		if (r->has_hitbox)
			update_hitbox(r);
		render_renderable(&doom->main_context, r);
		if (r->has_hitbox && r->show_hitbox && r->hitbox.type == COLLIDE_ELLIPSOID)
		{
			t_renderable *sphere = &doom->sphere_primitive;
			t_collide_ellipsoid	*ellipsoid = &r->hitbox.data.ellipsoid;
			sphere->position = ellipsoid->origin;
			sphere->scale = ellipsoid->radius;
			sphere->wireframe = TRUE;
			sphere->wireframe_color = 0xFFFF0000;
			sphere->dirty = TRUE;
			render_renderable(&doom->main_context, sphere);
		}
	}

	for (int i = 0; i < doom->lights->len; i++)
	{
		t_renderable	*sphere = &doom->sphere_primitive;
		t_light			*light = &doom->lights->values[i];
		sphere->position = light->position;
		sphere->scale = (t_vec3){ 0.2, 0.2, 0.2 };
		sphere->wireframe = TRUE;
		sphere->wireframe_color = 0xFFFF0000;
		sphere->dirty = TRUE;
		//sphere->double_faced = FALSE;
		render_renderable(&doom->main_context, sphere);
	}

	doom->main_context.image->pixels[(doom->main_context.image->height / 2) * doom->main_context.image->width + doom->main_context.image->width / 2 ] = 0xFF00FF00;
	draw_circle(doom->main_context.image, (t_pixel){ S_WIDTH_2, S_HEIGHT_2, 0xFF00FF00 }, 10);
	doom->main_context.image = &doom->screen_transparency;


	doom->main_context.image = &doom->screen;
	if (doom->main_context.type == CTX_NORMAL)
	{
		int i = -1;
		while (++i < PLAYER_INV_SIZE)
		{
			t_itemstack		*is = &doom->player.item[i];
			if (i == doom->player.selected_slot)
				fill_rect(doom->main_context.image, (SDL_Rect){ 8, 48 + i * 60, 54, 54 }, 0xFFFFFF00);
			fill_rect(doom->main_context.image, (SDL_Rect){ 10, 50 + i * 60, 50, 50 }, 0xFFFF0000);
			if (is->of)
			{
				apply_image_blended(doom->main_context.image, is->of->image->data.texture, is->of->bounds, (SDL_Rect){ 10, 50 + i * 60, 50, 50 });
				if (is->amount <= 1)
					continue;
				const SDL_Color	color = {255, 255, 255, 0};
				SDL_Surface		*text;

				text = TTF_RenderText_Blended(doom->fonts.helvetica,
					ft_int_to_str(is->amount).str, color);
				apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h},
					(SDL_Rect){ 40, 50 + i * 60, 20, 20 });
				SDL_FreeSurface(text);
			}
		}

		ticks += doom->stats.delta * 30.;

		t_itemstack	*is = &doom->player.item[doom->player.selected_slot]; 

		if (is->of && is->of->type == ITEM_WEAPON)
		{
			t_weapon	*weapon = &is->of->data.weapon;
			if (ticks > 1 && weapon->fireing)
			{
				ticks = 0;
				weapon->current_step++;
				if (weapon->current_step == weapon->steps_count)
				{
					weapon->current_step = (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT) ? 0 : weapon->idle_step);
					weapon->fireing = FALSE;
				}
				set_current_animation_step(weapon, weapon->animation_seq[weapon->current_step]);
			}
			apply_image_blended(doom->main_context.image, weapon->animation->data.texture, weapon->curr_image, (SDL_Rect){ S_WIDTH_2 - 80 / 2, S_HEIGHT - 300, 300, 300 });
		}

		((t_progress *)self->components->values[0])->value = doom->player.entity.life * (1 / doom->player.entity.max_life) * 100;
		if (doom->player.entity.life <= 0)
		{
			set_gui(doom, GUI_GAMEOVER);
			return ;
		}
	}

	self->components->values[0]->visible = doom->main_context.type == CTX_NORMAL;
	self->components->values[1]->visible = doom->main_context.type == CTX_NORMAL && !!doom->closer_boss;
	if (doom->closer_boss)
	{
		((t_progress *)self->components->values[1])->value = doom->closer_boss->life * (1 / doom->closer_boss->max_life) * 100;
	}

	render_components(doom, self);
	doom->guis[GUI_EDITOR_SETTINGS].render(&doom->guis[GUI_EDITOR_SETTINGS], doom);
}
