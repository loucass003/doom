/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdel.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/14 12:19:03 by llelievr          #+#    #+#             */
/*   Updated: 2018/11/16 16:40:18 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void				ft_lstdel(t_list **alst, void (*del)(void *, size_t))
{
	if ((alst[0])->next)
		ft_lstdel(&(alst[0])->next, del);
	ft_lstdelone(alst, del);
}
