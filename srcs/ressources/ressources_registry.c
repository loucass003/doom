/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ressources_registry.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/17 14:37:12 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/14 17:20:45 by louali           ###   ########.fr       */
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

	if (!(doom->res_manager.ressources = create_ressources_array(11)))
		return (FALSE);
	if (!init_ressources_select_items(&doom->res_manager))
		return (FALSE);
	error = FALSE;
	error |= a(doom, "MAIN_MENU_BACKGROUND", RESSOURCE_TEXTURE, TRUE);
	error |= a(doom, "GAMEOVER_BACKGROUND", RESSOURCE_TEXTURE, TRUE);
	error |= a(doom, "ITEMS_ICONS", RESSOURCE_TEXTURE, TRUE);
	error |= a(doom, "GUN_ANNIMATION", RESSOURCE_TEXTURE, TRUE);
	error |= a(doom, "AXE_ANNIMATION", RESSOURCE_TEXTURE, TRUE);
	error |= a(doom, "ENEMY_SPRITESHEET", RESSOURCE_TEXTURE, TRUE);
	error |= a(doom, "BOSS_SPRITESHEET", RESSOURCE_TEXTURE, TRUE);
	error |= a(doom, "GRENADE_MODEL", RESSOURCE_MODEL, TRUE);
	error |= a(doom, "ROCKET_MODEL", RESSOURCE_MODEL, TRUE);
	error |= a(doom, "SKYBOX_TEXTURE", RESSOURCE_TEXTURE, TRUE);
	error |= a(doom, "YOUWIN_TEXTURE", RESSOURCE_TEXTURE, TRUE);
	error |= a(doom, "SOUND_WILHELM", RESSOURCE_SOUND, TRUE);
	error |= a(doom, "SOUND_GUN", RESSOURCE_SOUND, TRUE);
	error |= a(doom, "SOUND_FOOTSTEP", RESSOURCE_SOUND, TRUE);
	error |= a(doom, "SOUND_PICKITEM", RESSOURCE_SOUND, TRUE);
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
	set_default_sounds(doom);
	return (FALSE);
}
