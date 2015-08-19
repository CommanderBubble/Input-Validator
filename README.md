Input-Validator

Input-Validator is a simple collection of routines that can be used to get user input from the console in a relatively safe manner.

NOTE: Currently this header uses Microsoft Windows specific calls to the console, and will ONLY run on Windows (the second function specifically). It should be relatively easy to port to linux (it just needs to catch character input), feel free to do so and submit a pull request, as I do not have a linux distro to test with currently.

As this is a very simple set of functions, usage is simple: include the header file in your project. No library is provided as it is overkill for these functions at this time.

The functions take a number of arguments, and only allow input between the end conditions.

* Functions

get_limited_input(...)

    args:
    unsigned int upper_bound
    unsigned int lower_bound = 1
    const std::string& input_message = ""
    const std::string& error_message = ""

    returns:
    unsigned int

This function takes arguments to define an upper and lower (in that order) boundary for input numbers.
If specified, it will also print an input and error messages when requesting input and receiving invalid input respectively.
if your application needs to capture the standard delimiter (\n) for any reason it can be changed.

get_list_input(...)

    args:
    unsigned int number_options
    std::string& input_message = "  "
    
    returns:
    unsigned int

This function is used after printing out the options to choose from.
It restricts input to numbers that make a valid choice.
For example if you have 7 options, you cannot enter a digit above 7, if you have 12, you cannot enter a digit above 3 if you have already entered a 1.

This function does not require the user to press enter in most circumstances; if the input is unambiguous, the function will automatically move on.
If the user has 125 choices, then 12 will require the user to press enter, but 13 will not, as you cannot enter 13X as a valid choice.

A basic makefile is provided to demonstrate the functions with examples.
it should run under most environments that provide g++ or a linux like replacement (mingw, msys, msys2) but is untested on cygwin. It may also run against MSVC but this is untested currently.
run it with

    <your compilers make> makefile all
