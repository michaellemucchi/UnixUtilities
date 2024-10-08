#include <iostream>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {

    if (argc == 1) {
        // if no command line arguments passed in
        cout << "wgrep: searchterm [file ...]" << endl;
        exit(1);
    }

    int fileDescriptor = STDIN_FILENO; // default to standard in
    string searchTerm = argv[1]; // search term
    char buffer[4096]; // buffer for reading data
    int bytesRead; // bytes read
    string curr_line; 

    // account for multiple files as command line arguments
    int loops;
    if (argc == 2) {
        loops = 1;
    } else {
        loops = argc - 2;
    }

    for (int j = 0; j < loops; j++) {
        if (argc != 2) {
            fileDescriptor = open(argv[j+2], O_RDONLY);
            // error if a file can't be read
            if (fileDescriptor == -1) { // open error check
                string open_error = "wgrep: cannot open file\n";
                write(STDOUT_FILENO, open_error.c_str(), open_error.length());
                exit(1);
            }
        } 
        
        // read from the file descriptor
        while ((bytesRead = read(fileDescriptor, buffer, sizeof(buffer))) > 0) {
            
            // convert the buffer (char array) to string to prevent conversion errors
            string temp;
            temp.append(buffer, 0, bytesRead);

            // iterate through buffer
            for (int i = 0; temp[i] != '\0'; i++) {
                if (temp[i] == '\n') { // check for end of line
                    if (curr_line.find(searchTerm) != string::npos) {
                        // the current line has the searchTerm, print out
                        curr_line += "\n";
                        int write_checker = write(STDOUT_FILENO, curr_line.c_str(), curr_line.length());
                        if (write_checker == -1) { // write error check
                            cout << "wgrep: error writing to standard output" << endl;
                            exit(1);
                        }
                    }
                    // reset the current line after checking for search term
                    curr_line.clear();
                    continue;
                }
                curr_line += temp[i];
            }
        }

        if (bytesRead == 0) {
            // after done printing to STD OUT, close the file descriptor
            int close_checker = close(fileDescriptor);
            // check if there was an error closing the file descriptor
            if (close_checker == -1) {
                cout << "wgrep: error closing file descriptor" << endl;
                exit(1);
            }
        } else { // bytesRead == -1, read error checker
            cout << "wgrep: cannot read file" << endl;
            exit(1);
        }
    }
    
    return 0;
}