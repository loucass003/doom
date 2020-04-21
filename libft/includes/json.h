/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/29 12:19:06 by llelievr          #+#    #+#             */
/*   Updated: 2019/04/07 23:31:50 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JSON_H
# define JSON_H

# include "libft.h"

typedef enum		e_json_types
{
	JSON_STRING,
	JSON_NUMBER,
	JSON_OBJECT,
	JSON_ARRAY,
	JSON_BOOLEAN,
	JSON_NULL,
	JSON_EOF
}					t_json_types;

typedef struct		s_json_state
{
	char		*str;
	int			pos;
	int			len;
}					t_json_state;

typedef struct		s_json_value
{
	t_json_types	type;
}					t_json_value;

typedef struct		s_json_member
{
	t_json_value			*value;
	struct s_json_string	*string;
	struct s_json_member	*next;
}					t_json_member;

typedef struct		s_json_element
{
	t_json_value			*value;
	struct s_json_element	*next;
}					t_json_element;

typedef struct		s_json_object
{
	t_json_value	super;
	size_t			members_count;
	t_json_member	*elements;
}					t_json_object;

typedef struct		s_json_array
{
	t_json_value	super;
	size_t			elems_count;
	t_json_element	*elements;
}					t_json_array;

typedef struct		s_json_string
{
	t_json_value	super;
	int				value_len;
	char			value[];
}					t_json_string;

typedef struct		s_json_number
{
	t_json_value	super;
	double			value;
}					t_json_number;

typedef struct		s_json_boolean
{
	t_json_value	super;
	t_bool			value;
}					t_json_boolean;

t_json_value		*parse_value(t_json_state *s);
t_json_value		*json_parse_boolean(t_json_state *state, t_bool is_true);
t_json_value		*json_parse_number(t_json_state *state);
t_json_value		*json_parse_null(t_json_state *state);
t_json_value		*json_parse_string(t_json_state *state);
t_json_value		*json_parse_object(t_json_state *state);
t_json_value		*json_object_get(t_json_object *obj, char *key);
t_json_value		*json_parse_array(t_json_state *state);

t_json_array		*json_to_array(t_json_value *val);
t_json_array		*json_get_array(t_json_object *obj, char *key);
t_bool				*json_to_bool(t_json_value *val);
t_bool				*json_get_bool(t_json_object *obj, char *key);
double				*json_to_number(t_json_value *val);
double				*json_get_number(t_json_object *obj, char *key);
t_json_string		*json_to_string(t_json_value *val);
t_json_string		*json_get_string(t_json_object *obj, char *key);
t_json_object		*json_get_object(t_json_object *obj, char *key);

char				json_skip_ws(t_json_state *state);
int					json_is_ws(char c);
int					json_match_key(t_json_state *s, char *at, char *k, int len);

void				*json_free_ret(void *addr);
void				json_free_array(struct s_json_element **elems);
void				json_free_members(struct s_json_member **member);
void				json_free_value(t_json_value *val);

#endif
