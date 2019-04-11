/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/09 14:54:33 by llelievr          #+#    #+#             */
/*   Updated: 2019/04/11 20:28:23 by llelievr         ###   ########.fr       */
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
	S_BACK = 2
}					t_side;

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
	t_line			partition;
	t_line_list		*segments;
	struct s_node	*front;
	struct s_node	*back;
}					t_node;

t_line lines[] = {
	{ { 0, 150 }, { 0, 450 } },
	{ { 0, 450 }, { 800, 450 } },
	{ { 800, 450 }, { 800, 300 } },
	{ { 800, 300 }, { 500, 300 } },
	{ { 500, 300 }, { 500, 75 } },
	{ { 500, 75 }, { 0, 150 } }
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
		return (FALSE);
	intersect->x = (i2.y * c1 - i1.y * c2) / d;
	intersect->y = (i1.x * c2 - i2.x * c1) / d;
	if (intersect->x < min(seg->a.x, seg->b.x) || intersect->x > max(seg->a.x, seg->b.x) // FAUX ! Ca ne verifie pas si le point d intersection est dans le segment
	|| intersect->y < min(seg->a.y, seg->b.y) || intersect->y > max(seg->a.y, seg->b.y)) // (ref https://stackoverflow.com/questions/4030565/line-and-line-segment-intersection?rq=1)
		return (FALSE);
	return (TRUE);
}

t_bool	append_list(t_line_list **list, t_line *line)
{
	t_line_list	*n;
	t_line_list	*elem;

	if (!(elem = (t_line_list *)malloc(sizeof(t_line_list))))
		return (FALSE);
	elem->line = *line;
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

void	get_line_sides(t_line *line)
{
	t_vec2 i1 = {
		line->a.x + (line->b.y - line->a.y),
		line->a.y + -(line->b.x - line->a.x)
	};
	float d = (i1.x - line->b.x) * (line->b.y - line->a.y)
	- (i1.y - line->a.y) * (line->b.x - line->a.x);

	t_vec2 i2 = {
		line->a.x + -(line->b.y - line->a.y),
		line->a.y + (line->b.x - line->a.x)
	};
	float d2 = (i2.x - line->b.x) * (line->b.y - line->a.y)
	- (i2.y - line->a.y) * (line->b.x - line->a.x);
	printf("(%f, %f) (%f, %f) (%f, %f)\n", i1.x, i1.y, i2.x, i2.y, d, d2);
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


t_side	get_side(t_line *partition, t_line *seg)
{
	const float dot = ft_vec2_cross(
		ft_vec2_norm((t_vec2) { partition->b.x - partition->a.x, partition->b.y - partition->a.y }),
		ft_vec2_norm((t_vec2) { seg->b.x - seg->a.x, seg->b.y - seg->a.y }));
	if (dot == 0)
		return (S_COLINEAR);
	return (dot < 0 ? S_BACK : S_FRONT);
}


t_node	*build_node(t_line_list *lines, int lines_count)
{
	t_line_list	*front = NULL;
	int			front_count = 0;
	t_line_list	*back = NULL;
	int			back_count = 0;
	t_node		*node;

	if (lines_count == 0 || !(node = (t_node *)malloc(sizeof(t_node))))
		return (NULL);
	node->segments = lines;
	node->partition = lines->line;
	while (lines)
	{
		t_vec2 it;
		if (intersect(&node->partition, &lines->line, &it) == FALSE)
		{
			lines = lines->next;
			continue;
		}
		t_side side = get_side(&node->partition, &lines->line);
		if (side == S_FRONT)
		{
			append_list(&front, &lines->line);
			front_count++;
		}
		else if (side == S_BACK)
		{
			append_list(&back, &lines->line);
			back_count++;
		}
		printf("(%d) (%f, %f)\n", side, it.x, it.y);
		lines = lines->next;
	}
 
	printf("build front node with %d segments\n", front_count);
	node->front = build_node(front, front_count);
	printf("build back node with %d segments\n", front_count);
	node->back = build_node(back, back_count);
	return (node);
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
		append_list(&lst, &lines[i]);
	t_node *n = NULL;

	n = build_node(lst, count);

	if (!n)
		return (-1);
	return (0);
}
