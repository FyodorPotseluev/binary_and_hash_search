
                            binary_and_hash_search

                                Description

    Creates and search in a database file that stores "string" + "value" pairs.

    It uses either binary search or hash table based search at the user's discretion.
    The binary algorithm can add and an entry to a file and find it in logarithmic time O(log N).
    The hash table based algorithm can add an entry to a file and find it for a constant time O=log(1)*

    *(except of the cases when we need to add an entry to a hash file that is already 2/3 full and needs to be rebuilt in order to keep the hash search efficient. The rebuilding process can take a considerable amount of time).

                                Prerequisites

    Apart from standard C library relies on `unistd` library so it won't natively run on Windows or other non-Unix platforms.

                                Installation

    Run either `make D=BIN` or `make D=HASH` in the project directory depending on wich algorithm you'd like to use.

                                Usage

    Run `make help` to see the list of Makefile commands.

    The list of program's commands:
    - add - the record containing the given ID must be found in the given file. The entry counter must be incremented. If there is no such entry, it must be added to the end of the file with the counter value equals 1.
    - query - The record containing the given ID must be found in the given file. The value of the "element ID + counter" pair must be printed to standard output. If there is no such entry, the counter value is going to be 0.
    - list - prints all the "item id + counter" pairs contained in the file into standart output. The third command line argument may be not specified and ,if it does, should be ignored.

                                Contributing

    Issue Tracker: github.com/..............................

                                License

    MIT License

                                Contact

    kommunist90@gmail.com
