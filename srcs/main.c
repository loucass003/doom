/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/09 14:54:33 by llelievr          #+#    #+#             */
/*   Updated: 2019/04/14 17:08:11 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL.h>
#include "libft.h"
#include <math.h>
#include <limits.h>

typedef enum		e_side
{
	S_COLINEAR = 0,
	S_FRONT = 1,
	S_BACK = 2,
	S_SPANNING = 3
}					t_side;

typedef enum		e_node_type
{
	N_NODE,
	N_LEAF
}					t_node_type;

typedef struct		s_line
{
	t_vec2			a;
	t_vec2			b;
//	uint8_t			side_sign;
}					t_line;

typedef struct			s_line_list
{
	t_line				line;
	struct s_line_list	*next;
}						t_line_list;

typedef struct		s_node
{
	t_node_type		type;
	t_line			partition;
	t_line_list		*segments;
	struct s_node	*front;
	struct s_node	*back;
}					t_node;

t_line lines[] = {
	/*{ { 0, 150 }, { 0, 450 } },
	{ { 0, 450 }, { 800, 450 } },
	{ { 800, 450 }, { 800, 300 } },
	{ { 800, 300 }, { 500, 300 } },
	{ { 500, 300 }, { 500, 75 } },
	{ { 500, 75 }, { 0, 150 } }*/
	{ { 0, 150 }, { 500, 75 } },
	{ { 500, 75 }, { 500, 300 } },
	{ {500, 300}, { 800, 300 }},
	{ {800, 300}, { 800, 450 }},
	{ {800, 450}, {0, 450 }},
//	{ {0, 450}, {0, 150}}
};

t_bool	intersect(t_line *line, t_line *seg, t_vec2 *intersect)
{
	if (line->a.x == seg->a.x && line->a.y == seg->a.y && line->b.x == seg->b.x && line->b.y == seg->b.y)
		return (FALSE);
	t_vec2 i1 = (t_vec2) {
		line->b.y - line->a.y,
		line->a.x - line->b.x
	};
	float c1 = i1.x * (line->a.x) + i1.y * (line->a.y);

	t_vec2 i2 = (t_vec2) {
		seg->b.y - seg->a.y,
		seg->a.x - seg->b.x
	};
	float c2 = i2.x * (seg->a.x) + i2.y * (seg->a.y);
	float d = i1.x * i2.y - i2.x * i1.y;
	if (d == 0)
	{
		intersect->x = -1;
		intersect->y = -1;
		return (FALSE);
	}
	intersect->x = (i2.y * c1 - i1.y * c2) / d;
	intersect->y = (i1.x * c2 - i2.x * c1) / d;
	
	return (TRUE);
}

t_bool	append_list(t_line_list **list, t_line line)
{
	t_line_list	*n;
	t_line_list	*elem;

	if (!(elem = (t_line_list *)malloc(sizeof(t_line_list))))
		return (FALSE);
	elem->line = line;
	elem->next = NULL;
	if (!*list)
		*list = elem;
	else
	{
		n = *list;
		while (n && n->next)
			n = n->next;
		n->next = elem;
	}
	return (TRUE);
}

/*

TODO: 
- Savoir si un polygone est un polygone ferme et ou croise ? (Pas obligatoire pour le moment) (https://stackoverflow.com/a/19180165)
- Savoir si un point est a l interieur d un polygone (https://stackoverflow.com/questions/217578/how-can-i-determine-whether-a-2d-point-is-within-a-polygon)
- Savoir quel cote d une droite est un FRONT/BACK (https://math.stackexchange.com/questions/274712/calculate-on-which-side-of-a-straight-line-is-a-given-point-located) (pourrai aussi etre calcule avec l editeur de map)

Iterer sur le polygone
	Tester un point a l exterieur et a l interieur de chaque segments est garder le sigine de d du point a l interieur

d va aussi servir a savoir de quel cote sont les segements des autres sections
*/

t_vec2	line_normal(t_line *line)
{
	return (ft_vec2_norm((t_vec2) {
		line->b.y - line->a.y,
		line->a.x - line->b.x
	}));
}

t_side	get_side_thin(t_line *partition, t_vec2 seg)
{
	const t_vec2 n = line_normal(partition);
	const float side = (seg.x - partition->a.x) * n.x + (seg.y - partition->a.y) * n.y;
	if (side == 0)
		return (S_COLINEAR);
	return (side < 0 ? S_BACK : S_FRONT);
}

