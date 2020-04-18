/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_ressource.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lisa <Lisa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/13 01:08:20 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/18 18:46:09 by Lisa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ressource.h"
#include "doom.h"
#include "write_structs.h"
#include "script.h"

t_bool		write_ressource(t_ressource_manager *rm, t_ressource *r)
{
	t_wr_ressource	wr_res;

	wr_res = (t_wr_ressource){
		.type = r->type,
		.fixed = r->fixed,
		.loaded = r->loaded,
		.name_len = ft_strlen(r->display_name),
		.used = r->used,
	};
	dp_write(rm, &wr_res, sizeof(t_wr_ressource));
	dp_write(rm, r->display_name, wr_res.name_len);
	if (!wr_res.loaded)
		return (TRUE);
	if (r->type == RESSOURCE_TEXTURE)
		return (write_texture(rm, r->data.texture));
	else if (r->type == RESSOURCE_MODEL)
		return (write_model(rm, r->data.model));
	else if (r->type == RESSOURCE_SOUND)
		return (write_sound(rm, r->data.sound));
	else if (r->type == RESSOURCE_SCRIPT)
		return (write_script(rm, r->data.script_data));
	return (TRUE);
}

t_bool		write_ressources(t_ressource_manager *rm)
{
	int				i;
	t_ressource		*r;

	dp_write(rm, &rm->ressources->len, sizeof(int));
	i = -1;
	while (++i < rm->ressources->len)
	{
		r = rm->ressources->values[i];
		if (!write_ressource(rm, r))
			return (FALSE);
	}
	return (TRUE);
}
