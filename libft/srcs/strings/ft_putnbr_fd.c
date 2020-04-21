/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/13 16:24:21 by llelievr          #+#    #+#             */
/*   Updated: 2018/11/15 17:22:15 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void		ft_putnbr_fd(int n, int fd)
{
	char	out[10];
	size_t	i;

	if (n == 0)
		ft_putchar_fd('0', fd);
	if (n < 0)
		ft_putchar_fd('-', fd);
	i = 0;
	n *= (n < 0 ? 1 : -1);
	while (n < 0)
	{
		out[i++] = (n % 10) * -1 + '0';
		n /= 10;
	}
	while (i-- > 0)
		ft_putchar_fd(out[i], fd);
}
