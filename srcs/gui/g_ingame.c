/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_ingame.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/24 11:22:28 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/05 16:33:08 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "doom.h"
#include <limits.h>
#include "gui.h"
#include "octree.h"
#include <math.h>
#include "render.h"
#include "editor.h"
#include "threads.h"
#include "door.h"

static t_bool			action_performed(t_component *cmp, t_doom *doom)
{
	if (cmp == doom->guis[doom->current_gui].components->values[2])
	{
		set_gui(doom, GUI_EDITOR);
		return (FALSE);
	}
	return (TRUE);
}

void	g_ingame_on_enter(t_gui *self, t_doom *doom)
{
	enter_gui(doom, doom->guis, GUI_EDITOR_SETTINGS);
	doom->screen.secure = FALSE;
	doom->mouse_focus = TRUE;
	append_components_array(&self->components, create_progress((SDL_Rect)
		{ 5, 5, 200, 30 }));
	((t_progress *)self->components->values[0])->value = 50;
	((t_progress *)self->components->values[0])->bg_color = 0xFFFF0000;
	((t_progress *)self->components->values[0])->fg_color = 0xFF00FF00;
	append_components_array(&self->components, create_progress((SDL_Rect)
		{ S_WIDTH_2 - 200, 5, 400, 20 }));
	((t_progress *)self->components->values[1])->value = 50;
	((t_progress *)self->components->values[1])->bg_color = 0xFFFF0000;
	((t_progress *)self->components->values[1])->fg_color = 0xFF00FF00;
	if (doom->main_context.type == CTX_EDITOR)
	{
		append_components_array(&self->components, create_button((SDL_Rect)
		{ 10, 10 , 200, 50 }, NULL, "BACK"));
		self->components->values[2]->perform_action = action_performed;
	}

}

void	g_ingame_on_leave(t_gui *self, t_doom *doom)
{
	(void)self;
	leave_gui(doom, doom->guis, GUI_EDITOR_SETTINGS);
	doom->mouse_focus = FALSE;
}

void	unselect_all(t_doom *doom)
{
	if (doom->editor.current_object != -1)
		doom->editor.objects->values[doom->editor.current_object].r
			->show_hitbox = FALSE;
	doom->editor.object_transform_mode = OT_MODE_TRANSLATION;
	doom->editor.current_object = -1;
	doom->editor.wall_section = -1;
	select_room(&doom->editor, -1);
}

void	transform_object(t_doom *doom, t_object *object, t_vec3 add)
{
	object->pos = ft_vec3_add(object->pos, add);
	if (object->r)
	{
		object->r->position = editor_to_world(object->pos);
		if (object->type == OBJECT_ENTITY)
		{
			object->r->of.data.entity->position = editor_to_world(object->pos);
			object->r->of.data.entity->position.y += add.y;
			object->r->of.data.entity->position.y += object->r->of.data.entity
				->radius.y;
		}
		else if (object->type == OBJECT_ITEMSTACK)
			object->r->position.y += object->r->scale.y * 0.5;
		else if (object->type == OBJECT_MODEL)
			object->r->position.y += object->r->scale.y;
		object->r->position.y += add.y;
		if (object->type == OBJECT_LIGHT)
		{
			t_light *light = &doom->lights->values[object->of.light_index];
			light->position = object->r->position;
		}
		object->r->dirty = TRUE;
	}
}

