/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events_ctx_editor.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/05 15:28:11 by lloncham          #+#    #+#             */
/*   Updated: 2020/02/05 16:20:27 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "gui.h"
#include "octree.h"
#include <math.h>
#include "render.h"
#include "editor.h"
#include "threads.h"
#include "door.h"

void        event_renderable_map(t_collision hit, t_doom *doom)
{
    t_face          face;
    t_room          *room;
    const Uint8		*s = SDL_GetKeyboardState(NULL);

    face = hit.renderable->faces->values[hit.who.data.triangle.face];
    room = &doom->editor.rooms->values[face.room_index];
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
    }
    select_room(&doom->editor, face.room_index);
    if (s[SDL_SCANCODE_LCTRL] && hit.who.data.triangle.face >= room->floor_start && hit.who.data.triangle.face < room->walls_start)
        select_floor_ceil(&doom->editor, face.room_index, hit.who.data.triangle.face < room->ceilling_start);
}

void        event_renderable_door(t_collision hit, t_doom *doom)
{
    t_door *door; 
    t_room *room;
    
    door = hit.renderable->of.data.door;
    room = &doom->editor.rooms->values[door->indexes[0]];
    doom->editor.current_seg.x = room->walls->values[door->indexes[1]].indice;
    doom->editor.current_seg.y = room->walls->values[(door->indexes[1] + 1) % room->walls->len].indice;
    doom->editor.wall_section = door->indexes[2];
    select_room(&doom->editor, door->indexes[0]);
}

void        event_button_left(t_doom *doom)
{
    t_ray ray; 
    t_collision hit;

    ray = create_shoot_ray(doom->player, (t_vec3){0, 0, 1});
    hit = ray_hit_world(doom, doom->renderables, ray);
    if (hit.collide)
    {
        if (doom->editor.current_object != -1)
            doom->editor.objects->values[doom->editor.current_object].r
                ->show_hitbox = FALSE;
        select_floor_ceil(&doom->editor, -1, FALSE);
        select_room(&doom->editor, -1);
        if (hit.renderable->of.type == RENDERABLE_MAP)
            event_renderable_map(hit, doom);
        if (hit.renderable->of.type == RENDERABLE_DOOR)
            event_renderable_door(hit, doom);
        else if (renderables_indexof(doom->renderables, hit.renderable) == doom->skybox_index)
            editor_settings_update(&doom->editor);
        else if (hit.renderable->object_index != -1)
        {
            doom->editor.current_object = hit.renderable->object_index;
            doom->editor.objects->values[doom->editor.current_object].r->show_hitbox = TRUE;
            editor_settings_update(&doom->editor);
        }
    }
}