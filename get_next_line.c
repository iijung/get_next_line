/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minjungk <minjungk@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/21 05:46:00 by minjungk          #+#    #+#             */
/*   Updated: 2022/08/21 10:43:29 by minjungk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*join(char *dst, char *src, size_t slen, int new_line)
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
	tmp = (char *)ft_calloc(dlen + slen + new_line + 1, sizeof(char));
	if (tmp == 0)
		return (0);
	ft_memcpy(tmp, dst, dlen);
	ft_memcpy(tmp + dlen, src, slen);
	if (new_line)
		tmp[dlen + slen] = '\n';
	free(dst);
	ft_memmove(src, src + slen + new_line, BUFFER_SIZE - slen - new_line);
	ft_memset(src + BUFFER_SIZE - slen - new_line, 0, slen + new_line);
	return (tmp);
}

char	*get_next_line(int fd)
{
	static char	read_buf[BUFFER_SIZE + 1];
	ssize_t		read_len;
	ssize_t		idx;
	char		*rtn;

	rtn = 0;
	idx = 0;
	read_len = BUFFER_SIZE;
	while (fd >= 0)
	{
		if (idx == BUFFER_SIZE)
		{
			rtn = join(rtn, read_buf, ft_strlen(read_buf), 0);
			read_len = read(fd, read_buf, BUFFER_SIZE);
			if (read_len < 0)
				break ;
			idx = 0;
		}
		if (idx == read_len || read_buf[idx] == '\n')
			return (join(rtn, read_buf, idx, read_buf[idx] == '\n'));
		++idx;
	}
	free(rtn);
	return (0);
}
