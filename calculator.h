// If CALCULATOR_H is not defined
#ifndef CALCULATOR_H

// Define CALCULATOR_H
#define CALCULATOR_H

// Includes the string library to use std::string
#include <string>

// Declaration of the Calculator class
class Calculator {
public:

    // Starts the calculator application and handles user interaction
    void run();

private:

    // Displays the introduction and instructions to the user
    void displayIntro();

    // Prompts the user for input and returns the response as a string
    std::string getInput(const std::string& prompt);

    // Gets a double value from user input
    double getDoubleInput(const std::string& prompt);

    // Analyzes user input to separate the first number, operator, and second number
    bool parseInput(const std::string& input, double& operand1, char& operatorSymbol, double& operand2);

    // Analyzes input to separate an operator and the second number if given
    bool parseOperatorAndOperand(const std::string& input, char& operatorSymbol, double& operand2);

    // Performs the calculation based on the provided numbers and operator
    double calculate(double num1, char op, double num2);

    // Clears the input buffer to handle invalid inputs
    void clearInputBuffer();

    // Asks the user if they want to perform another calculation
    bool continueCalculation();
};
// End of the condition for CALCULATOR_H
#endif 