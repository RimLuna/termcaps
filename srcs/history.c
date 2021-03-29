#include "minishell.h"


t_hist		*dlist_push_back(t_hist **lst, t_hist *new)
{
	t_hist	*save;

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

void	dlist_clear(t_hist **lst)
{
	t_hist	*save;
	t_hist	*temp;

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
append(char input, t_hist *hist)
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

void	go_up(t_hist **hist)
{
	int size;

	if ((*hist)->prev)
	{
		del(ft_strlen((*hist)->updated));
		*hist = (*hist)->prev;
		write(1, (*hist)->updated, ft_strlen((*hist)->updated));
	}
}

void	go_down(t_hist **hist)
{
	if ((*hist)->next)
	{
		del(ft_strlen((*hist)->updated));
		*hist = (*hist)->next;
		write(1, (*hist)->updated, ft_strlen((*hist)->updated));
	}
}

void	hist_init(t_hist *head, t_hist **end)
{
	*end = dlist_push_back(&g_all->hist, (t_hist *)malloc(sizeof(t_hist)));
	g_all->hist = *end;
	(*end)->llen = 0;
	(*end)->cmd = NULL;
	(*end)->updated = NULL;
	while (head)
	{
		head->updated = ft_strdup(head->cmd);
		head = head->next;
	}
}

void	hist_reset(t_hist *head)
{
	write(1 , "\n", 1);
	restore_state();
	while (head)
	{
		free(head->updated);
		head->updated = NULL;
		head = head->next;
	}
}

int
readline(char **line)
{
	char	input[5];
	t_hist	*end;
	static	t_hist *head = NULL;

	hist_init(head, &end);
	if (head == NULL)
		head = end;
	ft_bzero(input, 5);
	while ((g_all->parser.rt = read(0, &input, 4)) && input[0] != '\n')
	{
		if (g_all->parser.rt == 0 && g_all->hist->llen == 0)
			return (0);
		if (input[0] == 127 && g_all->hist->llen > 0)
		{
			g_all->hist->updated[--g_all->hist->llen] = '\0';
			write(1, "\b \b", 3);
		}
		else if (input[2] == 65)
			go_up(&g_all->hist);
			// exit(0);
		else if (input[2] == 66)
			go_down(&g_all->hist);
		else if (ft_isprint(input[0]))
			append(input[0], g_all->hist);
	}
	ft_bzero(input, 5);
	end->cmd = ft_strdup(g_all->hist->updated);
	*line = ft_strdup(g_all->hist->updated);
	hist_reset(head);
	return (1);
}
