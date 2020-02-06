/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ressources_registry.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/17 14:37:12 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/06 13:18:34 by llelievr         ###   ########.fr       */
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

	if (!(doom->res_manager.ressources = create_ressources_array(22)))
		return (FALSE);
	if (!init_ressources_select_items(&doom->res_manager))
		return (FALSE);
	error = FALSE;
	error |= a(doom, "MAIN MENU BACKGROUND", RESSOURCE_TEXTURE, TRUE);
	error |= a(doom, "GAMEOVER BACKGROUND", RESSOURCE_TEXTURE, TRUE);
	error |= a(doom, "ITEMS ICONS", RESSOURCE_TEXTURE, TRUE);
	error |= a(doom, "GUN ANNIMATION", RESSOURCE_TEXTURE, TRUE);
	error |= a(doom, "AXE ANNIMATION", RESSOURCE_TEXTURE, TRUE);
	error |= a(doom, "ENEMY SPRITESHEET", RESSOURCE_TEXTURE, TRUE);
	error |= a(doom, "BOSS SPRITESHEET", RESSOURCE_TEXTURE, TRUE);
	error |= a(doom, "GRENADE MODEL", RESSOURCE_MODEL, TRUE);
	error |= a(doom, "ROCKET MODEL", RESSOURCE_MODEL, TRUE);
	error |= a(doom, "SKYBOX TEXTURE", RESSOURCE_TEXTURE, TRUE);
	error |= a(doom, "YOUWIN TEXTURE", RESSOURCE_TEXTURE, TRUE);
	error |= a(doom, "SOUND WILHELM", RESSOURCE_SOUND, TRUE);
	error |= a(doom, "SOUND GUN", RESSOURCE_SOUND, TRUE);
	error |= a(doom, "SOUND FOOTSTEP", RESSOURCE_SOUND, TRUE);
	error |= a(doom, "SOUND PICKITEM", RESSOURCE_SOUND, TRUE);
	error |= a(doom, "SOUND BOSS FOOTSTEP", RESSOURCE_SOUND, TRUE);
	error |= a(doom, "SOUND BOSS GUN", RESSOURCE_SOUND, TRUE);
	error |= a(doom, "SOUND BOSS DIE", RESSOURCE_SOUND, TRUE);
	error |= a(doom, "SOUND BOSS HIT", RESSOURCE_SOUND, TRUE);
	error |= a(doom, "SOUND EXPLOSION", RESSOURCE_SOUND, TRUE);
	error |= a(doom, "LEADERBOARD", RESSOURCE_TEXTURE, TRUE);
	error |= a(doom, "HELP", RESSOURCE_TEXTURE, TRUE);
	error |= a(doom, "DOOR MODEL", RESSOURCE_MODEL, TRUE);
	error |= a(doom, "TRANSPORTEUR", RESSOURCE_MODEL, TRUE);
	error |= a(doom, "EXPLOSION", RESSOURCE_TEXTURE, TRUE);
	error |= a(doom, "EDITOR ICONS", RESSOURCE_TEXTURE, TRUE);
	return (error);
}

t_bool			ressource_error(t_ressource *r)
{
	return (r->type == RESSOURCE_UNSET || !r->loaded || !*r->display_name);
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
