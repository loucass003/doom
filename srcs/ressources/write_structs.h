/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_structs.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/25 05:41:51 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/13 01:41:18 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WRITE_STRUCTS_H
# define WRITE_STRUCTS_H

# include <libft.h>
# include "ressource.h"

typedef struct		s_wr_ressource
{
	t_ressource_type	type;
	t_bool				fixed;
	t_bool				loaded;
	int					name_len;
}					t_wr_ressource;

typedef struct		s_wr_room
{
	t_bool			closed;
	int				walls_count;
}					t_wr_room;

typedef struct		s_wr_wall
{
	int				indice;
	int				resource_index;
	uint8_t			normal_type;
	t_bool			invisible;
	t_bool			collisions;
}					t_wr_wall;

typedef struct		s_wr_img
{
	size_t			size;
	uint32_t		width;
	uint32_t		height;
	t_bool			secure;
}					t_wr_img;

typedef struct		s_wr_data
{
	t_wr_header		header;
}					t_wr_data;

#endif