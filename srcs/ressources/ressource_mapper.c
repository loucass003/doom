/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ressource_mapper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/29 13:50:12 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/21 17:11:23 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "ressource.h"
#include "file_utils.h"

t_bool		read_ressource_type(t_ressource_manager *r, t_json_object *val,
	char *name, t_ressource_type type)
{
	t_json_object	*obj;
	t_json_member	*e;
	t_json_string	*s;
	int				i;
	char			v[5][10] = { "UNSET\0", "TEXTURE\0", "MODEL\0", "SOUND\0", "SCRIPT\0" };

	if (!(obj = json_get_object(val, name)))
		return (FALSE);
	e = obj->elements;
	while (e)
	{
		if (e->value->type != JSON_STRING
			|| (i = get_ressource_by_name(r, type, e->string->value)) == -1)
		{
			ft_putstr("RESSOURCE MAPPER: Invalid ");
			ft_putstr(v[type]);
			ft_putstr(" at element ");
			ft_putendl(e->string->value);
			return (FALSE);
		}
		s = (t_json_string *)e->value;
		load_ressource(r->doom, r->ressources->values[i], s->value);
		e = e->next;
	}
	return (TRUE);
}

t_bool		read_mapping(t_ressource_manager *r, t_json_object *val)
{
	if (!read_ressource_type(r, val, "TEXTURES", RESSOURCE_TEXTURE))
		return (FALSE);
	if (!read_ressource_type(r, val, "MODELS", RESSOURCE_MODEL))
		return (FALSE);
	if (!read_ressource_type(r, val, "SOUNDS", RESSOURCE_SOUND))
		return (FALSE);
	if (!read_ressource_type(r, val, "SCRIPTS", RESSOURCE_SCRIPT))
		return (FALSE);
	return (TRUE);
}

t_bool		ressource_mapper(t_ressource_manager *r, char *path)
{
	char			*str_content;
	int				str_len;
	t_json_state	state;
	t_json_value	*val;

	str_content = get_file_content(path, &str_len);
	state = (t_json_state){str_content, 0, str_len};
	if (!str_len || !(val = parse_value(&state)))
	{
		ft_putendl("Invalid JSON MAPPING file");
		if (str_content)
			free((void *)str_content);
		return (FALSE);
	}
	free((void *)str_content);
	if (val->type != JSON_OBJECT || !(read_mapping(r, (t_json_object *)val)))
	{
		json_free_value(val);
		ft_putendl("Invalid JSON MAPPING content");
		return (FALSE);
	}
	json_free_value(val);
	return (TRUE);
}
