/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouvier <ebouvier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 17:09:47 by ebouvier          #+#    #+#             */
/*   Updated: 2023/05/03 15:33:25 by ebouvier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

#include <stdio.h>


size_t	ft_strlen(char *s)
{
	char	*cpy;

	cpy = s;
	while (*cpy)
		cpy++;
	return (cpy - s);
}

char	*ft_strdup(char *s)
{
	char	*new;
	size_t	i;

	new = malloc((ft_strlen(s) + 1) * sizeof(char));
	if (!new)
		return (NULL);
	i = 0;
	while (*s)
		new[i++] = *s++;
	new[i] = 0;
	return (new);
}

char	*ft_strchr(char *s, char c)
{
	while (*s)
	{
		if (*s == c)
			return (s);
		s++;
	}
	return (NULL);
}

size_t	lst_len(t_list *head)
{
	size_t	i;

	i = 0;
	while (head)
	{
		i++;
		head = head->next;
	}
	return (i);
}

t_list	*lst_new(char *content)
{
	t_list	*new;

	new = malloc(sizeof(t_list));
	if (!new)
		return (NULL);
	new->next = NULL;
	new->data = ft_strdup(content);
	return (new);
}

t_list	*lst_append(t_list **head, char *content)
{
	t_list	*tail;
	t_list	*new_node;

	tail = *head;
	new_node = lst_new(content);
	if (!new_node)
		return (NULL);
	if (!*head)
	{
		*head = new_node;
		return (*head);
	}
	while (tail->next)
		tail = tail->next;
	tail->next = new_node;
	return (*head);
}

void	lst_clear(t_list *head)
{
	t_list	*current;
	t_list	*tail;

	current = head;
	tail = NULL;
	while (current)
	{
		tail = current;
		current = current->next;
		free(tail->data);
		free(tail);
	}
}

void	lst_print(t_list *head)
{
	t_list	*current;

	current = head;
	while (current)
	{
		printf("value=%s\n", current->data);
		printf("len=%ld\n", ft_strlen(current->data));
		current = current->next;
	}
}

size_t	check_eol(t_list *head)
{
	size_t	i;

	i = 0;
	while (head)
	{
		while (*head->content)
		{
			if (*head->content == '\n')
				return (i);
			head->content++;
			i++;
		}
		head = head->next;
	}
	return (0);
}

void	lst_to_line(char **line, t_list *head)
{
	size_t	chars_to_eol;

	chars_to_eol = 0;
	while (*head)
	{
		*line = malloc();
		head = head->next;
	}
}

char	*get_next_line(int fd)
{
	int				ret;
	char			buffer[BUFFER_SIZE + 1];
	static t_list	*head;
	char			*line;
	size_t			char_count;

	head = NULL;
	printf("=========================\n\n");
	while ((ret = read(fd, buffer, BUFFER_SIZE)) > 0)
	{
		buffer[ret] = 0;
		lst_append(&head, buffer);
		// printf("head addr=%p\n", head);
		// printf("ret value=%d\n", ret);
		lst_print(head);
		if (check_eol(buffer))
		{
			printf("has EOL\n");
			while (head)
		}
	}
	printf("lst_len=%ld\n", lst_len(head));
	lst_clear(head);
	printf("=========================\n\n");
	return (NULL);
}
