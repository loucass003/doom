/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_gnl.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/20 12:30:49 by llelievr          #+#    #+#             */
/*   Updated: 2019/01/15 12:17:07 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/types.h>

static t_buff		*new_buf(void)
{
	t_buff	*n;

	if (!(n = (t_buff *)malloc(sizeof(t_buff))))
		return (NULL);
	n->buf = NULL;
	n->len = 0;
	return (n);
}

static int			process(char **line, t_buff **e, ssize_t len)
{
	ssize_t		x;
	char		*tmp;

	x = 0;
	while (x < len && (*e)->buf[x] != '\n')
		x++;
	if ((*e)->buf[x] == '\n')
	{
		*line = ft_strsub((*e)->buf, 0, x);
		tmp = (*e)->buf;
		(*e)->buf = ft_strsub((*e)->buf + x + 1, 0, len - x - 1);
		(*e)->len = len - x - 1;
		ft_strdel(&tmp);
		if ((*e)->buf[0] == '\0')
			ft_strdel(&((*e)->buf));
		return (1);
	}
	return (0);
}

static int			append(t_buff **e, char *buf, ssize_t rd)
{
	char	*join;
	ssize_t	e_len;

	buf[rd] = '\0';
	e_len = (*e)->len;
	(*e)->len += rd;
	if (!(*e)->buf)
		(*e)->buf = ft_strnew(0);
	join = ft_strnew((*e)->len);
	join = ft_strcpy(join, (*e)->buf);
	ft_strdel(&((*e)->buf));
	(*e)->buf = ft_strcat(join + e_len, buf) - e_len;
	if (!(*e)->buf)
		return (0);
	return (1);
}

int					ft_gnl(const int fd, char **line)
{
	static t_buff	*lst[OPEN_MAX];
	char			buf[GNL_BUFF_SIZE + 1];
	ssize_t			rd;

	if (fd < 0 || !line || (!lst[fd] && !(lst[fd] = new_buf())))
		return (-1);
	if (lst[fd]->buf)
		if (process(line, &(lst[fd]), lst[fd]->len) == 1)
			return (1);
	while ((rd = read(fd, &buf, GNL_BUFF_SIZE)) > 0)
	{
		if (!append(&lst[fd], buf, rd))
			return (-1);
		if (process(line, &lst[fd], lst[fd]->len))
			return (1);
	}
	if (rd == 0 && lst[fd]->buf)
	{
		*line = ft_strsub(lst[fd]->buf, 0, lst[fd]->len);
		ft_strdel(&(lst[fd]->buf));
		return (1);
	}
	return (rd == 0 ? 0 : -1);
}
