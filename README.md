# UnixUtilities
Implemented command line-utilities including wcat, wgrep, wzip, wunzip

## Usage
compilation: g++ -o {executable} -Wall -Werror {fileName.cpp}

wcat:
- Prints out the contents of file(s).
- ./wcat [file1] [file2] ...

wgrep:
- Utilizes a search term and prints out lines with that term from input file(s).
- ./wgrep {searchTerm} [file1] [file2] ...
- cat [file] | ./wgrep {searchTerm}

wzip:
- Reads in file(s) and performs Run-Length Encoding to compress file(s). Prints to standard output.
- ./wzip [file1] [file2] ...
  
wunzip:
- Reads in file(s) and reverts Run-Length Encoding to input file(s). Prints to standard output.
- ./wunzip [file1] [file2] ...

  

