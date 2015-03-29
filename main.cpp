#include <iostream>
#include <sstream>

#include "ValidatedInput.h"

// example usage
int main() {

    /** Example for Limited Input */

    // declare some input variables
    unsigned int upperLimit = 5,
                 lowerLimit = 2,
                 input = 0;
    std::stringstream ss;
    ss << "Enter number of Players (" << lowerLimit << " - " << upperLimit << "): ";
    std::string errorMessage = "Invalid number of Players\n";

    // get thge input from the user
    input = get_limited_input(upperLimit, lowerLimit, ss.str(), errorMessage);

    // display the input recieved
    std::cout << "Number input: " << input << std::endl;






    /** Example for List Input */

    //arbitrary number of options
    unsigned int options = 15;

    // print out our example options
    for (unsigned int i = 0; i < options; i++) {
        std::cout << "\t" << i+1 << ") Option " << i+1 << std::endl;
    }

    // store the selected option
    unsigned int returned = get_list_input(options);

    // print it to show our chosen option
    std::cout << "Returned value is: " << returned << std::endl;
}

