/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: swquinc <swquinc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/15 09:52:43 by hovalygta         #+#    #+#             */
/*   Updated: 2020/10/02 22:45:43 by swquinc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	error(char **need_free)
{
	free(*need_free);
	return (-1);
}

static char	*cleaner(char *need_free, char *reallocate)
{
	char		*new;

	new = ft_strdup(reallocate);
	free(need_free);
	need_free = NULL;
	return (new);
}

static char	*remainder_reader(char **remainder, char **n)
{
	char		*ptr_to_n;
	char		*line;

	ptr_to_n = NULL;
	line = ft_line_calloc(0, 0);
	if (*remainder)
	{
		if ((ptr_to_n = ft_strchr(*remainder, '\n')))
		{
			*ptr_to_n = '\0';
			line = cleaner(line, *remainder);
			*remainder = cleaner(*remainder, ++ptr_to_n);
			*n = "Exist";
		}
		else
		{
			line = cleaner(line, *remainder);
			free(*remainder);
			*remainder = NULL;
		}
	}
	return (line);
}

static int	reader(int fd, char *buffer, char **line)
{
	size_t			i;
	char			*n;
	static char		*remainder;

	n = NULL;
	*line = remainder_reader(&remainder, &n);
	while (n == NULL && (i = read(fd, buffer, BUFFER_SIZE)))
	{
		buffer[i] = '\0';
		if ((n = ft_strchr(buffer, '\n')) == NULL)
		{
			*line = ft_strjoin_free(*line, buffer);
		}
		else
		{
			*n = '\0';
			*line = ft_strjoin_free(*line, buffer);
			n++;
			if (*n != '\0')
				remainder = ft_strdup(n);
		}
	}
	if (*line == NULL)
		return (error(&remainder));
	return (n == NULL && i == 0 && remainder == NULL) ? 0 : 1;
}

int			get_next_line(int fd, char **line)
{
	char			*buffer;
	int				i;
	static int		b;

	if (fd < 0 || read(fd, NULL, 0) || BUFFER_SIZE <= 0 || !line)
		return (-1);
	if ((buffer = ft_line_calloc(BUFFER_SIZE, sizeof(char))) == NULL)
		return (-1);
	i = reader(fd, buffer, line);
	free(buffer);
	buffer = NULL;
	if (i > 0)
		b = 0;
	if (i == 0 && b == 0)
	{
		b = 1;
		i = 1;
	}
	return (i);
}
