/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouvier <ebouvier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 17:09:47 by ebouvier          #+#    #+#             */
/*   Updated: 2023/05/04 17:43:09 by ebouvier         ###   ########.fr       */
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

t_list	*lst_new(char *content)
{
	t_list	*new;

	new = malloc(sizeof(t_list));
	if (!new)
		return (NULL);
	new->next = NULL;
	new->data = ft_strdup(content);
	new->eol = -1;
	if (new->data && strchr(new->data, '\n'))
		new->eol = strchr(new->data, '\n') - new->data;
	if (new->data)
		new->d_len = ft_strlen(new->data);
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

void	lst_print(t_list *head)
{
	while (head)
	{
		printf("value=%s\n", head->data);
		printf("len=%d\n", head->d_len);
		printf("eol=%d\n", head->eol);
		head = head->next;
	}
}

int	lst_check_eol(t_list *head)
{
	int	chars_to_eol;

	chars_to_eol = -1;
	while (head)
	{
		if (head->eol >= 0)
		{
			chars_to_eol += head->eol;
			return (chars_to_eol);
		}
		chars_to_eol += head->d_len;
		head = head->next;
	}
	return (-1);
}

int		lst_data_len(t_list *head)
{
	int	i = 0;
	while (head)
	{
		i += head->d_len;
		head = head->next;
	}
	return (i);
}

char	*ft_substr(char *s,  int start, int len)
{
	char	*new;
	int	s_len;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (start >= s_len)
		start = s_len;
	if (len > s_len - start)
		len = s_len - start;
	new = malloc((len + 1) * sizeof(char));
	if (!new)
		return (NULL);
	memcpy(new, s + start, len);
	new[len] = 0;
	return (new);
}


char	*lst_to_line(t_list **head, int line_len)
{
	char	*line;
	t_list	*tail;

	if (line_len < 0)
		line_len = lst_data_len(*head);
	line = malloc(sizeof(char) * (line_len + 1));
	if (!line)
		return (NULL);
	line[0] = 0;
	tail = NULL;
	while (*head)
	{
		tail = *head;
		int sz = (*head)->eol >= 0 ? (*head)->eol + 1 : (*head)->d_len;
		line = ft_strncat(line, (*head)->data, sz);
		if ((*head)->eol >= 0 && ((*head)->d_len >= (*head)->eol))
		{
			char *tmp = ft_substr((*head)->data, (*head)->eol + 1, (*head)->d_len - (*head)->eol);
			*head = lst_new(tmp);
			free(tail->data);
			free(tail);
			free(tmp);
			break;
		}
		*head = (*head)->next;
		free(tail->data);
		free(tail);
	}
	return (line);
}

char	*get_next_line(int fd)
{
	int				ret;
	char			*buffer;
	static t_list	*head;
	int				eol;

	// printf("\n\n=========================\n\n");
	if (fd < 0)
		return (NULL);
	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	while ((ret = read(fd, buffer, BUFFER_SIZE)) > 0)
	{
		// printf("lst_len%d\n", lst_len(head));
		buffer[ret] = 0;
		if (head)
		{
			eol = lst_check_eol(head);
			if (eol >= 0)
			{
				free(buffer);
				// printf("!has EOL %d!\n", eol + 1);
				return (lst_to_line(&head, eol));
			}
		}
		lst_append(&head, buffer);
		// lst_print(head);

	}
	if (ret == 0 && lst_len(head) > 0)
	{
		free(buffer);
		return (lst_to_line(&head, -1));
	}
	free(buffer);
	// printf("ret value after loop = %d\n", ret);
	// printf("\n\n=========================\n\n");
	return (NULL);
}
