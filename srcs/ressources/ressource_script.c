/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ressource_script.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/10 14:15:25 by lloncham          #+#    #+#             */
/*   Updated: 2020/02/11 12:46:39 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "script.h"
#include "ressource.h"
#include "file_utils.h"

t_bool		load_script(t_ressource *r, char *path)
{
    t_script_data	*script;

	if (!(script = ft_memalloc(sizeof(t_script_data))))
		return (FALSE);
	if (!(script->script_str = get_file_content(path, &script->script_len)))
		return (FALSE);
	if (!parse_script_data(script))
		return (FALSE);
	r->data.script_data = script;
	r->loaded = TRUE;
	return (TRUE);
}

t_bool			read_script(t_ressource_manager *r, t_script_data **s_data)
{
	t_script_data	*s;

	if (!(s = malloc(sizeof(t_script_data))))
		return (FALSE);
	if (!io_memcpy(&r->reader, &s->script_len, sizeof(int)))
		return (FALSE);
	if (!(s->script_str = malloc(sizeof(char) * s->script_len)))
		return (FALSE);
	if (!io_memcpy(&r->reader, s->script_str, sizeof(char) * s->script_len))
		return (FALSE);
	if (!parse_script_data(s))
		return (FALSE);	
	*s_data = s;
	return (TRUE);
}

t_bool		write_script(t_ressource_manager *rm, t_script_data *s_data)
{
	dp_write(rm, &s_data->script_len, sizeof(int));
	dp_write(rm, s_data->script_str, s_data->script_len * sizeof(char));
    return (TRUE);
}

static t_trigger_type	get_trigger_type(char *str)
{
	const int	len = ft_strlen(str);

	if (len == 5 && ft_strncmp(str, "SPAWN", 5) == 0)
		return (TRIG_SPAWN);
	if (len == 9 && ft_strncmp(str, "PICK ITEM", 9) == 0)
		return (TRIG_PICK_ITEM);
	if (len == 4 && ft_strncmp(str, "AREA", 4) == 0)
		return (TRIG_AREA);
	return (TRIG_NONE);
}

t_bool		script_return_error(char *error)
{
	ft_putstr("Script error: ");
	ft_putendl(error);
	return (FALSE);
}

t_bool			parse_script_data(t_script_data *s_data)
{
	t_json_state	state;
	t_json_value	*val;
	t_json_array	*array;
	t_json_element	*element;
	
	t_trigger_type	trig_type;
	t_json_string	*j_string;

	state = (t_json_state){s_data->script_str, 0, s_data->script_len};
	if (!(val = parse_value(&state)))
		return (FALSE);
	if (!(array = json_to_array(val)))
		return (FALSE);
	s_data->script_count = array->elems_count; 
	if (!(s_data->scripts = ft_memalloc(sizeof(t_script) * s_data->script_count)))
		return (FALSE);	
	element = array->elements;
	// while (element)
	// {
	// 	if (element->value->type != JSON_OBJECT)
	// 		return (FALSE);
	// 	t_json_object *object = json_get_object(element->value, "trigger");
	// 	if (!(j_string = json_get_string(object, "type")))
	// 		return (FALSE);
	// 	trig_type = get_trigger_type(j_string->value);
	// 	if (trig_type == TRIG_NONE)
	// 		return(script_return_error(j_string->value));
	// 	if (trig_type == TRIG_SPAWN)
	// 	{
	// 		printf("SPAWN\n");
	// 	}
	// 	if (trig_type == TRIG_PICK_ITEM)
	// 	{
	// 		printf("ITEM\n");
	// 	}
	// 	if (trig_type == TRIG_AREA)
	// 	{
	// 		printf("AREA\n");
	// 	}
	// 		//
	// 	element = element->next;
	// }
	json_free_value(val);
	return (TRUE);
}