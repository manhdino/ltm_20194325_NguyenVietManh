#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define SIZE 2048

void send_file_data(FILE *fp, int sockfd, struct sockaddr_in addr)
{
    char buf[SIZE];
    while (fgets(buf, SIZE, fp) != NULL)
    {
        {
            int res = sendto(sockfd, buf, SIZE, 0, (struct sockaddr *)&addr, sizeof(addr));
            if (res == -1)
            {
                perror("[ERROR] sending data to the server.");
                exit(1);
            }
            bzero(buf, SIZE);
        }
    }

    // sendind END of file
    strcpy(buf, "END");
    sendto(sockfd, buf, SIZE, 0, (struct sockaddr *)&addr, sizeof(addr));
    fclose(fp);
}
int main(int argc, char *argv[])
{
    int sender = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // Khai bao dia chi ben nhan
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));

    char *filename = argv[3];
    FILE *fp = fopen(filename, "r");

    // send file name to server
    char buf_name[SIZE];
    int name_len = strlen(filename);
    printf("name_len%d\n", name_len);
    int send_name_len = sendto(sender, &name_len, sizeof(int), 0, (struct sockaddr *)&addr, sizeof(addr));
    if (send_name_len == -1)
    {
        printf("send_name_len failed");
    }
    strcpy(buf_name, filename);
    printf("buf_name: %s\n", buf_name);
    int send_name = sendto(sender, buf_name, strlen(filename), 0, (struct sockaddr *)&addr, sizeof(addr));
    if (send_name == -1)
    {
        printf("send_name failed");
    }
    send_file_data(fp, sender, addr);
    close(sender);
}