/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouvier <ebouvier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 17:09:47 by ebouvier          #+#    #+#             */
/*   Updated: 2023/05/06 00:57:30 by ebouvier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>
#include <strings.h>

char *strjoin(char *s1, char *s2)
{
	char *new;
	int total_size = strlen(s1) + strlen(s2);

	new = malloc(sizeof(char) * (total_size + 1));
	if (!new)
		return (NULL);
	bzero(new, total_size + 1);
	strcat(new, s1);
	strcat(new, s2);
	free(s1);
	return (new);
}

char *get_line(char *stc)
{
	char *line;
	size_t line_len;

	if (!*stc)
		return (NULL);
	if (strchr(stc, '\n'))
		line_len = strchr(stc, '\n') - stc + 1;
	else
		line_len = strlen(stc);
	line = malloc(sizeof(char) * (line_len + 1));
	if (!line)
		return NULL;
	line[line_len] = 0;
	memmove(line, stc, line_len);
	return (line);
}

char *clean_stc(char *stc) 
{
	char *new;
	int i;
	int j;

	i = 0;
	while (stc[i] && stc[i] != '\n')
		i++;
	if (!stc[i])
		return (free(stc), NULL);
	new = malloc(sizeof(char) * strlen(stc) - i);
	if (!new)
		return (NULL);
	j = i + 1;
	while (stc[++i])
		new[i - j] = stc[i];
	new[i - j] = 0;
	free(stc);
    return new;
}

char *read_to_buff(int fd, char *stc)
{
	char	*buffer;
	int		bytescopy;

	bytescopy = 1;
	buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	while (!strchr(stc, '\n') && bytescopy)
	{
		bytescopy = read(fd, buffer, BUFFER_SIZE);
		if (bytescopy == -1)
			return (free(buffer), NULL);
		buffer[bytescopy] = '\0';
		stc = strjoin(stc, buffer);
	}
	free(buffer);
	return (stc);
}

char *get_next_line(int fd)
{
	static  char *stc;
	char *line;

	if (fd < 0 || BUFFER_SIZE < 1 || read(fd, &line, 0) < 0)
		return (NULL);
	stc = read_to_buff(fd, stc);
	if (!stc)
		return (NULL);
	line = get_line(stc);
	if (!line)
		return (free(stc), NULL);
	stc = clean_stc(stc);
	return (line);
}