#include <iostream>
#include <cstdlib>
#include <unistd.h>

void start_server();
void start_client();
using std::cout;

int main() {
    std::cout << "Starting main\n";
    pid_t pid = fork();

    if (pid == 0) {
        std::cout << "Child process: Starting server\n";
        start_server();
    } else {
        std::cout << "Parent process: Sleeping 2 seconds before starting client\n";
        sleep(2); // Give server time to bind and listen
        start_client();
    }

    return 0;

}