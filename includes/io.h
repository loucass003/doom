/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/17 10:06:09 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/17 10:10:02 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IO_H
# define IO_H

# include <libft.h>
# define READ_BUFFER (4096)

typedef struct	s_reader
{
	uint8_t		buffer[READ_BUFFER];
	size_t		pos;
	size_t		len;
	int			fd;
}				t_reader;



#endif
