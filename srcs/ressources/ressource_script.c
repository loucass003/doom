/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ressource_script.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lisa <Lisa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/10 14:15:25 by lloncham          #+#    #+#             */
/*   Updated: 2020/04/15 17:29:43 by Lisa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "script.h"
#include "ressource.h"
#include "file_utils.h"
#include "item.h"

t_bool					load_script(t_ressource *r, char *path)
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

t_bool					read_script(t_ressource_manager *r,
	t_script_data **s_data)
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

t_bool					write_script(t_ressource_manager *rm,
	t_script_data *s_data)
{
	dp_write(rm, &s_data->script_len, sizeof(int));
	dp_write(rm, s_data->script_str, s_data->script_len * sizeof(char));
	return (TRUE);
}
