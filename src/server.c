#include <stdlib.h>
#include <stdio.h>
#include <linux/epoll.h>
#include "utils.h"
#include "klist.h"
#include "srvlog.h"

typedef struct st_client {
	int sock_fd;
	struct list_head list;
}client_info;

struct list_head client_list;

int set_nonblocking(int fd)
{
	RET_CHECK(fcntl(fd, F_SETFL, fcntl(fd, F_GETFD, 0) | O_NONBLOCK));
	return 0;
}

client_info * alloc_client_info()
{
	client_info *client = NULL;
	client = (client_info *)malloc(sizeof(client_info));
	if(client)
		return client;
	return NULL;
}

void free_client_info(client_info *info)
{
	FREE(info);
}

int main(int argc, char *argv[])
{
	int listen_fd;
	client_info *client_node = NULL;
	struct sockaddr_in srv_addr, new_addr;
	socklen_t sock_len;
	char msg[BUF_SIZE] = {0};
	int epoll_fd;
	int client, ret, events_count, i;
	/*time tracker*/
	clock_t start;

	srv_addr.sin_family = PF_INET;
	srv_addr.sin_port = htons(SERVER_HOST);
	srv_addr.sin_addr.s_addr = inet_addr(SERVER_HOST);
	sock_len = sizeof(struct sockaddr_in);

	struct epoll_event ev,events[EPOLL_SIZE];
	ev.events = EPOLLIN | EPOLLET;

	/*initial socket fd*/
	RSLT_CHECK(listen_fd, socket(PF_INET, SOCK_STREAM, 0));
	set_nonblocking(listen_fd);
	RET_CHECK(bind(listen_fd, (struct sockaddr *)&addr, sizeof(addr)));
	RET_CHECK(listen(listen_fd, 1));

	/*epoll standard api*/
	RSLT_CHECK(epoll_fd, epoll_create(EPOLL_SIZE));
	ev.date.fd = listen_fd;
	RET_CHECK(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &ev));

	/*persistent server*/
	while(1) {
		RSLT_CHECK(events_count, epoll_wait(epoll_fd, events, EPOLL_SIZE, EPOLL__TIMEOUT));
		start = clock();
		for(i = 0; i < events_count; i++) {
			if(events[i].data.fd == listen_fd) {
				/*new connection*/
				RSLT_CHECK(client, accept(listen_fd, (struct sockaddr *)&new_addr, &sock_len));
				set_nonblocking(client);
				ev.data.fd = client;
				RET_CHECK(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client, &ev));

				/*add new connect to client list*/
				client_node = alloc_client_info();
				if(client_node)
					list_add_tail(&(client_node->list), &client_list);

				/*return message to client*/
				bzero(msg, BUF_SIZE);
				ret = snprintf(msg, BUF_SIZE, "%s, Your client ID : %d.\n", "Welcome to connect server.", client);
				RSLT_CHECK(ret, send(client, msg, BUF_SIZE, 0));

			}
			else {
				/*recieve client message*/
				RSLT_CHECK(ret, handle_message(events[i].data.fd));
			}
		}
		fprintf(stdout,"%d events handle at :%.2f(s) \n",events_count, (double)(clock() - start)/CLOCKS_PER_SEC);
	}
	close(listen_fd);
	close(epoll_fd);

    return 0;
}
int handle_message(int client)
{
    char buf[BUF_SIZE] = {0};
    char message[BUF_SIZE] = {0};
    int len = 0;
    client_info *tmp_node = NULL;
    struct list_head *pos,*n;

    RST_CHK(len,recv(client, buf, BUF_SIZE, 0));

    if(len == 0) {
        RST_CHK(close(client));
	list_for_each_safe(pos, n, &client_list) {
		tmp_node = list_entry(pos, client_info, list);
		if(client == tmp_node->sockfd) {
			list_del(pos);
			free_client_info(tmp_node);
		}
	}
    }
    else {
        sprintf(message, BUF_SIZE, "%d %s", client, buf);
        list_for_each_safe(pos, n, head)(pos,n,&client_list) {
            tmp_node = list_entry(pos, client_info, client_list);
            if(!tmp_node) {
                RET_CHECK(send(tmp_node->fd, message, BUF_SIZE, 0));
           }
        }
    }

    return len;
}
