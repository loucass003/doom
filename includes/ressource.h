/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ressource.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/17 10:19:30 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/24 03:21:48 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESSOURCE_H
# define RESSOURCE_H

# include <libft.h>
# include "gui.h"

typedef enum	e_ressource_type
{
	RESSOURCE_UNSET = 0,
	RESSOURCE_TEXTURE = 1,
	RESSOURCE_MODEL = 2,
	RESSOURCE_SOUND = 3
}				t_ressource_type;

typedef union	u_ressource_data
{
	struct s_img	*texture;
	struct s_obj	*model;
}				t_ressource_data;

typedef struct	s_ressource
{
	char				display_name[255];
	char				*path;
	t_ressource_type	type;
	t_ressource_data	data;
	t_bool				loaded;
	size_t				used;
	t_bool				fixed;
}				t_ressource;

typedef struct	s_ressources
{
	int				len;
	int				capacity;
	t_ressource		*values[];
}				t_ressources;

typedef struct	s_ressource_manager
{
	t_ressources	*ressources;
	int				page;
	t_select_items	*ressources_types;
	int				fd;
}				t_ressource_manager;

t_ressources	*create_ressources_array(int capacity);
t_ressources	*append_ressources_array(t_ressources **arr, t_ressource *v);
t_ressources	*splice_ressources_array(t_ressources *arr, int index, int n);
t_ressources	*copy_ressources_array(t_ressources *src, t_ressources **dst);
t_bool			init_ressources_registry(struct s_doom *doom);
t_bool			check_ressources_errors(struct s_doom *doom);
t_bool			ressource_error(t_ressource *r);
t_bool			a(t_doom *doom, char *name, t_ressource_type type, t_bool fixed);
t_bool			load_ressource(t_doom *doom, t_ressource *r, char *path);
t_bool			close_datapack(t_doom *doom);

#endif
