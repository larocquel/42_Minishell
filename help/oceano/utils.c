#include "cell.h"

void	printbanner(void)
{
	printf(RED"   ███▄ ▄███▓ ██▓ ███▄    █  ██▓ ██░ ██ ▓█████  ██▓     ██▓\n"
			"  ▓██▒▀█▀ ██▒▓██▒ ██ ▀█   █ ▓██▒▓██░ ██▒▓█   ▀ ▓██▒    ▓██▒\n"
			"  ▓██    ▓██░▒██▒▓██  ▀█ ██▒▒██▒▒██▀▀██░▒███   ▒██░    ▒██░\n"
			"  ▒██    ▒██ ░██░▓██▒  ▐▌██▒░██░░▓█ ░██ ▒▓█  ▄ ▒██░    ▒██░\n"
			"  ▒██▒   ░██▒░██░▒██░   ▓██░░██░░▓█▒░██▓░▒████▒░██████▒░██████▒\n"
			"  ░ ▒░   ░  ░░▓  ░ ▒░   ▒ ▒ ░▓   ▒ ░░▒░▒░░ ▒░ ░░ ▒░▓  ░░ ▒░▓  ░\n"
			"  ░  ░      ░ ▒ ░░ ░░   ░ ▒░ ▒ ░ ▒ ░▒░ ░ ░ ░  ░░ ░ ▒  ░░ ░ ▒  ░\n"
			"  ░      ░    ▒ ░   ░   ░ ░  ▒ ░ ░  ░░ ░   ░     ░ ░     ░ ░\n"
		    "  ░    ░           ░  ░   ░  ░  ░   ░  ░    ░  ░    ░  ░\n"RST);
}

void	Getcwd(char *buf, size_t size)
{
	if (getcwd(buf, size) == NULL)
		perror(RED"getcwd FAILED"RST);
}

void	*Malloc(size_t size)
{
	void	*ptr;

	if (size == 0)
		return (NULL);

	ptr = malloc(size);
	if (!ptr)
	{
		perror(RED"Malloc failed"RST);
		exit(EXIT_FAILURE);
	}
	return(ptr);
}

void	*Realloc(void *ptr, size_t size)
{
	void	*new_ptr;

	new_ptr = realloc(ptr, size);
	if (!new_ptr && size != 0)
	{
		perror(RED"Realloc failed"RST);
		exit(EXIT_FAILURE);
	}
	return (new_ptr);
}

void	loading(void)
{
	const char	*charging[] =
	{
		"[            ]",
		"[=           ]",
		"[==          ]",
		"[===         ]",
		"[====        ]",
		"[=====       ]",
		"[======      ]",
		"[=======     ]",
		"[========    ]",
		"[=========   ]",
		"[==========  ]",
		"[=========== ]",
		"[===========🌀]",
	};
	const int	frames = sizeof(charging) / sizeof(charging[0]);

	printf(RED"Shutting down...\n"RST);

	for (int i = 0; i < frames; i++)
	{
		printf("\r"YEL"%s"RST, charging[i]);
		fflush(stdout);
		usleep(50000);
	}
	printf(CIA"\nEXIT\n"RST);
}

pid_t	Fork(void)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror(RED"Fork failed"RST);
		exit(EX_OSERR);
	}
	return (pid);
}

void	Execvp(const char *file, char *const argv[])
{
	if (!file || !argv)
	{
		fprintf(stderr, RED"Execvp failed: Invalid arguments\n"RST);
		exit(EXIT_FAILURE);
	}
	if (execvp(file, argv) == -1)
	{
		perror(RED"Cell_Jr failed"RST);
		exit(EX_UNAVAILABLE);
	}
}

pid_t	Wait(int *status)
{
	pid_t	result;

	if (!status)
	{
		fprintf(stderr, RED"Wait: Status argument required\n"RST);
		return (-1);
	}
	result = wait(status);
	if (result == -1)
		perror(RED"Wait failed"RST);
	if (WIFEXITED(*status))
		*status = WEXITSTATUS(*status);
	return (result);
}