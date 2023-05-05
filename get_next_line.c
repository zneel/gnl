/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouvier <ebouvier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 17:09:47 by ebouvier          #+#    #+#             */
/*   Updated: 2023/05/05 18:12:09 by ebouvier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>

int	lst_len(t_list *head)
{
	int	i;

	i = 0;
	while (head)
	{
		i++;
		head = head->next;
	}
	return (i);
}

t_list	*lst_new(void)
{
	t_list	*new;

	new = malloc(sizeof(t_list));
	if (!new)
		return (NULL);
	new->next = NULL;
	new->data = NULL;
	new->read = 0;
	new->eol = 0;
	new->eol_found = 0;
	return (new);
}

t_list	*lst_append(t_list *head)
{
	t_list	*tail;
	t_list	*new_node;

	tail = head;
	new_node = lst_new();
	if (!new_node)
		return (NULL);
	if (!head)
	{
		head = new_node;
		return (head);
	}
	while (tail->next)
		tail = tail->next;
	tail->next = new_node;
	return (new_node);
}

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

char*	lst_to_line(t_list **head)
{
	size_t line_len = line_size(*head);
	size_t copied = 0;
	t_list *curr = *head;
	t_list *prev = *head;
	char *line;

	line = malloc(sizeof(char) * (line_len + 1));
	if (!line)
		return NULL;
	line[line_len] = 0;
	while (curr)
	{
		if (!curr->eol_found)
		{
			memcpy(line + copied, curr->data, curr->read);
			copied += curr->read;
		}
		else
		{
			if (curr->eol + 1 < curr->read)
			{
				*head = lst_new();
				(*head)->data = malloc(sizeof(char) * (curr->read - curr->eol + 1));
				memcpy((*head)->data, curr->data+curr->eol, curr->read-curr->eol);
				return NULL;
			}
		}
		prev = curr;
		curr = curr->next;
		free(prev->data);
		free(prev);
	}
	*head = NULL;
	return line;
}

t_list	*read_to_lst(t_list *head, int fd)
{
	t_list	*curr;

	if (!head)
		head = lst_append(head);
	curr = head;
	while (1)
	{
		curr->data = malloc(sizeof(char) * BUFFER_SIZE);
		if (!curr->data)
			return (NULL);
		curr->read = read(fd, curr->data, BUFFER_SIZE);
		if (curr->read < 1)
		{
			free(curr->data);
			free(curr);
			return (NULL);
		}
		if (memchr(curr->data, '\n', curr->read))
		{
			curr->eol = (char *)memchr(curr->data, '\n', curr->read) - curr->data;
			curr->eol_found = 1;
			break ;
		}
		curr = lst_append(head);
	}
	return (head);
}

char	*get_next_line(int fd)
{
	static t_list	*head;

	if (fd < 0 || BUFFER_SIZE < 1)
		return (NULL);
	head = read_to_lst(head, fd);
	if (!head)
		return (NULL);
	lst_print(head);
	return (lst_to_line(&head));
}
