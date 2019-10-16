/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gen-embed.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/16 01:44:14 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/16 03:23:52 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int		main(int argc, char **argv)
{
	int			fd;
	int			rd;
	int			content_len;
	int			i;
	int			first = 0;
	uint32_t	buf[GNL_BUFF_SIZE / sizeof(uint32_t)];

	if (argc != 2 || (fd = open(argv[1], O_RDONLY)) == -1)
		return (0);
	content_len = 0;
	while ((rd = read(fd, &buf, GNL_BUFF_SIZE)) > 0)
	{
		i = -1;
		while (++i < rd / sizeof(uint32_t))
		{
			if (first != 0)
				printf("%s", ((i) % 6 == 0 ? ",\n" : ", "));
			printf("%#08x", buf[i]);
			first = 1;
		}
		
		content_len += rd / sizeof(uint32_t);
	}
	printf("\nsize = %d\n", content_len);
	return 0;
}