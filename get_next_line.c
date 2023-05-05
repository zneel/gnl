/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouvier <ebouvier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 17:09:47 by ebouvier          #+#    #+#             */
/*   Updated: 2023/05/05 21:11:50 by ebouvier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>
#include <strings.h>

void	lst_print(t_list *head)
{
	while (head)
	{
		printf(" \ncurrentPtr=%p ", head);
		printf(" nextPtr=%p ", head->next);
		printf(" data=%.*s ", head->read, head->data);
		printf(" read=%d ", head->read);
		printf(" eol=%d ", head->eol);
		printf(" eol_found=%d\n", head->eol_found);
		head = head->next;
	}
}

int	line_size(t_list *head)
{
	t_list	*current;
	int		total;

	current = head;
	total = 0;
	while (current)
	{
		if (current->read > 0)
			total += current->read;
		current = current->next;
	}
	return (total);
}

char* lst_to_line(t_list *head)
{
    size_t line_len = line_size(head);
    size_t copied = 0;
    char *line;

    line = malloc(sizeof(char) * (line_len + 1));
    if (!line)
        return NULL;
    bzero(line, line_len + 1);
    while (head)
    {
        if (!head->eol_found)
        {
            memcpy(line + copied, head->data, head->read);
            copied += head->read;
        }
        else
        {
			memcpy(line + copied, head->data, head->eol + 1);
			copied += head->eol + 1;
        }
        head = head->next;
    }
    return line;
}

t_list	*read_to_lst(t_list *head, int fd)
{
	t_list	*curr;

	if (!head)
		head = lst_append(head);
	curr = head;
	while (!curr->eof)
	{
		curr->data = malloc(sizeof(char) * BUFFER_SIZE);
		if (!curr->data)
			return (NULL);
		curr->read = read(fd, curr->data, BUFFER_SIZE);
		if (curr->read < 1)
		{
			free(curr->data);
			free(curr);
			curr->eof = 1;
			break;
		}
		if (memchr(curr->data, '\n', curr->read))
		{
			curr->eol = (char *)memchr(curr->data, '\n', curr->read) - curr->data;
			curr->eol_found = 1;
			break ;
		}
		// curr = lst_append(head);
	}
	return (head);
}

t_list	*lst_shift(t_list *head)
{
	t_list	*new;

	new = lst_new();
	if (head == NULL || new == NULL)
		return (NULL);
	while (head && head->next)
		head = head->next;
	new->data = malloc(sizeof(char) * (head->read - (head->eol + 1)));
	if (new->data == NULL)
		return (NULL);
	memcpy(new->data, head->data + head->eol + 1, head->read - (head->eol + 1));
	new->read = head->read - (head->eol + 1);
	lst_free(&head);
	return(new);
}

char	*get_next_line(int fd)
{
	static t_list	*head;
	char 			*line;

	if (fd < 0 || BUFFER_SIZE < 1)
		return (NULL);
	head = read_to_lst(head, fd);
	if (!head)
		return (NULL);
	if (head->eof)
	{
		lst_free(&head);
		return (NULL);
	}
	line = lst_to_line(head);
	head = lst_shift(head);
	return line;
}
