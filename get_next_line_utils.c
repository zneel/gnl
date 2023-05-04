/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouvier <ebouvier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 17:10:07 by ebouvier          #+#    #+#             */
/*   Updated: 2023/05/04 13:37:06 by ebouvier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char *ft_strncat(char *dst, char *src, int n)
{
	int l_dst;

	l_dst = ft_strlen(dst);
	while (*src && n--)
		dst[l_dst++] = *src++;
	dst[l_dst] = 0;
	return (dst);
}

int	ft_strlen(char *s)
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
	int	i;

	new = malloc((ft_strlen(s) + 1) * sizeof(char));
	if (!new)
		return (NULL);
	i = 0;
	while (*s)
		new[i++] = *s++;
	new[i] = 0;
	return (new);
}