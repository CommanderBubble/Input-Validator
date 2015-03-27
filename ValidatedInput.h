#ifndef VALIDATEDINPUT_H_INCLUDED
#define VALIDATEDINPUT_H_INCLUDED

#include <string>
#include <windows.h>

unsigned int getListInput(const unsigned int, const std::string = "  ");
unsigned int getLimitedInput(const unsigned int , const unsigned int = 1, const std::string = "", const std::string = "", const char = '\n');

#endif // VALIDATEDINPUT_H_INCLUDED
