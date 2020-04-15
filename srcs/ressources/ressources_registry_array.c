/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ressources_registry_array.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/15 03:46:50 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/15 03:51:06 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "doom.h"
#include "ressource.h"

t_bool			a(t_doom *doom, char *name, t_ressource_type type, t_bool fixed)
{
	t_ressource	*ressource;

	if (!(ressource = malloc(sizeof(t_ressource))))
		return (FALSE);
	*ressource = (t_ressource){ .type = type, .fixed = fixed, .used = fixed };
	ft_memcpy(ressource->display_name, name, ft_strlen(name));
	return (!append_ressources_array(&doom->res_manager.ressources, ressource));
}

t_bool			registry_array_part1(t_doom *doom)
{
	t_bool		error;

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
	return (error);
}

t_bool			registry_array_part2(t_doom *doom)
{
	t_bool		error;

	error = FALSE;
	error |= a(doom, "HELP", RESSOURCE_TEXTURE, TRUE);
	error |= a(doom, "DOOR MODEL", RESSOURCE_MODEL, TRUE);
	error |= a(doom, "TRANSPORTEUR", RESSOURCE_MODEL, TRUE);
	error |= a(doom, "EXPLOSION", RESSOURCE_TEXTURE, TRUE);
	error |= a(doom, "EDITOR ICONS", RESSOURCE_TEXTURE, TRUE);
	error |= a(doom, "SCRIPT", RESSOURCE_SCRIPT, TRUE);
	return (error);
}