void	g_ingame_on_events(t_gui *self, SDL_Event *event, t_doom *doom)
{
	const SDL_Scancode	key = event->key.keysym.scancode;

	if (event->type == SDL_KEYDOWN && (key == SDL_SCANCODE_TAB))
			doom->mouse_focus = !doom->mouse_focus;
	if (doom->main_context.type == CTX_EDITOR)
	{
		if (!doom->mouse_focus && is_settings_open(&doom->editor))
		{
			g_editor_settings_on_event(self, event, doom);
			return ;
		}
		if (event->type == SDL_MOUSEBUTTONDOWN
			&& event->button.button == SDL_BUTTON_LEFT)
			event_button_left(doom);
		else if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_RIGHT)
			unselect_all(doom);
		if (event->type == SDL_KEYDOWN && doom->editor.selected_floor_ceil != -1 && doom->editor.current_room != -1)
		{
			t_room	*room;
			room = &doom->editor.rooms->values[doom->editor.current_room];
			if (key == SDL_SCANCODE_KP_4 || key == SDL_SCANCODE_KP_6 || key == SDL_SCANCODE_KP_8 || key == SDL_SCANCODE_KP_5)
			{
				t_vec3	rot;
				rot = doom->editor.selected_floor_ceil == 0 ? room->floor_rot : room->ceil_rot;

				if (key == SDL_SCANCODE_KP_4 || key == SDL_SCANCODE_KP_6)
					rot.x += 0.01 * (key == SDL_SCANCODE_KP_4 ? 1 : -1);
				if (key == SDL_SCANCODE_KP_8 || key == SDL_SCANCODE_KP_5)
					rot.z += 0.01 * (key == SDL_SCANCODE_KP_8 ? 1 : -1);
				rot.x = clamp(-M_PI / 4, M_PI / 4, rot.x);
				rot.z = clamp(-M_PI / 4, M_PI / 4, rot.z);
				t_vec2 first_point;
				first_point = doom->editor.points->vertices[room->walls->values[0].indice];
				t_mat4 m_rot;
				m_rot = ft_mat4_mul(ft_mat4_translation(ft_vec3_inv((t_vec3){ first_point.x, 0, first_point.y })), ft_mat4_rotation(rot));
				int i;
				i = -1;
				while (++i < room->walls->len)
				{
					t_wall	*wall;
					wall = &room->walls->values[i];
					t_vec2 v;
					v = doom->editor.points->vertices[wall->indice];
					t_vec3 point;
					point = ft_mat4_mulv(m_rot, editor_to_world((t_vec3){ v.x, 0, v.y }));
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
				add_map(&doom->renderables->values[doom->editor.map_renderable], &doom->editor);
				select_floor_ceil(&doom->editor, doom->editor.current_room, doom->editor.selected_floor_ceil == 0);
			}
			else if (key == SDL_SCANCODE_KP_PLUS || key == SDL_SCANCODE_KP_MINUS)
			{
				int		i;

				i = -1;
				while (++i < room->walls->len)
				{
					t_wall	*wall;
					wall = &room->walls->values[i];
					float add;
					add = 0.1 * (key == SDL_SCANCODE_KP_PLUS ? -1 : 1);					
					// room->r->vertices->vertices[point_index].y = point.y;
					if (doom->editor.selected_floor_ceil == 0)
						wall->floor_height += add;
					else
						wall->ceiling_height += add;
				}
				add_map(&doom->renderables->values[doom->editor.map_renderable], &doom->editor);
				select_floor_ceil(&doom->editor, doom->editor.current_room, doom->editor.selected_floor_ceil == 0);
			}
		}

		if (event->type == SDL_KEYDOWN && doom->editor.current_object != -1)
		{
			if (key == SDL_SCANCODE_PAGEUP)
			{
				doom->editor.object_transform_mode++;
				if (doom->editor.object_transform_mode == 3)
					doom->editor.object_transform_mode = 0;
			}

			if (key == SDL_SCANCODE_PAGEDOWN)
			{
				doom->editor.object_transform_mode--;
				if ((int)doom->editor.object_transform_mode == -1)
					doom->editor.object_transform_mode = 2;
			}

			t_vec3 add;
			add = (t_vec3){ 0, 0, 0 };
			if (key == SDL_SCANCODE_KP_PLUS || key == SDL_SCANCODE_KP_MINUS)
				add.y = 0.1 * (key == SDL_SCANCODE_KP_PLUS ? 1 : -1);
			if (key == SDL_SCANCODE_LEFT || key == SDL_SCANCODE_RIGHT)
				add.x = 0.1 * (key == SDL_SCANCODE_RIGHT ? 1 : -1);
			if (key == SDL_SCANCODE_UP || key == SDL_SCANCODE_DOWN)
				add.z = 0.1 * (key == SDL_SCANCODE_UP ? 1 : -1);
			if (doom->editor.object_transform_mode == OT_MODE_TRANSLATION)
			{
				t_object *object;
				object = &doom->editor.objects->values[doom->editor.current_object];
				if (add.x != 0 || add.y != 0 || add.z != 0)
					transform_object(doom, object, add);
			}
			else if (doom->editor.object_transform_mode == OT_MODE_ROTATION)
			{
				t_object *object;
				object = &doom->editor.objects->values[doom->editor.current_object];
				object->rotation = ft_vec3_add(object->rotation, add);
				if (object->r)
				{
					object->r->rotation = object->rotation;
					object->r->dirty = TRUE;
				}
				// if (object->type == OBJECT_LIGHT)
				// {
				// 	t_light *light = &doom->lights->values[object->of.light_index];
				// 	light->dir = ft_vec3_norm(object->rotation);
				// }
			}
			else if (doom->editor.object_transform_mode == OT_MODE_SCALE)
			{
				t_object *object;
				object = &doom->editor.objects->values[doom->editor.current_object];
				object->scale = ft_vec3_add(object->scale, ft_vec3_mul_s(add, 0.1));
				if (object->r)
				{
					object->r->scale = object->scale;
					object->r->dirty = TRUE;
				}
			}
		}
	}
	player_inventory_event(doom, event);
	components_events(doom, doom->guis, event, GUI_EDITOR_SETTINGS);
}

