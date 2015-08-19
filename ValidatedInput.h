#ifndef VALIDATEDINPUT_H_INCLUDED
#define VALIDATEDINPUT_H_INCLUDED

#include <string>
#include <windows.h>
#include <iostream>

// does not deal with INT overflows
inline unsigned int get_limited_input(const unsigned int upper_limit, const unsigned int lower_limit = 1, const std::string& input_message = "", const std::string& error_message = "") {
    unsigned int ret;

    std::string temp;

    while (!(ret >= lower_limit and ret <= upper_limit)) {
        if (input_message != "")
            std::cout << input_message;

        ret = 0;

        getline(std::cin, temp);

        for (unsigned int i = 0; i < temp.size(); i++)
            if (isdigit(temp.c_str()[i]))
                ret = ret * 10 + (static_cast<unsigned int>(temp.c_str()[i]) - 48);
            else
                break;

        if (!(ret >= lower_limit and ret <= upper_limit) and error_message != "")
            std::cout << error_message << std::endl;
    }

    return ret;
}

inline void error_exit(const std::string& error_message, const HANDLE hStdin = 0, const DWORD fdwSaveOldMode = 0, const bool exitProgram = false, const int exitCode = 0) {
    std::cerr << error_message << std::endl;

    // Restore input mode on exit.
    /* Windows Specific Function */
    if (hStdin and fdwSaveOldMode)
        SetConsoleMode(hStdin, fdwSaveOldMode);

    // Only terminate the program if desired
    if (exitProgram) {
        std::cerr << "Process will now terminate\n";
        ExitProcess(exitCode);
    }
}

// does not deal with INT overflows; though if you have this many options you might want to reconsider what you're doing...
inline unsigned int get_list_input(const unsigned int number_options, const std::string& input_message = "  ") {
    HANDLE hStdin;
    DWORD fdwSaveOldMode, fdwMode;

    /* Windows Specific functions */
    // Get the standard input handle.
    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    if (hStdin == INVALID_HANDLE_VALUE)
        error_exit("GetStdHandle", hStdin, fdwSaveOldMode);

    // Save the current input mode, to be restored on exit.
    if (! GetConsoleMode(hStdin, &fdwSaveOldMode) )
        error_exit("GetConsoleMode", hStdin, fdwSaveOldMode);

    // Enable the window input events.
    fdwMode = ENABLE_WINDOW_INPUT;
    if (! SetConsoleMode(hStdin, fdwMode) )
        error_exit("SetConsoleMode", hStdin, fdwSaveOldMode);

    // Setup the variables
    unsigned int temp = number_options,
                 option_digits = 0,
                 option_places[20] = {0};

    // store the digits for easy access
    while (temp != 0 and option_digits < 20) {
        option_places[option_digits] = temp % 10;
        temp = (temp - option_places[option_digits]) / 10;
        option_digits++;
    }

    // Setup the input variables
    unsigned int ret = 0,
                 working = 0,
                 working_places[20] = {0},
                 entered_digits = 0;

    std::cout << input_message;

    while (ret == 0 or ret > number_options) {
        // allocate space for additional events incase they are captured
        char buf[100];
        DWORD read;

        // Get the next 10 input from the console
        // this should return them one at a time, but will capture up to the next 10
        /* Windows Specific Function */
        ReadConsole(hStdin, buf, 10, &read, NULL);

        // Iterate over the characters we have recieved
        for (unsigned int i = 0; i < read; i++) {

            // If a character is entered that isn't a digit
            if (!isdigit(buf[i])) {

                // Handle the backspace character
                if (static_cast<int>(buf[i]) == 8) {
                    if (entered_digits > 0) {
                        working = (working - (working % 10)) / 10;
                        working_places[entered_digits] = 0;

                        // clear the invalid character from the screen
                        std::cout << "\r" << input_message;
                        for (unsigned int i = 0; i < entered_digits; i++)
                            std::cout << " ";

                        std::cout << "\r" << input_message;
                        if (working)
                            std::cout << working;

                        entered_digits--;
                    }

                // Handle the enter character
                } else if (static_cast<int>(buf[i]) == 13)

                    // If we have a valid choice entered we accept it
                    if (working > 0 and working <= number_options)
                        ret = working;

                // Ignore any other characters
                continue;

            // If we enter a digit
            } else {

                // Get the character as an integer
                unsigned int digit = static_cast<unsigned int>(buf[i]) - 48;

                // If we:
                //        enter the first digit as a 0
                //        would exceed the maximum number of options with this digit, i.e. choices 1-12, we enter a 1 then a 3
                if ((working == 0 and digit == 0) or (entered_digits + 1 == option_digits and digit > option_places[option_digits - (entered_digits + 1)]))
                    break;

                working = working * 10 + digit;
                entered_digits++;
                working_places[entered_digits] = digit;

                std::cout << digit;

                // If the number entered so far can't possibly be another option ( choies 1-12, we enter 7; choices 1-125, we enter 13) then accept it without needing an enter keypress
                if ((entered_digits + 1 == option_digits and working_places[entered_digits] > option_places[option_digits - entered_digits]) or entered_digits == option_digits)
                    //if (working > 0 and working <= number_options) /// check not needed?
                        ret = working;
            }
        }
    }

    std::cout << std::endl;

    // restore the saved console mode
    /* Windows Specific Function */
    SetConsoleMode(hStdin, fdwSaveOldMode);

    return ret;
}

#endif // VALIDATEDINPUT_H_INCLUDED
