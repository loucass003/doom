/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ressources_registry.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/17 14:37:12 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/25 07:03:15 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "doom.h"
#include "ressource.h"

t_bool	a(t_doom *doom, char *name, t_ressource_type type, t_bool fixed)
{
	t_ressource	*ressource;
	
	if (!(ressource = malloc(sizeof(t_ressource))))
		return (FALSE);
	*ressource = (t_ressource){ .type = type, .fixed = fixed, .used = fixed };
	ft_memcpy(ressource->display_name, name, ft_strlen(name));
	return (!append_ressources_array(&doom->res_manager.ressources, ressource));
}

t_bool			init_ressources_select_items(t_ressource_manager *rm)
{
	if (rm->ressources_types || !(rm->ressources_types = create_select_items_array(5)))
		return (!!rm->ressources_types);
	append_select_items_array(&rm->ressources_types, (t_select_item){ "TEXTURE", RESSOURCE_TEXTURE });
	append_select_items_array(&rm->ressources_types, (t_select_item){ "MODEL", RESSOURCE_MODEL });
	append_select_items_array(&rm->ressources_types, (t_select_item){ "SOUND", RESSOURCE_SOUND });
	return (TRUE);
}

t_bool			init_ressources_registry(t_doom *doom)
{
	t_bool	error;

	if (!(doom->res_manager.ressources = create_ressources_array(10)))
		return (FALSE);
	if (!init_ressources_select_items(&doom->res_manager))
		return (FALSE);
	error = FALSE;
	error = a(doom, "MAIN_MENU_BACKGROUND", RESSOURCE_TEXTURE, TRUE);
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
	while (++i < doom->res_manager.ressources->len)
	{
		r = doom->res_manager.ressources->values[i];
		if (ressource_error(r))
			return (TRUE);
	}
	return (FALSE);
}
