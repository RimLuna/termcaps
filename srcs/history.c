#include "minishell.h"
#include <termcap.h>
#include <termios.h>
#include <term.h>

#define DELETE 127

t_dlist		*hist = NULL;

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

int
set_state(void)
{
	struct termios		term;

	if (tgetent(0, get_var_value("TERM")) < 1)
		return (0);
	tcgetattr(0, &term);
	term.c_lflag &= ~(ICANON | ECHO);
	term.c_cc[VMIN] = 1;
	term.c_cc[VTIME] = 0;
	tcsetattr(0, TCSANOW, &term);
	return (1);
}

void
restore_state(void)
{
	struct termios		restore;

	tcgetattr(0, &restore);
	restore.c_lflag |= (ICANON | ECHO);
	tcsetattr(0, TCSANOW, &restore);
}


void
append(char input)
{
	char *tmp;
	int len;

	len = ft_strlen(hist->updated);
	tmp = hist->updated;
	hist->updated = (char *)malloc((len + 1) * sizeof(char));
	ft_bzero(hist->updated, len + 1);
	strncpy(hist->updated, tmp, len);
	hist->updated[len] = input;
	hist->updated[len + 1] = '\0';
	write(1, &input, 1);
	hist->llen++;
	free(tmp);
}

void	del(int size)
{
	while (size-- > 0)
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

void hist_init(t_dlist *head)
{
	while (head)
	{
		head->updated = ft_strdup(head->cmd);
		head = head->next;
	}
}

void	hist_reset(t_dlist *head)
{
	while (head)
	{
		free(head->updated);
		head->updated = NULL;
		head = head->next;
	}
}
char*
readline(t_dlist *history)
{
	char	input[5];
	t_dlist	*end;
	static	t_dlist *head = NULL;

	if (!set_state())
		return (NULL);
	ft_bzero(input, 5);
	end = dlist_push_back(&hist, (t_dlist *)malloc(sizeof(t_dlist)));
	if (head == NULL)
		head = end;
	hist = end;
	hist->llen = 0;
	hist->cmd = NULL;
	hist->updated = NULL;
	hist_init(head);
	while (read(0, &input, 4) && input[0] != '\n')
	{
		if (input[0] == DELETE && hist->llen > 0)
		{
			hist->updated[--hist->llen] = '\0';
			write(1, "\b \b", 3);
		}
		else if (input[2] == 65)
			go_up();
		else if (input[2] == 66)
			go_down();
		else if (ft_isprint(input[0]))
			append(input[0]);
		ft_bzero(input, 5);
	}
	write(1 , "\n", 1);
	end->cmd = ft_strdup(hist->updated);
	hist_reset(head);
	restore_state();
	return (NULL);
}
