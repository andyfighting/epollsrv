#include <stdlib.h>
#include <stdio.h>
#include <linux/epoll.h>
#include "utils.h"
#include "klist.h"
#include "srvlog.h"

int main(int argc, char *argv[])
{
    int listen_fd;
	struct sockaddr_in srv_addr, new_addr;

	srv_addr.sin_family = PF_INET;
	srv_addr.sin_port = htons(SERVER_HOST);
	srv_addr.sin_addr.s_addr = inet_addr(SERVER_HOST);
	socklen_t sock_len;
	sock_len = sizeof(struct sockaddr_in);

	struct epoll_event ev,events[EPOLL_SIZE];
	ev.events = EPOLLIN | EPOLLET;

    /*epoll standard api*/

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
    }
    else {
        sprintf(message, STR_MESSAGE, client, buf);

        list_for_each_safe(pos, n, head)(pos,n,&client_list) {
            tmp_node = list_entry(pos, client_info, client_list);
            if(!tmp_node) {
                RST_CHK(send(tmp_node->fd, message, BUF_SIZE, 0));
           }
        }
    }

    return len;
}