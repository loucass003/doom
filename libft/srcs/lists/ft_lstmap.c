/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/15 11:46:31 by llelievr          #+#    #+#             */
/*   Updated: 2018/11/16 14:53:19 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list		*ft_lstmap(t_list *lst, t_list *(*f)(t_list *elem))
{
	t_list	*origin;
	t_list	*cpy;
	t_list	*new;

	if (!lst)
		return (NULL);
	new = f(lst);
	origin = ft_lstnew(new->content, new->content_size);
	cpy = origin;
	lst = lst->next;
	while (lst)
	{
		new = f(lst);
		cpy->next = ft_lstnew(new->content, new->content_size);
		cpy = cpy->next;
		lst = lst->next;
	}
	return (origin);
}
