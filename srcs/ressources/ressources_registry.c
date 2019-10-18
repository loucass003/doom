/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ressources_registry.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/17 14:37:12 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/18 23:17:17 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "doom.h"
#include "ressource.h"

static t_bool	a(t_doom *doom, char *name, t_ressource_type type)
{
	t_ressource	*ressource;
	

	if (!(ressource = malloc(sizeof(t_ressource))))
		return (FALSE);
	*ressource = (t_ressource){ .type = type };
	ft_memcpy(ressource->display_name, name, ft_strlen(name));
	return (!append_ressources_array(&doom->ressources, ressource));
}

t_bool			init_ressources_registry(t_doom *doom)
{
	t_bool	error;

	if (!(doom->ressources = create_ressources_array(10)))
		return (FALSE);
	error = FALSE;
	error |= a(doom, "MAIN_MENU_BACKGROUND", RESSOURCE_TEXTURE);
	return (error);
}

t_bool			ressource_error(t_ressource *r)
{
	return (r->type == RESSOURCE_UNSET || !r->loaded);
}

t_bool			check_ressources_errors(t_doom *doom)
{
	t_ressource	*r;
	int			i;

	i = -1;
	while (++i < doom->ressources->len)
	{
		r = doom->ressources->values[i];
		if (ressource_error(r))
			return (TRUE);
	}
	return (FALSE);
}
