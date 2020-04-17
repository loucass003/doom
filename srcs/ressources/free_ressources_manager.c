/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_ressources_manager.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/11 06:02:31 by llelievr          #+#    #+#             */
/*   Updated: 2020/03/09 02:09:26 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ressource.h"
#include "image.h"
#include "script.h"

void	free_ressource(t_ressource **r_addr)
{
	t_ressource		*r;

	r = *r_addr;
	if (r->loaded)
	{
		if (r->type == RESSOURCE_MODEL)
		{
			free_renderable(r->data.model, TRUE, TRUE);
			ft_memdel((void **)&r->data.model);
		}
		else if (r->type == RESSOURCE_TEXTURE)
		{
			destroy_image(r->data.texture);
			ft_memdel((void **)&r->data.texture);
		}
		else if (r->type == RESSOURCE_SOUND)
			free_sound(&r->data.sound);
		else if (r->type == RESSOURCE_SCRIPT)
			free_script(&r->data.script_data);
	}
	ft_memdel((void **)r_addr);
}

void	free_ressources(t_ressources **resources)
{
	int				i;
	t_ressource		*r;

	if (!*resources)
		return ;
	i = -1;
	while (++i < (*resources)->len)
	{
		r = (*resources)->values[i];
		free_ressource(&r);
	}
	ft_memdel((void **)resources);
}

void	free_ressources_manager(t_ressource_manager *rm)
{
	ft_memdel((void **)&rm->ressources_types);
	free_ressources(&rm->ressources);
}
