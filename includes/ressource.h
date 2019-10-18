/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ressource.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/17 10:19:30 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/18 23:16:27 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESSOURCE_H
# define RESSOURCE_H

# include <libft.h>

typedef enum	e_ressource_type
{
	RESSOURCE_UNSET,
	RESSOURCE_TEXTURE,
	RESSOURCE_MODEL,
	RESSOURCE_SOUND
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
}				t_ressource;


typedef struct	s_ressources
{
	int				len;
	int				capacity;
	t_ressource		*values[];
}				t_ressources;

t_ressources	*create_ressources_array(int capacity);
t_ressources	*append_ressources_array(t_ressources **arr, t_ressource *v);
t_ressources	*splice_ressources_array(t_ressources *arr, int index, int n);
t_ressources	*copy_ressources_array(t_ressources *src, t_ressources **dst);
t_bool			init_ressources_registry(struct s_doom *doom);
t_bool			check_ressources_errors(struct s_doom *doom);
t_bool			ressource_error(t_ressource *r);

#endif
