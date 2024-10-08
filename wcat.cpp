#include <iostream>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
    // if no arguments exit the program with RC 0
    if (argc == 1) {
        return 0;
    }

    // iterate through our command line arguments
    int fileDescriptor;
    for (int i = 1; i < argc; i++) {

        fileDescriptor = open(argv[i], O_RDONLY);
        if (fileDescriptor == -1) { // error if a file can't be read
            cout << "wcat: cannot open file" << endl;
            exit(1);
        }

        // create a buffer to read from the file, return value to interpret read syscalls
        char buffer[4096];
        int bytesRead;

        // read from the file and write to standard output
        while ((bytesRead = read(fileDescriptor, buffer, sizeof(buffer))) > 0) {
            int write_checker = write(STDOUT_FILENO, buffer, bytesRead);
            // check if there was a write error
            if (write_checker == -1) {
                cout << "wcat: error writing to standard output" << endl;
                exit(1);
            }
        }

        if (bytesRead == 0) {
            // after done printing to STD OUT, close the file descriptor
            int close_checker = close(fileDescriptor);
            // check if there was an error closing the file descriptor
            if (close_checker == -1) {
                cout << "wcat: error closing file descriptor" << endl;
                exit(1);
            }
        } else { // bytesRead == -1, read error checker
            cout << "wcat: cannot read file" << endl;
            exit(1);
        }

    }
    // successful execution 
    return 0;
}