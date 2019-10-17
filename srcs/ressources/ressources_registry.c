/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ressources_registry.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/17 14:37:12 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/17 14:46:51 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "doom.h"
#include "ressource.h"

static t_bool	a(t_doom *doom, char *name, t_render_type type)
{
	return (append_ressources_array(&doom->ressources,
		(t_ressource){ .display_name = name, .type = type }));
}

t_bool			init_ressources_registry(t_doom *doom)
{
	t_bool	error;

	if (!(doom->ressources = create_ressources_array(10)))
		return (FALSE);
	error = FALSE;
	error = a(doom, "MAIN_MENU_BACKGROUND", RESSOURCE_TEXTURE);
	return (error);
}

t_bool			check_ressources_errors(t_doom *doom)
{
	t_ressource	*r;
	int			i;

	i = -1;
	while (++i < doom->ressources->len)
	{
		r = &doom->ressources->values[i];
		if (!r->loaded)
			return (FALSE);
	}
	return (TRUE);
}
