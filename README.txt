
                            binary_and_hash_search

                                Description

    Creates and search in a database file that stores "string" + "value" pairs.

    It uses either binary search or hash table based search at the user's discretion.
    The binary algorithm can add and an entry to a file and find it in logarithmic time O(log N).
    The hash table based algorithm can add an entry to a file and find it for a constant time O(1)*

    *(except of the cases when we need to add an entry to a hash file that is already 2/3 full and needs to be rebuilt in order to keep the hash search efficient. The rebuilding process can take a considerable amount of time).

                                Prerequisites

    Apart from standard C library relies on `unistd` library so it won't natively run on Windows or other non-Unix platforms.

                                Installation

    Run either `make D=BIN` or `make D=HASH` in the project directory depending on wich algorithm you'd like to use.

                                Usage

    Run `make help` to see the list of Makefile commands.

    The list of program's commands:
    - add - the record containing the given ID must be found in the given file. The entry counter must be incremented. If there is no such entry, it must be added to the file with the counter value equals 1.
    ```
    make run file add entry
    ```
    - query - The record containing the given ID must be found in the given file. The value of the "element ID + counter" pair must be printed to standard output. If there is no such entry, the counter value is going to be 0.
    ```
    make run file query entry
    ```
    - list - prints all the "item id + counter" pairs contained in the file into standart output. The third command line argument may be not specified and ,if it does, should be ignored.
    ```
    make run file list
    ```
    - merge - combines two files into a new one. The final file includes all the identifiers from the source files. The value of each identifier either remains unchanged, if the identifier was present only in one of the source files, or is added to the value of the same identifier from the second source file.
    ```
    make run merge file_1 file_2 final_file
    ```
                                Contributing

    Discussions:   github.com/FyodorPotseluev/binary_and_hash_search/discussions
    Issue Tracker: github.com/FyodorPotseluev/binary_and_hash_search/issues

                                License

    MIT License

                                Contact

    kommunist90@gmail.com
