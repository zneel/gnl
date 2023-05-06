/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouvier <ebouvier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 17:09:47 by ebouvier          #+#    #+#             */
/*   Updated: 2023/05/06 17:01:12 by ebouvier         ###   ########.fr       */
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
        if (current->eol_found)
        {
            total += current->eol + 1;
            break;
        }
        if (current->read > 0)
            total += current->read;
        current = current->next;
    }
    return (total);
}

char* lst_to_line(t_list *head)
{
    ssize_t line_len;
    size_t copied;;
    char *line;

    line_len = line_size(head);
    copied = 0;
    line = malloc(sizeof(char) * (line_len + 1));
    if (!line)
        return (NULL);
    line[line_len] = 0;
    while (head)
    {
        if (!head->eof && !head->eol_found)
        {
            memcpy(line + copied, head->data, head->read);
            copied += head->read;
        }
        else if(head->eol_found)
        {
            memcpy(line + copied, head->data, head->eol + 1);
            break ;
        }
        head = head->next;
    }
    return line;
}

t_list *read_to_lst(t_list *head, int fd)
{
    t_list *current;

    if (!head)
        return (NULL);
    current = head;
    if (current->read == 0 && current->eof == 0)
        current = lst_append(current);
    while (!current->eol_found && !current->eof)
    {
        if (current->read == 0 && current->eof == 0)
        {
            current->data = malloc(sizeof(char) * BUFFER_SIZE);
            if (!current->data)
                return (NULL);
            current->read = read(fd, current->data, BUFFER_SIZE);
            current->eof = current->read <= 0;
        }
        if (!current->eof)
        {
            current->eol_found = memchr(current->data, '\n', current->read) != 0;
            if (current->eol_found)
                current->eol = (char*)memchr(current->data, '\n', current->read) - current->data;
            else
                current = lst_append(head);
        }
    }
    return (head);
}


t_list *lst_shift(t_list *head)
{
    t_list *new;
    t_list *tail;

    if (head == NULL)
        return (NULL);
    tail = head;
    while (tail->next)
        tail = tail->next;
    new = lst_new();
    if (new == NULL)
        return (NULL);
    new->eof = tail->eof;
    if (!tail->eof)
    {
        new->read = tail->read - (tail->eol + 1);
        new->data = malloc(sizeof(char) * new->read);
        if (!new->data)
            return (NULL);
        memcpy(new->data, tail->data + tail->eol + 1, new->read);
        new->eol = (ssize_t)(char *)memchr(new->data, '\n', new->read);
        if (new->eol)
        {
            new->eol = (ssize_t)((char*)new->eol - new->data);
            new->eol_found = 1;
        }
    }
    lst_free(&head);
    return (new);
}


char	*get_next_line(int fd)
{
    static t_list	*head;
    char 			*line;

    if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, &line, 0) == -1)
        return (NULL);
    if (!head)
        head = lst_new();
    head = read_to_lst(head, fd);
    if (head && head->eof)
        return (lst_free(&head), NULL);
    line = lst_to_line(head);
    head = lst_shift(head);
    return line;
}
