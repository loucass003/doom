/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_ressource.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/24 01:51:49 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/12 17:28:33 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "doom.h"
#include "ressource.h"

t_bool		load_ressource(t_doom *doom, t_ressource *r, char *path)
{
	if (r->type == RESSOURCE_TEXTURE)
		return (load_texture(doom, r, path));
	else
		return (FALSE);
}
