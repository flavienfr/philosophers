/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/20 22:23:34 by user42            #+#    #+#             */
/*   Updated: 2020/06/20 22:23:37 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int			ft_atoi(const char *str)
{
	int i;
	int out;
	int is_minus;

	is_minus = 0;
	i = 0;
	out = 0;
	while (str[i] == ' ' || str[i] == '\f' || str[i] == '\n'
			|| str[i] == '\r' || str[i] == '\t' || str[i] == '\v')
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			is_minus++;
		i++;
	}
	while (str[i] != '\0' && str[i] >= '0' && str[i] <= '9')
	{
		out = out * 10 + (str[i] - 48);
		i++;
	}
	if (is_minus)
		out *= -1;
	return (out);
}

void		ft_putstr(char *s)
{
	int	i;

	if (!s)
		return ;
	i = -1;
	while (s[++i])
		write(1, &s[i], 1);
}

void		*ft_calloc(size_t count, size_t size)
{
	unsigned char	*ptr;
	size_t			i;

	i = 0;
	size *= count;
	if (!(ptr = (unsigned char *)malloc(size)))
		return (0);
	while (i < size)
		ptr[i++] = '\0';
	return (ptr);
}

static int	len(long int n)
{
	int len;

	len = 0;
	if (n < 0)
	{
		n *= -1;
		len++;
	}
	else if (n == 0)
		return (1);
	while (n)
	{
		n = n / 10;
		len++;
	}
	return (len);
}

char		*ft_itoa(int n)
{
	long int	i;
	long int	nn;
	char		*s;

	nn = n;
	i = len(nn);
	if (!(s = ft_calloc(i + 1, sizeof(*s))))
		return (0);
	i--;
	if (nn == 0)
		s[0] = '0';
	if (nn < 0)
	{
		s[0] = '-';
		nn *= -1;
	}
	while (nn > 0)
	{
		s[i] = (nn % 10) + 48;
		nn = nn / 10;
		i--;
	}
	return (s);
}
