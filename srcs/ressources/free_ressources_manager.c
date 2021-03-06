/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_ressources_manager.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lisa <Lisa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/11 06:02:31 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/18 22:04:12 by Lisa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ressource.h"
#include "image.h"
#include "script.h"

void	free_ressource(t_ressource **r_addr, t_bool free_itself)
{
	t_ressource		*r;

	r = *r_addr;
	if (r->loaded)
	{
		if (r->type == RESSOURCE_MODEL && r->data.model)
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
	if (free_itself)
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
		free_ressource(&r, TRUE);
	}
	ft_memdel((void **)resources);
}

void	free_ressources_manager(t_ressource_manager *rm)
{
	ft_memdel((void **)&rm->ressources_types);
	free_ressources(&rm->ressources);
}
