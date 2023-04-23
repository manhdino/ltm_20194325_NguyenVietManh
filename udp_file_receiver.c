#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define SIZE 1024

int main(int argc, char *argv[])
{
    int receiver = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(atoi(argv[1]));

    bind(receiver, (struct sockaddr *)&addr, sizeof(addr));

    struct sockaddr_in sender_addr;
    int sender_addr_len = sizeof(sender_addr);
    char filename[560];
    int name_size;
    int ret = recvfrom(receiver, &name_size, sizeof(int), 0,
                       (struct sockaddr *)&sender_addr, &sender_addr_len);
    if (ret == -1)
    {
        printf("recv size name failed\n");
    }
    printf("name_size:%d", name_size);
    int ret2 = recvfrom(receiver, filename, name_size, 0, (struct sockaddr *)&sender_addr, &sender_addr_len);
    filename[name_size] = '\0';
    printf("filename:%s", filename);
    char buf_name[1024];
    sprintf(buf_name, "new_%s", filename);
    printf("new_name:%s", buf_name);
    FILE *f = fopen(buf_name, "w");
    char buf[SIZE];
    printf("\n");
    while (1)
    {
        int ret = recvfrom(receiver, buf, sizeof(buf), 0,
                           (struct sockaddr *)&sender_addr, &sender_addr_len);
        if (strcmp(buf, "END") == 0)
        {
            break;
        }
        int buf_size = strlen(buf);
        printf("DATA RECEVING: %s\n", buf);
        fwrite(buf, buf_size, 1, f);
        printf("File received successfully\n");
        // bzero(buf, SIZE);
    }
    fclose(f);
    close(receiver);
}