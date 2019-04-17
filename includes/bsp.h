/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsp.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 19:34:25 by llelievr          #+#    #+#             */
/*   Updated: 2019/04/17 16:54:04 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BSP_H
# define BSP_H
# include "doom.h"

typedef enum		e_node_type
{
	N_NODE,
	N_LEAF
}					t_node_type;

typedef struct		s_node
{
	t_node_type		type;
	t_line			partition;
	t_line_list		*segments;
	struct s_node	*front;
	struct s_node	*back;
}					t_node;

t_bool				append_list(t_line_list **list, t_line line);
t_node				*create_node(t_line_list *segments);
void				build_node(t_node *node);
void				print_node(t_node *n);
void				draw_line(t_img *img, t_line l, int color);

#endif
