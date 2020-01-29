/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ressource_mapper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/29 13:50:12 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/29 15:50:50 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "ressource.h"
#include <fcntl.h>
#include <unistd.h>

static char			*get_file_content(char *file, int *content_len)
{
	int		fd;
	char	*content;
	char	buf[READ_BUFFER + 1];
	int		rd;

	if ((fd = open(file, O_RDONLY)) == -1)
		return (NULL);
	*content_len = 0;
	content = NULL;
	while ((rd = read(fd, &buf, READ_BUFFER)) > 0)
	{
		buf[rd] = 0;
		content = ft_realloc(content, *content_len, *content_len + rd);
		ft_memcpy(content + *content_len, buf, rd);
		*content_len += rd;
	}
	if (rd != -1)
		return (content);
	if (content)
		free(content);
	return (NULL);
}

t_bool		read_ressource_type(t_ressource_manager *r, t_json_object *val,
	char *name, t_ressource_type type)
{
	t_json_object	*obj;
	t_json_member	*e;
	t_json_string	*s;
	int				i;
	const char		v[4][10] = { "UNSET\0", "TEXTURE\0", "MODEL\0", "SOUND\0" };

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
		ft_putendl("Invalid JSON file");
		if (str_content)
			free((void *)str_content);
		return (FALSE);
	}
	free((void *)str_content);
	if (val->type != JSON_OBJECT || !(read_mapping(r, (t_json_object *)val)))
	{
		json_free_value(val);
		ft_putendl("Invalid JSON file");
		return (FALSE);
	}
	json_free_value(val);
	return (TRUE);
}
