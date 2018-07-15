#include "incl/ping.h"

static void			useage(void)
{
	printf("Usage:\tping [-chv]\n"
			"[-c count][-h help][-v verbose]\n");
	exit(SUCCESS);
}

static struct ifaddrs	*find_useable_src(struct ifaddrs *addrs)
{
	while (addrs)
	{
		if (addrs->ifa_addr && (addrs->ifa_flags & IFF_UP))
		{
			if (addrs->ifa_addr->sa_family == AF_INET &&
				((struct sockaddr_in *)addrs->ifa_addr)->sin_addr.s_addr != htonl(2130706433UL))
				return (addrs);
		}
		addrs = addrs->ifa_next;
	}
	return (NULL);
}

static void		get_source(t_mgr *mgr)
{
	struct ifaddrs	*addrs;
	struct ifaddrs	*src;

	if (getifaddrs(&addrs) != 0 || !(src = find_useable_src(addrs)))
	{
		dprintf(STDERR_FILENO, "ping: could not find suitable IP for localhost");
		exit(FAILURE);
	}
	inet_ntop(AF_INET, &((struct sockaddr_in *) src->ifa_addr)->sin_addr,
			  mgr->saddr, sizeof(mgr->saddr));
	printf("%s\n", mgr->saddr);
	freeifaddrs(addrs);
}

static void		get_destination(t_mgr *mgr, char *dst)
{
	struct addrinfo hints;
	struct addrinfo *infoptr;

	mgr->count = 0;
	ft_memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	ft_memcpy(mgr->domain, dst, ft_strlen(dst));
	if (getaddrinfo(dst, 0, &hints, &infoptr) != 0)
	{
		dprintf(STDERR_FILENO, "ping: cannot resolve %s: Unknown host\n", dst);
		exit(FAILURE);
	}
	inet_ntop(AF_INET, &((struct sockaddr_in *) infoptr->ai_addr)->sin_addr,
			  mgr->daddr, sizeof(mgr->daddr));
	printf("%s\n", mgr->daddr);
	freeaddrinfo(infoptr);
}

static void		set_option(t_mgr *mgr, char opt, char **av, int *i)
{
	if (opt == 'c')
	{
		mgr->flags.count = TRUE;
		if (!av[*i + 1] || (mgr->count = (size_t)ft_atoi(av[*i + 1])) == 0)
		{
			dprintf(STDERR_FILENO, "ping: invalid count of packets to transmit.\n");
			useage();
		}
		*i += 1;
	}
	else if (opt == 'v')
		mgr->flags.verbose = TRUE;
	else if (opt == 'h')
		useage();
	else
	{
		dprintf(STDERR_FILENO, "ping: invalid option -- %c\n", opt);
		useage();
	}
}

static int		parse_arguments(t_mgr *mgr, int ac, char **av)
{
	int 		i;

	i = 0;
	while (av[++i])
	{
		if (av[i][0] != '-' || ac == 2)
			get_destination(mgr, av[i]);
		else if (av[i][0] == '-' && av[i][1])
			set_option(mgr, av[i][1], av,  &i);
	}
	return (SUCCESS);
}

int				main(int ac, char **av)
{
	t_mgr		mgr;

	init_mgr(&mgr);
	if (ac == 1)
		useage();
	get_source(&mgr);
	parse_arguments(&mgr, ac, av);
	create_socket(&mgr);
	ping(&mgr);
	return (SUCCESS);
}
