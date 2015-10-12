#ifndef VALIDATEDINPUT_H_INCLUDED
#define VALIDATEDINPUT_H_INCLUDED

#include <string>
#include <iostream>
#include <cast.hpp>
#include <vector>

// does not deal with INT overflows
inline unsigned int get_limited_input(const unsigned int upper_limit, const unsigned int lower_limit = 1, const std::string& input_message = "", const std::string& error_message = "") {
    if (upper_limit <= lower_limit)
        return 0;

    unsigned int ret = 0;
    std::string temp;

    do {
        ret = 0;

        if (input_message != "")
            std::cout << input_message;
        getline(std::cin, temp);

        for (unsigned int i = 0; i < temp.size(); i++)
            if (isdigit(temp.c_str()[i]))
                ret = ret * 10 + (static_cast<unsigned int>(temp.c_str()[i]) - 48);
            else
                break;

        if ((ret < lower_limit or ret > upper_limit) and error_message != "")
            std::cout << error_message << std::endl;
    } while (!(ret >= lower_limit and ret <= upper_limit));

    return ret;
}

#ifdef _WIN32_WINNT
#include <windows.h>
inline void error_exit(const std::string& error_message, const HANDLE hStdin = 0, const DWORD fdwSaveOldMode = 0, const bool exitProgram = false, const int exitCode = 1) {
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
#elif defined(_NIX) //PLACEHOLDER ONLY
inline void error_exit(const std::string& error_message, const bool exitProgram = false, const int exitCode = 1) {
    std::cerr << error_message << std::endl;

    // Only terminate the program if desired
    if (exitProgram) {
        std::cerr << "Process will now terminate\n";
        ExitProcess(exitCode);
    }
}
#endif

///this version is for returning a number for vectors
// does not deal with INT overflows; though if you have this many options you might want to reconsider what you're doing...
inline unsigned int get_list_input(const unsigned int number_options, const std::string& input_message = "  ") {
    // If we have none or one options
    if (number_options < 2)
        return number_options;

    /* Windows Specific functions */
    #ifdef _WIN32_WINNT
    // Get the standard input handle.
    HANDLE hStdin;
    DWORD fdwSaveOldMode, fdwMode;

    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    if (hStdin == INVALID_HANDLE_VALUE)
        error_exit("GetStdHandle", hStdin, fdwSaveOldMode);

    // Save the current input mode, to be restored on exit.
    if (!GetConsoleMode(hStdin, &fdwSaveOldMode))
        error_exit("GetConsoleMode", hStdin, fdwSaveOldMode);

    // Enable the window input events.
    fdwMode = ENABLE_WINDOW_INPUT;
    if (!SetConsoleMode(hStdin, fdwMode))
        error_exit("SetConsoleMode", hStdin, fdwSaveOldMode);
    #endif // _WIN32_WINNT

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
        #ifdef _WIN32_WINNT
        // allocate space for additional events incase they are captured
        char buf[10];
        DWORD read;

        // Get the next input from the console
        /* Windows Specific Function */
        ReadConsole(hStdin, buf, 1, &read, NULL);
        #endif // _WIN32_WINNT

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
                        for (unsigned int j = 0; j < entered_digits; j++)
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
                // then discard the digit
                if ((working == 0 and digit == 0) or (entered_digits + 1 == option_digits and digit > option_places[option_digits - (entered_digits + 1)]))
                    continue;

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

    /* Windows Specific Function */
    #ifdef _WIN32_WINNT
    // restore the saved console mode
    SetConsoleMode(hStdin, fdwSaveOldMode);
    #endif // _WIN32_WINNT

    return ret;
}

std::string get_list_input(const std::vector<std::string>& options, const std::string& input_message = "  ") {
    // If we have none or one options
    if (options.size() < 2) {
        if (options.size())
            return options[0];
        else
            return "";
    }

    /* Windows Specific functions */
    #ifdef _WIN32_WINNT
    // Get the standard input handle.
    HANDLE hStdin;
    DWORD fdwSaveOldMode, fdwMode;

    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    if (hStdin == INVALID_HANDLE_VALUE)
        error_exit("GetStdHandle", hStdin, fdwSaveOldMode);

    // Save the current input mode, to be restored on exit.
    if (!GetConsoleMode(hStdin, &fdwSaveOldMode))
        error_exit("GetConsoleMode", hStdin, fdwSaveOldMode);

    // Enable the window input events.
    fdwMode = ENABLE_WINDOW_INPUT;
    if (!SetConsoleMode(hStdin, fdwMode))
        error_exit("SetConsoleMode", hStdin, fdwSaveOldMode);
    #endif // _WIN32_WINNT

    // Setup input variables
    std::vector<std::string> options_ = options; //this is an uppercase copy, so we don't have to do case comparisons
    for (auto i = options_.begin(); i != options_.end(); i++)
        std::transform(i->begin(), i->end(), i->begin(), ::toupper);

    std::string input,
                ret;

    std::cout << input_message;

    while (!ret.size()) {
        #ifdef _WIN32_WINNT
        // allocate space for additional events incase they are captured
        char buf[10];
        DWORD read;

        // Get the next input from the console
        /* Windows Specific Function */
        ReadConsole(hStdin, buf, 1, &read, NULL);
        #endif // _WIN32_WINNT

        // Iterate over the characters we have recieved
        for (unsigned int i = 0; i < read; i++) {
            // If we enter a letter or number
            if (isdigit(buf[i]) or isalpha(buf[i]) or buf[i] == ' ') {
                char key;
                // make uppercase
                if (static_cast<int>(buf[i]) >= 97 and static_cast<int>(buf[i]) <= 122)
                    key = buf[i] - 32;
                else
                    key = buf[i];

                unsigned int matches = 0;
                std::string match;

                for (unsigned int j = 0; j < options_.size(); j++)
                    // if the option has a string long enough for the characters we've entered and if the character we entered is in this option, count the option as a match
                    if ((options_[j].size() > input.size()) and (options_[j].substr(0, input.size()) == input) and (options_[j][input.size()] == key)) {
                        matches++;
                        match = options[j];
                    }

                if (matches) {
                    std::cout << buf[i];
                    input += key;

                    if (matches == 1)
                        ret = match;
                }

            // If a character is entered that isn't a letter or number - simple check at the moment, can be expanded
            } else {
                // Handle the backspace character
                if (static_cast<int>(buf[i]) == 8) {
                    if (input.size() > 0) {
                        // clear the invalid character from the screen
                        std::cout << "\r" << input_message;
                        for (unsigned int j = 0; j < input.size(); j++)
                            std::cout << " ";

                        input = input.substr(0, input.size() - 1);

                        std::cout << "\r" << input_message;
                        if (input.size())
                            std::cout << input;
                    }

                // Handle the enter character
                } else if (static_cast<int>(buf[i]) == 13) {
                    // If we have a valid choice entered we accept it
                    for (unsigned int j = 0; j < options_.size(); j++)
                        if (options_[j] == input) {
                            ret = options[j];
                            break;
                        }
                }
                // Ignore any other characters
                continue;
            }
        }
    }

    std::cout << std::endl;

    /* Windows Specific Function */
    #ifdef _WIN32_WINNT
    // restore the saved console mode
    SetConsoleMode(hStdin, fdwSaveOldMode);
    #endif // _WIN32_WINNT

    return ret;
}

/*
inline std::vector<std::string> get_multiple_list_input(std::vector<std::string> options, const std::string& input_message = "  ") {
    std::vector<unsigned int> choices;
    std::string input, choice;
    std::stringstream ss;

    std::getline(std::cin, input);
    ss << input;
    unsigned int temp;

    while (ss >> choice)
        if (cast<unsigned int>(choice, temp))
            if (temp >= 1 and temp <= number_options)
                choices.push_back(temp);
//            else
//                std::cerr << "Option " << temp << " is invalid\n";
//        else
//            std::cerr << "Cannot cast '" << choice << "' to an integer\n";

//    for(auto i = choices.begin(); i != choices.end(); i++)
//        std::cout << *i << std::endl;

    std::sort(choices.begin(), choices.end());
    auto it = std::unique(choices.begin(), choices.end());
    choices.resize(std::distance(choices.begin(), it));

    return choices;
}
*/

// provides the output as vector indicies - i.e. 0 based, and supports
inline std::vector<unsigned int> get_multiple_list_input(const unsigned int number_options, const std::string& input_message = "  ") {
    std::vector<unsigned int> choices;

    // if we have 1 or 0 options
    if (number_options < 2) {
        if (number_options == 1)
            choices.push_back(0);
        return choices;
    }

    std::string input, choice;
    std::stringstream ss;

    std::getline(std::cin, input);
    ss << input;
    unsigned int temp;

    while (ss >> choice)
        if (cast<unsigned int>(choice, temp)) {
            if (temp >= 1 and temp <= number_options)
                choices.push_back(temp - 1);

        // if we enter the word 'all'
        } else if (choice == "all") {
            choices.clear();
            for (unsigned int i = 0; i < number_options; i++) {
                choices.push_back(i);
            }
            return choices;
        }
//            else
//                std::cerr << "Option " << temp << " is invalid\n";
//        else
//            std::cerr << "Cannot cast '" << choice << "' to an integer\n";

//    for(auto i = choices.begin(); i != choices.end(); i++)
//        std::cout << *i << std::endl;

    std::sort(choices.begin(), choices.end());
    auto it = std::unique(choices.begin(), choices.end());
    choices.resize(std::distance(choices.begin(), it));

    return choices;
}

#endif // VALIDATEDINPUT_H_INCLUDED
