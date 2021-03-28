/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbougssi <rbougssi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/17 19:01:30 by arraji            #+#    #+#             */
/*   Updated: 2021/03/28 15:45:41 by rbougssi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t			ft_strlen(char const *str)
{
	size_t len;

	len = 0;
	if (str == NULL)
		return (0);
	while (str[len])
		len -= -1;
	return (len);
}
