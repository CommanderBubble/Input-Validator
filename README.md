Input-Validator

Input-Validator is a simple collection of routines that can be used to get user input from the console in a relatively safe manner.

As this is a very simple set of functions, usage is simple: include the files in your project and compile them into it. no library is provided as it is overkill for these functions at this time.

The functions take a number of arguments, and only allow input between the end conditions.

NB: Currently getListInput is windows only as it uses a windows specific function to get the keyboard input from the console.

* Functions

getLimitedInput(...)

    args:
    unsigned int upper_bound
    unsigned int lower_bound = 1
    std::string input_message = ""
    std:;string error_message = ""
    char delimiter = '\n'

    returns:
    unsigned int

This function takes arguments to define an upper and lower (in that order) boundary for input numbers.
If specified, it will also print an input and error messages when requesting input and receiving invalid input respectively.
if your application needs to capture the standard delimiter (\n) for any reason it can be changed.

getListInput(...)

    args:
    unsigned int number_options
    std::string input_message = "  "
    
    returns:
    unsigned int

This function is used after printing out the options to choose from.
It restricts input to numbers that make a valid choice. for example if you have 7 options, you cannot enter a digit above 7, if you have 12, you cannot enter a digit above 3 if you have already entered a 1. this function does not require the user to press enter in most circumstances, if the input in unambiguous, the function will automatically move on. if the user has 125 choices, then 12 will require the user to press enter, but 13 will not, as you cannot enter 13X as a valid choice.

A basic makefile is provided to demonstrate the functions with examples.
it should run under most environments that provide g++ or a linux like replacement (mingw, msys, msys2) but is untested on cygwin.
run it with

    <your compilers make> makefile all
