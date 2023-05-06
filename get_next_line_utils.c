/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouvier <ebouvier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 17:10:07 by ebouvier          #+#    #+#             */
/*   Updated: 2023/05/06 16:27:21 by ebouvier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

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
	new->eof = 0;
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
		return (new_node);
	while (tail->next)
		tail = tail->next;
	tail->next = new_node;
	return (new_node);
}

void lst_free(t_list **head) 
{
    t_list *current;
    t_list *next;

    current = *head;
    while (current) 
    {
        next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
    *head = NULL;
}
