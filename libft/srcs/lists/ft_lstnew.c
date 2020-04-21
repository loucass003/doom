/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/13 18:29:46 by llelievr          #+#    #+#             */
/*   Updated: 2018/11/14 12:18:55 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

t_list			*ft_lstnew(void const *content, size_t content_size)
{
	t_list		*new;
	void		*cpy;

	if (!(new = (t_list*)malloc(sizeof(t_list))))
		return (NULL);
	if (!(cpy = malloc(content_size)))
		return (NULL);
	if (content)
		new->content = ft_memcpy(cpy, (void *)content, content_size);
	else
		new->content = NULL;
	new->content_size = content == NULL ? 0 : content_size;
	new->next = NULL;
	return (new);
}
