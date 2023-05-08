/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouvier <ebouvier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 18:25:41 by ebouvier          #+#    #+#             */
/*   Updated: 2023/05/08 14:47:06 by ebouvier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

ssize_t	line_size(t_list *head)
{
	t_list	*current;
	ssize_t	total;

	current = head;
	total = 0;
	while (current)
	{
		if (current->eol_found)
		{
			total += current->eol + 1;
			break ;
		}
		if (current->read > 0)
			total += current->read;
		current = current->next;
	}
	return (total);
}

/*
* Takes a linked list as input and calculate the line length
* Iterate over the linked list and copy the data to the line buffer
* Stop copy after an EOL is found or EOF is reached.
*/

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

/*
* Iterate the linked list node and read data from the fd
* to the node data buffer
* Continue reading and appending until EOL or EOF is found.
*/

t_list	*read_to_lst(t_list *head, int fd)
{
	t_list	*c;

	c = head;
	while (1 && c)
	{
		if (c->read == 0 && c->eof == 0)
		{
			c->data = malloc(sizeof(char) * BUFFER_SIZE);
			if (!c->data)
				return (lst_free(&head), NULL);
			c->read = read(fd, c->data, BUFFER_SIZE);
			c->eof = c->read <= 0;
		}
		if (c->eof || c->eol_found)
			break ;
		c->eol_found = ft_memchr(c->data, '\n', c->read) != 0;
		if (c->eol_found)
			c->eol = (char *)ft_memchr(c->data, '\n', c->read) - c->data;
		else
			c = lst_append(c);
	}
	return (head);
}

/*
* Goes to the last element of the list
* Create a new node
* Copy the remaining data after the EOL char
* Free the linked list
* Return new head
*/

t_list	*lst_shift(t_list *head)
{
	t_list	*new;
	t_list	*tail;

	tail = head;
	while (tail->next)
		tail = tail->next;
	new = lst_new();
	if (!new)
		return (lst_free(&head), NULL);
	new->eof = tail->eof;
	if (!tail->eof && tail->read - (tail->eol + 1) != 0)
	{
		new->read = tail->read - (tail->eol + 1);
		new->data = malloc(sizeof(char) * new->read);
		if (!new->data)
			return (lst_free(&head), NULL);
		ft_memcpy(new->data, tail->data + tail->eol + 1, new->read);
	}
	return (lst_free(&head), new);
}

char	*get_next_line(int fd)
{
	static t_list	*head[MAX_OPEN];
	char			*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (lst_free(&head[fd]), NULL);
	if (!head[fd])
	{
		head[fd] = lst_new();
		if (!head[fd])
			return (NULL);
	}
	head[fd] = read_to_lst(head[fd], fd);
	if (!head[fd])
		return (NULL);
	if (head[fd]->eof)
		return (lst_free(&head[fd]), NULL);
	line = lst_to_line(head[fd]);
	head[fd] = lst_shift(head[fd]);
	return (line);
}
