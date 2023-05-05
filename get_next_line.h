/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouvier <ebouvier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 17:06:33 by ebouvier          #+#    #+#             */
/*   Updated: 2023/05/05 20:47:54 by ebouvier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE	10
# endif

# include <stdlib.h>
# include <unistd.h>
# include <string.h>


typedef struct s_list
{
	struct s_list	*next;
	char			*data;
	int				read;
	int				eol;
	int				eol_found;
	int				eof;
}				t_list;
char	*get_next_line(int fd);
t_list	*lst_new(void);
t_list	*lst_append(t_list *head);
void lst_free(t_list **head);
#endif