#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

char* read_file(const char* filename){
    FILE* ptr = fopen(filename, "r");
    if (ptr == NULL){ return NULL ; }

    fseek(ptr, 0, SEEK_END);
    long size = ftell(ptr);
    fseek(ptr, 0, SEEK_SET);

    char* buffer = malloc(size + 1);
    fread(buffer, 1, size, ptr);
    buffer[size] = '\0';

    fclose(ptr);
    return buffer;
    
}

void parse_request(char *buffer, char *method, char *path)
{
    sscanf(buffer, "%s %s", method, path);
}

void handle_request(int client_fd, char *method, char *path)
{
    int len = strlen(path);
    if (len > 1 && path[len - 1] == '/')
    {
        path[len - 1] = '\0';
    }
    char filename[256];

    if (strcmp(path, "/") == 0)
    {
        snprintf(filename, sizeof(filename), "pages/index.html");
    } else {
        snprintf(filename, sizeof(filename), "pages%s.html", path);
    }

    char* body = read_file(filename);
    if (body == NULL)
    {
        char* not_found = read_file("pages/404.html");
        char response[1024];
        snprintf(response, sizeof(response),
        "HTTP/1.1 404 Not Found\r\n"
            "Content-Type: text/html\r\n"
            "\r\n"
            "%s", not_found ? not_found : "<h1>404</h1>");

            write(client_fd, response, strlen(response));
            free(not_found);
            return;
    }
    
    char response[9090];
    snprintf(response, sizeof(response), "HTTP/1.1 200 OK\r\n"
                                         "Content-Type: text/html\r\n"
                                         "\r\n"
                                         "%s",body);

    write(client_fd, response, strlen(response));
}

void* handle_client(void* arg){
    int client_fd = *(int*)arg;
    free(arg);

    char buffer[1024] = {0};
        read(client_fd, buffer, 1024);

        char method[8] = {0};
        char path[256] = {0};
        parse_request(buffer, method, path);

        printf("Method: %s | Path: %s\n", method, path);

        handle_request(client_fd, method, path);

        close(client_fd);
        return NULL;
}

int main()
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
        perror("socket failed");
        return 1;
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);
    address.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == -1)
    {
        perror("bind failed");
        return 1;
    }

    listen(server_fd, 5);
    printf("Server running on http://localhost:8080\n");

    while (1)
    {
        int client_fd = accept(server_fd, NULL, NULL);
        if (client_fd == -1)
        {
            perror("accept failed");
            continue;
        }

        int* client_fd_ptr = malloc(sizeof(int));
        *client_fd_ptr = client_fd;

        pthread_t thread;
        pthread_create(&thread, NULL, handle_client, client_fd_ptr);
        pthread_detach(thread);
    }

    close(server_fd);

    return 0;
}
