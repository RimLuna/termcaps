#include <libc.h>
#include "libft/includes/libft.h"
typedef	struct	s_dlist
{
	struct s_dlist	*next;
	struct s_dlist	*prev;
	char			*cmd;
	char			*updated;
}				t_dlist;


t_dlist		*dlist_push_back(t_dlist **lst, t_dlist *new)
{
	t_dlist	*save;

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

void	dlist_clear(t_dlist **lst)
{
	t_dlist	*save;
	t_dlist	*temp;

	save = (*lst)->next;
	while (save)
	{
		temp = save;
		save = save->next;
		free(temp);
	}
	save = (*lst);
	while (save)
	{
		temp = save;
		save = save->prev;
		free(temp);
	}
	*lst = NULL;
}
	t_dlist		*hist;


void	del(int size)
{
	while (size > 0)
		write(1, "\b \b", 3);
}

void	go_up()
{
	int size;

	if (hist->prev)
	{
		del(ft_strlen(hist->updated));
		hist = hist->prev;
		write(1, hist->updated, ft_strlen(hist->updated));
	}
}

void	go_down()
{
	if (hist->next)
	{
		del(ft_strlen(hist->updated));
		hist = hist->next;
		write(1, hist->updated, ft_strlen(hist->updated));
	}
}

void	handler(int sig)
{
	if (sig == SIGSTOP)
		go_up();
	else if (sig == SIGQUIT)
		go_down();
}
int main(int argc, char **argv)
{
	char	*line;
	t_dlist	*new;

	int i = 0;
	while (!i)
{

}
	hist = NULL;
	line = NULL;
	signal(SIGQUIT, handler);
	signal(SIGTSTP, handler);
	while (1)
	{
		hist = dlist_push_back(&hist, (t_dlist *)malloc(sizeof(t_dlist)));
		hist->cmd = NULL;
		get_next_line(1, &hist->updated);
	}
}