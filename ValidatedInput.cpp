#include "ValidatedInput.h"
#include <iostream>


// does not deal with INT overflows
unsigned int getLimitedInput(unsigned int upper_limit, unsigned int lower_limit, std::string input_message, std::string error_message, char delimiter) {
    unsigned int ret;

    std::string temp;

    while (!(ret >= lower_limit and ret <= upper_limit)) {
        if (input_message != "") {
            std::cout << input_message;
        }

        ret = 0;

        getline(std::cin, temp, delimiter);

        for (unsigned int i = 0; i < temp.size(); i++) {
            if (isdigit(temp.c_str()[i])) {
                ret = ret * 10 + (static_cast<int>(temp.c_str()[i]) - 48);
            } else {
                break;
            }
        }

        if (!(ret >= lower_limit and ret <= upper_limit) and error_message != "") {
            std::cout << error_message << std::endl;
        }
    }
    return ret;
}


void ErrorExit (const std::string error_message, const HANDLE hStdin, const DWORD fdwSaveOldMode, const bool exitProgram = false, const int exitCode = 0) {
    std::cerr << error_message << std::endl << "Process will now terminate\n";

    // Restore input mode on exit.
    /* Windows Specific Function*/
    SetConsoleMode(hStdin, fdwSaveOldMode);

    // Only terminate the progtam if desired
    if (exitProgram) {
        ExitProcess(exitCode);
    }
}

// does not deal with INT overflows
unsigned int getListInput(unsigned int number_options, std::string input_message) {
    HANDLE hStdin;
    DWORD fdwSaveOldMode, fdwMode;

    /* Windows Specific functions */
    // Get the standard input handle.
    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    if (hStdin == INVALID_HANDLE_VALUE)
        ErrorExit("GetStdHandle", hStdin, fdwSaveOldMode);

    // Save the current input mode, to be restored on exit.
    if (! GetConsoleMode(hStdin, &fdwSaveOldMode) )
        ErrorExit("GetConsoleMode", hStdin, fdwSaveOldMode);

    // Enable the window input events.
    fdwMode = ENABLE_WINDOW_INPUT;
    if (! SetConsoleMode(hStdin, fdwMode) )
        ErrorExit("SetConsoleMode", hStdin, fdwSaveOldMode);

    // Setup the variables
    unsigned int temp = number_options,
                 option_digits = 0,
                 option_places[20] = {0};

    // store the digits for quick access
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

        for (unsigned int i = 0; i < read; i++) {
            if (!isdigit(buf[i])) {
                std::cout << (unsigned int)buf[i] << std::endl;
                if (static_cast<int>(buf[i]) == 8) {
                    if (entered_digits > 0) {
                        working = (working - (working % 10)) / 10;
                        working_places[entered_digits] = 0;

                        std::cout << "\r" << input_message;
                        for (unsigned int i = 0; i < entered_digits; i++)
                            std::cout << " ";

                        std::cout << "\r" << input_message;
                        if (working)
                            std::cout << working;

                        entered_digits--;
                    }
                } else if (static_cast<int>(buf[i]) == 13) {
                    if (working > 0 and working <= number_options) {
                        ret = working;
                    }
                }
                continue;
            } else {
                unsigned int digit = static_cast<int>(buf[i]) - 48;

                if ((working == 0 and digit == 0) or working > number_options or (entered_digits + 1 == option_digits and digit > option_places[option_digits - (entered_digits + 1)]))
                    break;

                working = working * 10 + digit;
                entered_digits++;
                working_places[entered_digits] = digit;

                std::cout << digit;

                if ((entered_digits + 1 == option_digits and working_places[entered_digits] > option_places[option_digits - entered_digits]) or entered_digits == option_digits) {
                    if (working > 0 and working <= number_options) {
                        ret = working;
                    }
                }
            }
        }
    }

    std::cout << std::endl;
    return ret;
}

