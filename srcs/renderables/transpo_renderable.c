/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transpo_renderable.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/09 15:46:39 by llelievr          #+#    #+#             */
/*   Updated: 2020/03/09 16:34:18 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "editor.h"

t_transpo		*create_default_transpo(t_doom *doom, t_object *object)
{
	t_transpo	*transpo;

	if (!(transpo = ft_memalloc(sizeof(t_transpo))))
		return (NULL);
	transpo->connected = objects_indexof(doom->editor.objects, object);
	return (transpo);
}

void				prev_transpo(t_transpo *transpo, t_doom *doom)
{
	if (transpo->connected == 0)
		transpo->connected = doom->editor.objects->len;
	transpo->connected--;
	while (doom->editor.objects->values[transpo->connected].type
		!= OBJECT_TRANSPO)
	{
		if (transpo->connected == 0)
			transpo->connected = doom->editor.objects->len;
		transpo->connected--;
	}
}

void				next_transpo(t_transpo *transpo, t_doom *doom)
{
	if (transpo->connected == doom->editor.objects->len - 1)
			transpo->connected = 0;
	else
		transpo->connected++;
	while (doom->editor.objects->values[transpo->connected].type
		!= OBJECT_TRANSPO)
	{
		if (transpo->connected == doom->editor.objects->len - 1)
			transpo->connected = 0;
		else
			transpo->connected++;
	}
}
