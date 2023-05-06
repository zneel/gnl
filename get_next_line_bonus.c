/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouvier <ebouvier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 18:25:41 by ebouvier          #+#    #+#             */
/*   Updated: 2023/05/06 19:20:08 by ebouvier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

ssize_t	line_size(t_list *head)
{
	ssize_t	total;

	total = 0;
	while (head)
	{
		if (head->eol_found)
		{
			total += head->eol + 1;
			break ;
		}
		if (head->read > 0)
			total += head->read;
		head = head->next;
	}
	return (total);
}

char	*lst_to_line(t_list *head)
{
	ssize_t	line_len;
	size_t	copied;
	char	*line;

	line_len = line_size(head);
	line = malloc(sizeof(char) * (line_len + 1));
	if (!line)
		return (NULL);
	line[line_len] = 0;
	copied = 0;
	while (head)
	{
		if (!head->eof && !head->eol_found)
		{
			ft_memcpy(line + copied, head->data, head->read);
			copied += head->read;
		}
		else if (head->eol_found)
		{
			ft_memcpy(line + copied, head->data, head->eol + 1);
			break ;
		}
		head = head->next;
	}
	return (line);
}

t_list	*read_to_lst(t_list *head, int fd)
{
	t_list	*c;

	if (!head)
		return (NULL);
	c = head;
	while (1)
	{
		if (c->read == 0 && c->eof == 0)
		{
			c->data = malloc(sizeof(char) * BUFFER_SIZE);
			if (!c->data)
				return (NULL);
			c->read = read(fd, c->data, BUFFER_SIZE);
			c->eof = c->read <= 0;
		}
		if (c->eof || c->eol_found)
			break ;
		c->eol_found = ft_memchr(c->data, '\n', c->read) != 0;
		if (c->eol_found)
			c->eol = (char *)ft_memchr(c->data, '\n', c->read) - c->data;
		else
			c = lst_append(head);
	}
	return (head);
}

t_list	*lst_shift(t_list *head)
{
	t_list	*new;
	t_list	*tail;

	tail = head;
	while (tail->next)
		tail = tail->next;
	new = lst_new();
	if (new == NULL)
		return (NULL);
	new->eof = tail->eof;
	if (!tail->eof && tail->read - (tail->eol + 1) != 0)
	{
		new->read = tail->read - (tail->eol + 1);
		new->data = malloc(sizeof(char) * new->read);
		if (!new->data)
			return (NULL);
		ft_memcpy(new->data, tail->data + tail->eol + 1, new->read);
		new->eol = (ssize_t)(char *)ft_memchr(new->data, '\n', new->read);
		if (new->eol)
		{
			new->eol = (ssize_t)((char *)new->eol - new->data);
			new->eol_found = 1;
		}
	}
	lst_free(&head);
	return (new);
}

char	*get_next_line(int fd)
{
	static t_list	*head[OPEN_MAX];
	char			*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (read(fd, &line, 0) == -1)
		return (NULL);
	if (!head[fd])
		head[fd] = lst_new();
	head[fd] = read_to_lst(head[fd], fd);
	if (!head[fd])
		return (NULL);
	if (head[fd]->eof)
		return (lst_free(&head[fd]), NULL);
	line = lst_to_line(head[fd]);
	head[fd] = lst_shift(head[fd]);
	return (line);
}