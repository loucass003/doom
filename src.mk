SRC+=arrays/components.c
SRC+=arrays/ints.c
SRC+=arrays/lights.c
SRC+=arrays/objects.c
SRC+=arrays/objs.c
SRC+=arrays/ptrs.c
SRC+=arrays/render_datas.c
SRC+=arrays/renderables.c
SRC+=arrays/ressources.c
SRC+=arrays/rooms.c
SRC+=arrays/select_items.c
SRC+=arrays/vertices2d.c
SRC+=arrays/vertices3d.c
SRC+=arrays/vertices4d.c
SRC+=arrays/wall_sections.c
SRC+=arrays/walls.c
SRC+=audio/audio.c
SRC+=audio/sounds.c
SRC+=audio/wav_parser.c
SRC+=audio/wav_utils.c
SRC+=colisions/aabb.c
SRC+=colisions/aabb_triangle.c
SRC+=colisions/aabb_triangle_utils.c
SRC+=colisions/compute_collidables.c
SRC+=colisions/obb.c
SRC+=colisions/physics/maths.c
SRC+=colisions/physics/physics.c
SRC+=colisions/physics/utils.c
SRC+=colisions/triangle.c
SRC+=components/apply.c
SRC+=components/c_button.c
SRC+=components/c_checkbox.c
SRC+=components/c_progress.c
SRC+=components/c_select.c
SRC+=components/c_textfield.c
SRC+=components/component.c
SRC+=editor/editor_create_map.c
SRC+=editor/editor_create_walls.c
SRC+=editor/editor_delete.c
SRC+=editor/editor_free.c
SRC+=editor/editor_free_wall.c
SRC+=editor/editor_grid.c
SRC+=editor/editor_hide_wall.c
SRC+=editor/editor_intersect.c
SRC+=editor/editor_object.c
SRC+=editor/editor_player.c
SRC+=editor/editor_point.c
SRC+=editor/editor_point_room.c
SRC+=editor/editor_point_seg.c
SRC+=editor/editor_room.c
SRC+=editor/editor_room_mod.c
SRC+=editor/editor_rooms.c
SRC+=editor/editor_set_render.c
SRC+=editor/editor_setmap.c
SRC+=editor/editor_setmap_floor.c
SRC+=editor/editor_setmap_render.c
SRC+=editor/editor_setmap_wall.c
SRC+=editor/editor_settings.c
SRC+=editor/editor_tool_gg.c
SRC+=editor/editor_tool_object.c
SRC+=editor/editor_tool_player.c
SRC+=editor/editor_tool_point.c
SRC+=editor/editor_tool_room.c
SRC+=editor/editor_tool_select.c
SRC+=editor/editor_wall.c
SRC+=editor/editor_wall_sections.c
SRC+=editor/editor_wall_update.c
SRC+=editor/editor_world.c
SRC+=entity/boss.c
SRC+=entity/boss_hit_dist.c
SRC+=entity/boss_tools.c
SRC+=entity/enemy.c
SRC+=entity/enemy_tools.c
SRC+=entity/entity/check_collision.c
SRC+=entity/entity/colide_with.c
SRC+=entity/entity/collisions.c
SRC+=entity/entity/damage.c
SRC+=entity/entity/entity_hit_something.c
SRC+=entity/entity/entity_update.c
SRC+=entity/entity/update_entity_type.c
SRC+=entity/entity/utils.c
SRC+=entity/entity.c
SRC+=entity/grenada.c
SRC+=entity/rocket.c
SRC+=events.c
SRC+=file_utils.c
SRC+=frustum.c
SRC+=game.c
SRC+=game_utils.c
SRC+=gaps_filler/gaps_filler.c
SRC+=gaps_filler/range_wall.c
SRC+=gaps_filler/room_gaps.c
SRC+=gaps_filler/wall_sections.c
SRC+=gui/editor/editor_button.c
SRC+=gui/editor/editor_event.c
SRC+=gui/editor/tool_editor_button.c
SRC+=gui/editor_settings/g_es_global.c
SRC+=gui/editor_settings/g_es_object.c
SRC+=gui/editor_settings/g_es_player.c
SRC+=gui/editor_settings/g_es_room.c
SRC+=gui/editor_settings/g_es_room_components.c
SRC+=gui/editor_settings/g_es_wall.c
SRC+=gui/editor_settings/g_es_wall_actions.c
SRC+=gui/editor_settings/object/g_es_obj_entity.c
SRC+=gui/editor_settings/object/g_es_obj_itemstack.c
SRC+=gui/editor_settings/object/g_es_obj_itemstack_actions.c
SRC+=gui/editor_settings/object/g_es_obj_light.c
SRC+=gui/editor_settings/object/g_es_obj_model.c
SRC+=gui/editor_settings/object/g_es_obj_sprite.c
SRC+=gui/editor_settings/object/g_es_obj_transpo.c
SRC+=gui/editor_settings/object/g_es_object_events.c
SRC+=gui/g_editor.c
SRC+=gui/g_editor_settings.c
SRC+=gui/g_esc.c
SRC+=gui/g_gameover.c
SRC+=gui/g_ingame.c
SRC+=gui/g_leaderboard.c
SRC+=gui/g_level.c
SRC+=gui/g_mainmenu.c
SRC+=gui/g_message.c
SRC+=gui/g_message_render.c
SRC+=gui/g_ressources.c
SRC+=gui/g_ressources_components.c
SRC+=gui/gameover_tools.c
SRC+=gui/gui.c
SRC+=gui/ingame/editor_events.c
SRC+=gui/ingame/events_button_left.c
SRC+=gui/ingame/events_ceil_floor.c
SRC+=gui/ingame/events_objects.c
SRC+=gui/ingame/events_uvs.c
SRC+=gui/ingame/ingame_scene.c
SRC+=gui/ingame/render_ingame.c
SRC+=gui/ingame/render_utils.c
SRC+=gui/ressources/ressources_render.c
SRC+=gui/ressources/ressources_utils.c
SRC+=hitbox.c
SRC+=image/apply_surface.c
SRC+=image/circle.c
SRC+=image/get_surface.c
SRC+=image/image.c
SRC+=image/line.c
SRC+=image/rect.c
SRC+=image/tga_parser.c
SRC+=image/tga_read_formats.c
SRC+=image/tga_utils.c
SRC+=io/io_numbers.c
SRC+=io/io_reader.c
SRC+=io/io_reader_tool.c
SRC+=io/io_skip.c
SRC+=item/create_item.c
SRC+=item/create_item_weapon.c
SRC+=item/hit_item.c
SRC+=item/item_utils.c
SRC+=item/itemstack_renderable.c
SRC+=item/use_item.c
SRC+=light/light.c
SRC+=main.c
SRC+=math/line.c
SRC+=math/mat4.c
SRC+=math/mat4_inverse.c
SRC+=math/plane.c
SRC+=math/process_triangle.c
SRC+=math/triangle.c
SRC+=math/vec2.c
SRC+=math/vec3.c
SRC+=math/vec3_rotate.c
SRC+=math/vec4.c
SRC+=math/vec4_add_sub.c
SRC+=math/vec4_mul_div.c
SRC+=math/vec4_utils.c
SRC+=math/vectex.c
SRC+=multithreading/threads.c
SRC+=multithreading/threads_data.c
SRC+=multithreading/threads_stop.c
SRC+=multithreading/threads_utils.c
SRC+=obj/face.c
SRC+=obj/formatters.c
SRC+=obj/mtl.c
SRC+=obj/mtl_formaters.c
SRC+=obj/mtllist.c
SRC+=obj/obj.c
SRC+=obj/obj_utils.c
SRC+=octree/octree.c
SRC+=octree/octree_free.c
SRC+=octree/octree_insert.c
SRC+=octree/octree_intersect.c
SRC+=player/draw_inventory.c
SRC+=player/intersect.c
SRC+=player/inventory.c
SRC+=player/player.c
SRC+=player/set_player.c
SRC+=player/update_controls.c
SRC+=ray/ray.c
SRC+=ray/ray_utils.c
SRC+=render/camera.c
SRC+=render/face.c
SRC+=render/light.c
SRC+=render/prepare_triangle.c
SRC+=render/triangle.c
SRC+=render/triangle_pixel.c
SRC+=render/update_renderable.c
SRC+=render.c
SRC+=renderables/cube_renderable.c
SRC+=renderables/door/transform.c
SRC+=renderables/door/update.c
SRC+=renderables/door_renderable.c
SRC+=renderables/ellipsoid_renderable.c
SRC+=renderables/free_renderable.c
SRC+=renderables/post_processing.c
SRC+=renderables/renderable.c
SRC+=renderables/sprite_renderable.c
SRC+=renderables/transpo_renderable.c
SRC+=ressources/free_ressources_manager.c
SRC+=ressources/load_datapack.c
SRC+=ressources/load_map.c
SRC+=ressources/load_map_all.c
SRC+=ressources/load_map_read_itemstack.c
SRC+=ressources/load_map_read_object.c
SRC+=ressources/load_map_rooms.c
SRC+=ressources/load_ressource.c
SRC+=ressources/ressource_mapper.c
SRC+=ressources/ressource_model.c
SRC+=ressources/ressource_model_utils.c
SRC+=ressources/ressource_script/action_question.c
SRC+=ressources/ressource_script/get_type.c
SRC+=ressources/ressource_script/parse.c
SRC+=ressources/ressource_script/parse_json_actions.c
SRC+=ressources/ressource_script/trigger.c
SRC+=ressources/ressource_script/trigger_pick_item.c
SRC+=ressources/ressource_script/utils.c
SRC+=ressources/ressource_script.c
SRC+=ressources/ressource_sound.c
SRC+=ressources/ressource_texture.c
SRC+=ressources/ressource_texture_utils.c
SRC+=ressources/ressource_utils.c
SRC+=ressources/ressources_registry.c
SRC+=ressources/ressources_registry_array.c
SRC+=ressources/save_datapack.c
SRC+=ressources/save_map.c
SRC+=ressources/save_map_dec.c
SRC+=ressources/save_map_objects.c
SRC+=ressources/save_map_wall_rooms.c
SRC+=ressources/save_ressource.c
SRC+=sdl.c
SRC+=triangulation/ear_clipping.c
SRC+=triangulation/uv_mapping.c
