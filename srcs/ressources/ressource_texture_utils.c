/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ressource_texture_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/17 21:58:49 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/17 21:58:54 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ressource.h"

t_ressource		*get_next_texture(t_ressource_manager *r, t_ressource *res)
{
	int			i;
	t_ressource	*t_r;

	i = 0;
	if (res)
	{
		res->used--;
		i = ressources_indexof(r->ressources, res) + 1;
	}
	t_r = NULL;
	while (!t_r || t_r->type != RESSOURCE_TEXTURE || !t_r->loaded)
	{
		if (i == r->ressources->len)
			i = 0;
		t_r = r->ressources->values[i++];
	}
	if (t_r)
		t_r->used++;
	return (t_r);
}

t_ressource		*get_prev_texture(t_ressource_manager *r, t_ressource *res)
{
	int			i;
	t_ressource	*t_r;

	i = 0;
	if (res)
	{
		res->used--;
		i = ressources_indexof(r->ressources, res) - 1;
	}
	t_r = NULL;
	while (!t_r || t_r->type != RESSOURCE_TEXTURE || !t_r->loaded)
	{
		if (i == -1)
			i = r->ressources->len - 1;
		t_r = r->ressources->values[i--];
	}
	if (t_r)
		t_r->used++;
	return (t_r);
}