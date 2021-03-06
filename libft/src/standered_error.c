/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   standered_error.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arraji <arraji@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/07 16:15:10 by arraji            #+#    #+#             */
/*   Updated: 2021/03/07 16:15:29 by arraji           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	standered_error(void)
{
	ft_fprintf(2, "%s\n", strerror(errno));
	exit(errno);
}