void	draw_object_transform_type(t_editor *editor, t_gui *self)
{
	const char	types[3][12] = {"TRANSLATION\0", "ROTATION\0", "SCALING\0"};
	
	(void)self;
	if (editor->current_object == -1
		|| editor->doom->main_context.type != CTX_EDITOR)
		return ;
	SDL_Surface		*text;
	text = TTF_RenderText_Blended(editor->doom->fonts.helvetica,
		types[(int)editor->object_transform_mode], (SDL_Color){255, 255, 255, 0});
	apply_surface_blended(editor->doom->main_context.image, text, (SDL_Rect){0, 0, text->w, text->h},
		(SDL_Rect){ S_WIDTH - text->w - 15, 15, text->w, 20 });
	SDL_FreeSurface(text);
}

float t = 0;

void	g_ingame_render(t_gui *self, t_doom *doom)
{
	t += 0.05;

	//doom->lights->values[0].dir.z = 1;
	doom->lights->values[1].position.y = 1;
	doom->lights->values[1].position.z = 25 + (50) * ((sin(t) + 1) / 2);


	// doom->lights->values[1].
	update_controls(doom);
	doom->main_context.image = &doom->screen;
	for (int i = 0; i < S_WIDTH * S_HEIGHT; i++)
		doom->main_context.buffer[i] = 0;
	
	if (doom->skybox_index != -1)
	{
		doom->renderables->values[doom->skybox_index].visible = doom->skybox_enabled;
		doom->renderables->values[doom->skybox_index].position = doom->main_context.camera->pos;
		doom->renderables->values[doom->skybox_index].dirty = TRUE;
	}

	threads_clear(&doom->threads);
	
	//printf("START FAME ------------\n");
	if (doom->main_context.type == CTX_EDITOR)
	{
		t_renderable *sphere = &doom->sphere_primitive;
		sphere->position = editor_to_world(doom->player.spawn_data.position);
		sphere->position.y += doom->player.entity.radius.y;
		sphere->scale = doom->player.entity.radius;
		sphere->wireframe = TRUE;
		sphere->wireframe_color = 0xFF5C5C5C;
		sphere->dirty = TRUE;
	//	sphere->double_faced = FALSE;
		render_renderable(&doom->main_context, sphere);
	}

	doom->lights->values[0].position = doom->player.camera.pos;
	doom->lights->values[0].dir = doom->player.camera.forward;
	doom->closer_boss = NULL;
	for (int i = 0; i < doom->renderables->len; i++)
	{
		t_renderable	*r;
		r = doom->renderables->values + i;
		if (r->of.type == RENDERABLE_ENTITY)
		{
			r->of.data.entity->r = r;
			if (!entity_update(doom, r->of.data.entity, doom->stats.delta))
				continue;
			if (r->of.data.entity->type == ENTITY_BOSS && ft_vec3_len(ft_vec3_sub(doom->player.entity.position, r->of.data.entity->position)) <= 50)
				doom->closer_boss = r->of.data.entity;
		}
		if (r->has_hitbox)
			update_hitbox(r);
		if (!render_renderable(&doom->main_context, r))
		{
			i--;
			continue;
		}
		if (r->has_hitbox && r->show_hitbox && r->hitbox.type == COLLIDE_ELLIPSOID)
		{
			t_renderable *sphere;
			t_collide_ellipsoid	*ellipsoid;
			sphere = &doom->sphere_primitive;
			ellipsoid = &r->hitbox.data.ellipsoid;
			sphere->position = ellipsoid->origin;
			sphere->scale = ellipsoid->radius;
			sphere->wireframe = TRUE;
			sphere->wireframe_color = 0xFFFF0000;
			sphere->dirty = TRUE;
			render_renderable(&doom->main_context, sphere);
		}
	}
	doom->lights->values[0].position = doom->player.camera.pos;
	int i;
	i = -1;
	while (++i < doom->lights->len)
	// for (int i = 0; i < doom->lights->len; i++)
	{
		t_light			*light = &doom->lights->values[i];
		if (!light->model_visible && doom->main_context.type == CTX_EDITOR)
		{
			t_renderable	*sphere = &doom->sphere_primitive;
			sphere->position = light->position;
			sphere->scale = (t_vec3){ 0.2, 0.2, 0.2 };
			sphere->wireframe = TRUE;
			sphere->wireframe_color = 0xFFFF0000;
			sphere->dirty = TRUE;
			//sphere->double_faced = FALSE;
			render_renderable(&doom->main_context, sphere);
		}
		if (i != 0 && doom->main_context.type == CTX_EDITOR && light->type == LIGHT_SPOT)
		{
			t_renderable	*sphere = &doom->sphere_primitive;
			sphere->position = ft_vec3_mul_s(ft_vec3_add(light->position, light->dir), 1);
			sphere->scale = (t_vec3){ 0.2, 0.2, 0.2 };
			sphere->wireframe = TRUE;
			sphere->wireframe_color = 0xFFFFFF00;
			sphere->dirty = TRUE;
			//sphere->double_faced = FALSE;
			render_renderable(&doom->main_context, sphere);
		}
	}
	threads_launch(&doom->threads);
	threads_wait(&doom->threads);

	doom->main_context.image->pixels[(doom->main_context.image->height / 2) * doom->main_context.image->width + doom->main_context.image->width / 2 ] = 0xFF00FF00;
	draw_circle(doom->main_context.image, (t_pixel){ S_WIDTH_2, S_HEIGHT_2, 0xFF00FF00 }, 10);
	doom->main_context.image = &doom->screen_transparency;


	doom->main_context.image = &doom->screen;
	if (!draw_player_inventory(doom, self))
		return ;
	draw_object_transform_type(&doom->editor, self);

	self->components->values[0]->visible = doom->main_context.type == CTX_NORMAL;
	self->components->values[1]->visible = doom->main_context.type == CTX_NORMAL && !!doom->closer_boss;
	if (doom->closer_boss)
	{
		((t_progress *)self->components->values[1])->value = doom->closer_boss->life * (1 / doom->closer_boss->max_life) * 100;
	}

	render_components(doom, self);
	if (doom->main_context.type == CTX_EDITOR)
		doom->guis[GUI_EDITOR_SETTINGS].render(&doom->guis[GUI_EDITOR_SETTINGS], doom);
}
