/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minjungk <minjungk@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/21 05:46:00 by minjungk          #+#    #+#             */
/*   Updated: 2022/08/22 04:02:23 by minjungk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static t_stream	*search_stream(t_stream **head, int fd)
{
	t_stream	*curr;
	t_stream	*new;

	if (head == 0)
		return (0);
	curr = *head;
	while (curr)
	{
		if (curr->fd == fd)
			return (curr);
		if (curr->next == 0)
			break ;
		curr = curr->next;
	}
	new = ft_calloc(1, sizeof(t_stream));
	if (new == 0)
		return (0);
	new->fd = fd;
	if (curr)
		curr->next = new;
	else
		*head = new;
	return (new);
}

static void	clear_stream(t_stream **head, int fd)
{
	t_stream	*prev;
	t_stream	*curr;
	t_stream	*next;

	if (head == 0)
		return ;
	prev = 0;
	curr = *head;
	while (curr)
	{
		if (curr->fd == fd)
		{
			next = curr->next;
			free(curr);
			if (prev)
				prev->next = next;
			else
				*head = next;
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

static char	*join(char *dst, char *src, size_t slen, int *err)
{
	char	*tmp;
	size_t	dlen;

	if (src == 0 || src[0] == 0)
		return (dst);
	dlen = 0;
	if (dst)
		dlen = ft_strlen(dst);
	if (slen > ft_strlen(src))
		slen = ft_strlen(src);
	tmp = (char *)ft_calloc(dlen + slen + 1, sizeof(char));
	if (tmp == 0)
	{
		*err = 1;
		free(dst);
		return (0);
	}
	ft_memcpy(tmp, dst, dlen);
	ft_memcpy(tmp + dlen, src, slen);
	free(dst);
	ft_memmove(src, src + slen, BUFFER_SIZE - slen);
	ft_memset(src + BUFFER_SIZE - slen, 0, slen);
	return (tmp);
}

static char	*get_line(t_stream *curr, int *close)
{
	ssize_t	idx;
	char	*rtn;

	if (curr == 0 || close == 0)
		return (0);
	rtn = 0;
	idx = -1;
	curr->read_len = BUFFER_SIZE;
	while (1)
	{
		if (++idx == BUFFER_SIZE)
		{
			rtn = join(rtn, curr->read_buf, ft_strlen(curr->read_buf), close);
			curr->read_len = read(curr->fd, curr->read_buf, BUFFER_SIZE);
			if (curr->read_len < 0 || *close)
				break ;
			if (curr->read_len == 0)
				*close = 2;
			idx = 0;
		}
		if (idx == curr->read_len || curr->read_buf[idx] == '\n')
			return (join(rtn, curr->read_buf, idx + 1, close));
	}
	free(rtn);
	return (0);
}

char	*get_next_line(int fd)
{
	static t_stream	*stream;
	t_stream		*curr;
	char			*rtn;
	int				close;

	if (fd < 0)
		return (0);
	curr = search_stream(&stream, fd);
	if (curr == 0)
		return (0);
	close = 0;
	rtn = get_line(curr, &close);
	if (rtn == 0 || close)
		clear_stream(&stream, fd);
	if (rtn && (close == 0 || close == 2))
		return (rtn);
	free(rtn);
	return (0);
}
