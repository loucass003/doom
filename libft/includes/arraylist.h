/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arraylist.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/28 19:44:08 by llelievr          #+#    #+#             */
/*   Updated: 2019/04/30 13:46:45 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARRAYLIST_H
# define ARRAYLIST_H
# include "libft.h"

typedef struct	s_arraylist
{
	int			len;
	int			capacity;
	void		*values[];
}				t_arraylist;

t_arraylist		*create_arraylist(int capacity);
t_arraylist		*arraylist_append(t_arraylist **arr, void *elem);
t_arraylist		*arraylist_appendm(t_arraylist **arr, void *elem, size_t n);
t_arraylist		*arraylist_splicef(t_arraylist *arr, int index, int n,
					void (*del_val)(void *elem));
t_arraylist		*arraylist_splice(t_arraylist *arr, int index, int n);
void			arraylist_clear(t_arraylist **arr, void (*del_val)(void *elem));
int				arraylist_indexof(t_arraylist *arr, void *elem);

#endif