t_side	get_side_thick(t_line *partition, t_vec2 seg)
{
	const t_vec2 n = line_normal(partition);
	t_side f_side = get_side_thin(partition, (t_vec2){ (seg.x - n.x) / 2, (seg.y - n.y) / 2 });
	if (f_side == S_FRONT)
		return (S_FRONT);
	else if (f_side == S_BACK)
	{
		t_side b_side = get_side_thin(partition, (t_vec2){ (seg.x + n.x) / 2, (seg.y + n.y) / 2 });
		if (b_side == S_BACK)
			return (S_BACK);
	}
	return (S_COLINEAR);
}



t_side	get_side(t_line *partition, t_line seg)
{
	if (partition->a.x == seg.a.x && partition->a.y == seg.a.y 
		&& partition->b.x == seg.b.x && partition->b.y == seg.b.y)
		return (S_COLINEAR);
	/*const float dot = ft_vec2_cross(
		ft_vec2_norm((t_vec2) { partition->b.x - partition->a.x, partition->b.y - partition->a.y }),
		ft_vec2_norm((t_vec2) { seg.b.x - seg.a.x, seg.b.y - seg.a.y }));
	if (dot == 0)
		return (S_COLINEAR);
	return (dot < 0 ? S_BACK : S_FRONT);*/
	t_side s1 = get_side_thick(partition, seg.a);
	t_side s2 = get_side_thick(partition, seg.b);
	if (s1 == s2)
		return (s1);
	else if (s1 == S_COLINEAR)
		return (s2);
	else if (s2 == S_COLINEAR)
		return (s1);
	else
		return (S_SPANNING);
}

t_node	*create_node(t_line_list *segments)
{
	t_node *node;

	if (!(node = (t_node *)malloc(sizeof(t_node))))
		return (NULL);
	node->front = NULL;
	node->back = NULL;
	node->segments = segments;
	node->type = N_NODE;
	if (segments)
		node->partition = segments->line;
	else
	{
		node->type = N_LEAF;
		//TODO: add more data (floor_height, ceil_height)
	}
	return (node);
}

void build_node(t_node *node)
{
	t_line_list	*lst;
	t_line_list	*front;
	t_line_list	*back;
	t_line_list	*colinear;

	if (node->type == N_LEAF)
		return;
	lst = node->segments;
	front = NULL;
	back = NULL;
	colinear = NULL;
	while (lst)
	{
		t_side side = get_side(&node->partition, lst->line);
		if (side == S_COLINEAR)
			append_list(&colinear, lst->line);
		else if (side == S_FRONT)
			append_list(&front, lst->line);
		else if (side == S_BACK)
			append_list(&back, lst->line);
		else if (side == S_SPANNING)
		{
			t_vec2 it;
			if (!intersect(&node->partition, &lst->line, &it))
				continue;
			t_line a = { lst->line.a, it };
			t_line b = { it, lst->line.b };
			t_side side = get_side(&node->partition, a);
			if (side == S_FRONT)
				append_list(&front, a);
			else if (side == S_BACK)
				append_list(&back, a);
			side = get_side(&node->partition, b);
			if (side == S_FRONT)
				append_list(&front, b);
			else if (side == S_BACK)
				append_list(&back, b);
		}
		lst = lst->next;
	}
	node->segments = colinear;
	node->front = create_node(front);
	node->back = create_node(back);
	build_node(node->front);
	build_node(node->back);
}

int		main(void)
{
	t_vec2 max = {INT_MIN, INT_MIN};
	t_vec2 min = {INT_MAX, INT_MAX};
	int count = sizeof(lines) / sizeof(t_line);
	for (int i = 0; i < count; i++)
	{
		max.x = fmax(max.x, fmax(lines[i].a.x, lines[i].b.x));
		max.y = fmax(max.y, fmax(lines[i].a.y, lines[i].b.y));
		min.x = fmin(min.x, fmin(lines[i].a.x, lines[i].b.x));
		min.y = fmin(min.y, fmin(lines[i].a.y, lines[i].b.y));
	}
	printf("(%f, %f) (%f, %f)\n", min.x, min.y, max.x, max.y);
	t_line_list *lst = NULL;
	for (int i = 0; i < count; i++)
		append_list(&lst, lines[i]);
	t_node *n = create_node(lst);
	build_node(n);
	if (!n)
		return (-1);
	return (0);
}
