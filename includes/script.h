/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   script.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/10 13:36:27 by lloncham          #+#    #+#             */
/*   Updated: 2020/03/04 17:33:21 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCRIPT_H
# define SCRIPT_H

# include "libft.h"
# include "item.h"
# include "ressource.h"

# define MAX_ACTIONS (25)

typedef enum			e_trigger_type
{
	TRIG_NONE,
	TRIG_SPAWN,
	TRIG_PICK_ITEM,
	TRIG_AREA,
}						t_trigger_type;

typedef struct			s_trigger_area
{
	t_vec3				pos;
	float				radius;
}						t_trigger_area;

typedef struct			s_trigger_pick_item
{
	t_item_type			item_type;
	t_bool				is_weapon_set;
	t_weapon_type		weapon_type;
}						t_trigger_pick_item;

typedef union			u_trigger_data
{
	t_trigger_pick_item pick_item;
	t_trigger_area		area;
}						t_trigger_data;

typedef struct			s_trigger
{
	t_trigger_type		type;
	t_trigger_data		data;
}						t_trigger;

typedef enum			e_action_type
{
	ACTION_NONE,
	ACTION_MESSAGE,
	ACTION_TELEPORT,
	ACTION_QUESTION
}						t_action_type;

typedef struct			s_action_message
{
	int					textes_count;
	char				**textes;
}						t_action_message;

typedef struct			s_action_question
{
	int					quest_count;
	int					answer_count;
	char				**quest;
	char				**answer;
}						t_action_question;

typedef struct			s_action_teleport
{
	t_vec3				pos;
	t_bool				is_rotation_set;
	t_vec3				rotation;
}						t_action_teleport;

typedef union			u_action_data
{
	t_action_message	message;
	t_action_teleport	teleport;
	t_action_question	question;
}						t_action_data;

typedef struct			s_action
{
	t_action_type		type;
	t_action_data		data;
}						t_action;

typedef struct			s_script
{
	t_trigger			trigger;
	int					actions_count;
	t_action			action[MAX_ACTIONS];
	int					use;
	int					use_default;
}						t_script;

typedef struct			s_script_data
{
	int					script_count;
	t_script			*scripts;
	int					script_len;
	char				*script_str;
}						t_script_data;

void					trigger_event(struct s_doom *doom, t_trigger trigger);
void					trigger_script(struct s_doom *doom, t_trigger trigger);
t_bool					parse_script_data(t_script_data *data);
t_bool					write_script(t_ressource_manager *rm,
							struct s_script_data *s_data);
t_bool					read_script(t_ressource_manager *r,
							struct s_script_data **s_data);
t_bool					load_script(t_ressource *r, char *path);
t_bool					trigger_item_weapon(t_trigger_pick_item *trigger,
							t_json_object *object);
t_bool					trigger_pick_item(t_trigger_pick_item *trigger,
							t_json_object *object);
t_bool					script_return_error(char *error);
t_trigger_type			get_trigger_type(char *str);
t_action_type			get_action_type(char *str);
t_bool					parse_json_actions(t_script *script,
							t_json_array *array);


#endif
