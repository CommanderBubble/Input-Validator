Input-Validator

Input-Validator is a simple collection of routines that can be used to get user input from the console in a relatively safe manner.
The functions take a number of arguments, and only allow input between the end conditions.

NB: Currently getListInput is windows only as it uses a windows specific function to get the keyboard input from the console.

* Functions

getLimitedInput
  unsigned int upper_bound
  unsigned int lower_bound = 1
  std::string input_message = ""
  std:;string error_message = ""
  char delimiter = '\n'

This function takes arguments to define an upper and lower (in that order) boundary for input numbers.
If specified, it will also print an input and error messages when requesting input and recieving invalid input respectively.
if your application needs to capture the standard delimiter (\n) for any reason it can be changed.

getListInput
  unsigned int number_options
  std::string input_message

This function is used after printing out the options to choose from.
it resticts input to to numbers that make a valid choice. for exmaple if you have 7 options, you cannot enter a digit above 7, if you have 12, you cannot enter a digit above 3 if you have already entered a 1. this function does not require the user to press enter in most circumstances, if the input in unambiguous, the function will automatically move on. if the user has 125 choices, then 12 will require the user to press enter, but 13 will not, as you cannot enter 13X as a valid choice.
