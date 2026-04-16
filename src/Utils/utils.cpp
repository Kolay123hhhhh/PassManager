#include "utils.h"

#include <bemapiset.h>
#include <fstream>
#include <iostream>
#include <windows.h>

std::string input(std::string promt) {
    std::string value;
    std::cout << promt;
    std::getline(std::cin, value);
    return value;

}
void print(std::string promt) {
    std::cout << promt << std::endl;
}

void setColor(int color) {
    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}