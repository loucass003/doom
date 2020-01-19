/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ressource.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/17 10:19:30 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/19 01:18:38 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESSOURCE_H
# define RESSOURCE_H

# include <libft.h>
# include "gui.h"
# include "io.h"
# include "audio.h"

typedef enum	e_ressource_type
{
	RESSOURCE_UNSET = 0,
	RESSOURCE_TEXTURE = 1,
	RESSOURCE_MODEL = 2,
	RESSOURCE_SOUND = 3
}				t_ressource_type;


typedef union	u_ressource_data
{
	struct s_img		*texture;
	struct s_renderable	*model;
	t_sound				*sound;
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

typedef struct	s_wr_header
{
	size_t		file_size;
}				t_wr_header;

typedef struct	s_ressource_manager
{
	t_ressources	*ressources;
	int				page;
	t_select_items	*ressources_types;
	t_reader		reader;
	struct s_doom	*doom;
	t_wr_header		dp_header;
	char			*path;
}				t_ressource_manager;

t_ressources	*create_ressources_array(int capacity);
t_ressources	*append_ressources_array(t_ressources **arr, t_ressource *v);
t_ressources	*splice_ressources_array(t_ressources *arr, int index, int n);
t_ressources	*copy_ressources_array(t_ressources *src, t_ressources **dst);
int				ressources_indexof(t_ressources *arr, t_ressource *r);
t_bool			init_ressources_registry(struct s_doom *doom);
t_bool			check_ressources_errors(struct s_doom *doom);
t_bool			ressource_error(t_ressource *r);
t_bool			a(t_doom *doom, char *name, t_ressource_type type, t_bool fixed);
t_bool			load_ressource(t_doom *doom, t_ressource *r, char *path);
t_bool			load_texture_file(t_doom *doom, t_ressource *r, char *path);
t_bool			load_model_file(t_doom *doom, t_ressource *r, char *path);
t_bool			close_datapack(t_doom *doom);
t_bool			save_datapack(t_doom *doom);
t_ressource		*get_default_texture(t_ressource_manager *rm, t_bool use);
void			dp_write(t_ressource_manager *rm, const void *buf, size_t n);

t_bool			write_map(t_ressource_manager *r);
t_bool			write_rooms(t_ressource_manager *r);
t_bool			write_room(t_ressource_manager *r, struct s_room *room);
t_bool			write_wall(t_ressource_manager *r, struct s_wall *wall);
t_bool			write_texture(t_ressource_manager *rm, t_img *img);
t_bool			write_model(t_ressource_manager *r, struct s_renderable *model);

t_bool			read_texture(t_ressource_manager *rm, t_img **img);
t_bool			read_ressources(t_ressource_manager *rm);
t_bool			read_ressource(t_ressource_manager *rm, t_ressource *r);
t_bool			read_map(t_ressource_manager *r);
t_bool			read_rooms(t_ressource_manager *r);
t_bool			read_room(t_ressource_manager *r);
t_bool			read_wall(t_ressource_manager *r, struct s_walls **walls);
t_bool			read_model(t_ressource_manager *r, struct s_renderable **m);

t_bool			write_sound(t_ressource_manager *r, struct s_sound *sound);
t_bool			read_sounds(t_ressource_manager *r, struct s_sound **sound);
t_bool			load_sound(t_doom *doom, t_ressource *r, char *path);

t_ressource		*get_next_ressource(t_ressource_manager *r, t_ressource *res, t_ressource_type type);
t_ressource		*get_prev_ressource(t_ressource_manager *r, t_ressource *res, t_ressource_type type);
t_ressource		*get_ressource(t_ressource_manager *r, t_ressource_type type);
#endif
