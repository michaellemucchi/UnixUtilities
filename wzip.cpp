#include <iostream>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
    if (argc == 1) {
        // if no command line arguments passed in, print usage
        cout << "wzip: file1 [file2 ...]" << endl;
        exit(1);
    }

    char buffer[4096];  // buffer for reading data
    int bytesRead;

    // Compression variables
    char curr_char = '\0';
    int32_t char_length = 0;
    bool first_char = true;  // handling the first character read

    // iterate through all files provided in command line arguments
    for (int i = 1; i < argc; i++) {

        int fileDescriptor = open(argv[i], O_RDONLY);
        if (fileDescriptor == -1) { // error if the file cannot be opened
            cout << "wzip: cannot open file" << endl;
            exit(1);
        }

        // read data from file in chunks
        while ((bytesRead = read(fileDescriptor, buffer, sizeof(buffer))) > 0) {
            for (int j = 0; j < bytesRead; j++) {
                if (first_char) {
                    // Initialize the first character and its count
                    memcpy(&curr_char, buffer, sizeof(curr_char));
                    char_length = 1;
                    first_char = false;
                    continue;
                }
                if (buffer[j] == curr_char) {
                    // if current char matches the previous, increment counter
                    char_length++;
                } else {
                    char write_buffer[5]; // buffer for writing data;

                    // write the count and character to our write buffer
                    memcpy(write_buffer, &char_length, sizeof(char_length));
                    memcpy(write_buffer + 4, &curr_char, sizeof(curr_char));

                    // write the data to standard output
                    int write_check = write(STDOUT_FILENO, write_buffer, sizeof(write_buffer));
                    if (write_check == -1) {
                        cout << "wzip: error writing output" << endl;
                        exit(1);
                    }

                    // reset for the new character
                    memcpy(&curr_char, buffer + j, sizeof(curr_char));
                    char_length = 1;
                }
            }
        }

        if (bytesRead == 0) {
            // after done printing to STD OUT, close the file descriptor
            int close_checker = close(fileDescriptor);
            // check if there was an error closing the file descriptor
            if (close_checker == -1) {
                cout << "wzip: error closing file descriptor" << endl;
                exit(1);
            }
        } else { // bytesRead == -1, read error checker
            cout << "wzip: cannot read file" << endl;
            exit(1);
        }
    }


    // after processing all files, write the final run of characters
    if (!first_char) {
        char write_buffer[5]; // buffer for writing data;

        // write the count and character to the write buffer
        memcpy(write_buffer, &char_length, sizeof(char_length));
        memcpy(write_buffer + 4, &curr_char, sizeof(curr_char));
        
        // write the data to standard output
        int write_check = write(STDOUT_FILENO, write_buffer, sizeof(write_buffer));
        if (write_check == -1) {
            cout << "wzip: error writing output" << endl;
            exit(1);
        }

    }

    return 0;
}
