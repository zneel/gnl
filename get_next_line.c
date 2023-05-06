/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouvier <ebouvier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 17:09:47 by ebouvier          #+#    #+#             */
/*   Updated: 2023/05/06 11:11:57 by ebouvier         ###   ########.fr       */
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
		printf(" data=%.*s ", (int)head->read, head->data);
		printf(" read=%li ", head->read);
		printf(" eol=%li ", head->eol);
		printf(" eol_found=%d\n", head->eol_found);
		head = head->next;
	}
}

ssize_t line_size(t_list *head)
{
	t_list	    *current;
	ssize_t     total;

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
    ssize_t line_len = line_size(head);
    size_t copied = 0;
    char *line;

    line = malloc(sizeof(char) * (line_len + 1));
    if (!line)
        return (NULL);
    line[line_len] = 0;
    while (head)
    {
        if (!head->eof)
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
                break ;
            }
        }
        head = head->next;
    }
    return line;
}

t_list	*read_to_lst(t_list *head, int fd)
{
	t_list	*current;

    if (!head)
        head = lst_append(head);
    if (!head)
        return (NULL);
    current = head;
	while (!current->eol_found || !current->eof)
	{
        current->data = malloc(sizeof(char) * BUFFER_SIZE);
		if (!current->data)
			return (NULL);
        current->read = read(fd, current->data, BUFFER_SIZE);
		if (current->read <= 0)
		{
            current->eof = 1;
			break;
		}
		if (memchr(current->data, '\n', current->read))
		{
            current->eol = (char *)memchr(current->data, '\n', current->read) - current->data;
            current->eol_found = 1;
		}
        current = lst_append(current);
	}
	return (head);
}

t_list	*lst_shift(t_list *head)
{
	t_list	*new;

    if (head == NULL)
        return (NULL);
    new = lst_new();
	if (new == NULL)
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
	static t_list	*head = NULL;
	char 			*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, &line, 0) == -1)
		return (NULL);
    if (head && head->eof)
    {
        free(head->data);
        free(head);
        return (NULL);
    }
	head = read_to_lst(head, fd);
	if (!head)
		return (NULL);
	line = lst_to_line(head);
    if (!head->eof)
	    head = lst_shift(head);
	return line;
}
