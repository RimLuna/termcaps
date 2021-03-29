#include "minishell.h"

t_hlist		*dlist_push_back(t_hlist **lst, t_hlist *new)
{
	t_hlist	*save;

	if (!new || !lst)
		return (NULL);
	new->next = NULL;
	if (!*lst)
	{
		new->prev = NULL;
		*lst = new;
	}
	else
	{
		save = *lst;
		while (save->next)
			save = save->next;
		save->next = new;
		new->prev = save;
	}
	return (new);
}

void	del(int size)
{
	while (size-- > 0)
		write(1, "\b \b", 3);
}
