/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ressource_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/17 21:58:49 by llelievr          #+#    #+#             */
/*   Updated: 2019/12/02 15:09:04 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ressource.h"

t_ressource		*get_next_ressource(t_ressource_manager *r, t_ressource *res, t_ressource_type type)
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
	while (!t_r || t_r->type != type || !t_r->loaded)
	{
		if (i == r->ressources->len)
			i = 0;
		t_r = r->ressources->values[i++];
	}
	if (t_r)
		t_r->used++;
	return (t_r);
}

t_ressource		*get_prev_ressource(t_ressource_manager *r, t_ressource *res, t_ressource_type type)
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
	while (!t_r || t_r->type != type || !t_r->loaded)
	{
		if (i == -1)
			i = r->ressources->len - 1;
		t_r = r->ressources->values[i--];
	}
	if (t_r)
		t_r->used++;
	return (t_r);
}

t_ressource		*get_ressource(t_ressource_manager *r, t_ressource_type type)
{
	int			i;

	i = -1;
	while (++i < r->ressources->len)
		if (r->ressources->values[i]->type == type)
			return (r->ressources->values[i]);
	return (NULL);
}