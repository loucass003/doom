/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   datapack.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/24 22:46:54 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/25 00:14:02 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATAPACK_H
# define DATAPACK_H

# include "ressource.h"

typedef struct		s_datapack
{
	t_ressource			*ressources;
	struct s_img		*textures;
	struct s_renderable	*renderables;

}					t_datapack;

#endif