#include <iostream>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
    if (argc == 1) {
        // if no command line arguments passed in
        cout << "wunzip: file1 [file2 ...]" << endl;
        exit(1);
    }
    
    char buffer[4095]; // Buffer to read in data
    int bytesRead; // bytes read into buffer
    int fileDescriptor = STDIN_FILENO;
    
    // for translating the characters to be read
    int32_t char_length = 0;
    char curr_char = '\0';
    string curr_string = "";

    // iterate through compressed files
    for (int i = 1; i < argc; i++) {

        fileDescriptor = open(argv[i], O_RDONLY);
        if (fileDescriptor == -1) { // error if file cannot be opened
            cout << "wunzip: cannot open file" << endl;
            exit(1);
        }

        // read data into the buffer
        while ((bytesRead = read(fileDescriptor, buffer, sizeof(buffer))) > 0) {
            // parse the buffer 
            int j = 0;
            while (j + 5 <= bytesRead) {
                // read in the count of the character
                memcpy(&char_length, buffer + j, sizeof(char_length));

                // read in the character byte
                memcpy(&curr_char, buffer + j + 4, sizeof(curr_char));

                // convert the character into a string to be printed out
                curr_string = "";
                for (int k = 0; k < char_length; k++) {
                    curr_string += curr_char;
                }

                // output the character string
                int write_check = write(STDOUT_FILENO, curr_string.c_str(), curr_string.length());
                if (write_check == -1) {
                    cout << "wunzip: error writing to standard output" << endl;
                }

                // move the iterator to read the next 5 bytes
                j += 5;
            }
        }

        if (bytesRead == 0) {
            // after done printing to STD OUT, close the file descriptor
            int close_check = close(fileDescriptor);
            // check if there was an error closing the file descriptor
            if (close_check == -1) {
                cout << "wunzip: error closing file descriptor" << endl;
                exit(1);
            }
        } else { // bytesRead == -1, read error checker
            cout << "wunzip: cannot read file" << endl;
            exit(1);
        }
    }


    return 0;
}