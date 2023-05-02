/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouvier <ebouvier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 17:09:47 by ebouvier          #+#    #+#             */
/*   Updated: 2023/05/02 22:36:15 by ebouvier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*seek_eol(char *buffer)
{
	while (*buffer)
	{
		if (*buffer == '\n')
			return (*buffer);
	}
	return (NULL);
}

char	*strjoinfree(char *s1, char *s2)
{
	
}

char	*get_next_line(int fd)
{
	int			ret;
	char		buffer[BUFFER_SIZE + 1];
	static char	*stock;
	char		*line;
	char		*eol;

	ret = read(fd, buffer, BUFFER_SIZE);
	stock = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!stock)
		return (NULL);
	while (ret > 0)
	{
		buffer[ret] = 0;
		stock = strjoinfree(stock, buffer);
		if (!stock)
			return (NULL); // free and return
		eol = seek_eol(stock);
		if (eol)
		{
			strjoinfree(line, stock); // strsub or strjoinfree + strshift
			return (line);
		}
		ret = read(fd, buffer, BUFFER_SIZE);
	}
	return (NULL);
}
