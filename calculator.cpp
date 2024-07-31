//Calculator class declaration
#include "Calculator.h"

//Standard input / output library
#include <iostream>

//Limits for stream size
#include <limits>

//Library for parsing and processing strings
#include <sstream>

//Character handling functions
#include <cctype>

// Handling classes for managing errors
#include <stdexcept>

// Helper function to check if a string is a valid non-negative number
bool isValidNonNegativeNumber(const std::string& str) {
    std::istringstream ss(str);
    double d;
    char c;

    // Check if the string can be converted to a number and contains no extra characters
    return ss >> d && !(ss >> c) && d >= 0 && str.find_first_not_of("0123456789.") == std::string::npos;
}

// Helper function to check if a character is a valid operator
bool isValidOperator(char op) {

    // Valid operators are +, -, *, and /
    return (op == '+' || op == '-' || op == '*' || op == '/');
}

// Function to validate the first input
bool isValidFirstInput(const std::string& input) {
    // Check if the input is a valid number without signs
    if (isValidNonNegativeNumber(input)) {
        return true;
    }

    // Check if the input is a valid equation (e.g., 6+3)
    std::stringstream ss(input);
    double op1, op2;
    char oper;

    //Parse the input as two numbers separated by an operator
    if (ss >> op1 >> oper >> op2) {

        // Ensure the operator is valid and both operands are non-negative
        return isValidOperator(oper) && op1 >= 0 && op2 >= 0;
    }

    // Disallow standalone signs and incorrect formats
    return false;
}

// Function to run the calculator, handling user input and calculations
void Calculator::run() {

    // Display an introduction message to the user
    displayIntro();

    // Main calculator loop, runs until the user chooses to exit
    while (true) {

        // Prompt the user for input
        std::string input = getInput("Enter an equation (e.g., 6+3) or the first number: ");

        // Validate the first input using the helper function isValidFirstInput
        if (!isValidFirstInput(input)) {

            // Display an error message if the input is invalid
            std::cout << "Invalid input. You must enter a valid non-negative number or a complete equation (e.g., 6+3).\n\n";

            // Go to the next iteration of the loop if input is invalid
            continue;
        }

        // Variables to store the operands and operator
        double operand1, operand2;
        char operatorSymbol;

        // Check if the input is an equation (contains both operands and operator)
        bool isEquation = parseInput(input, operand1, operatorSymbol, operand2);

        // If the input is not an equation, it's treated as the first operand
        if (!isEquation) {

            // Convert the input string to a double and store it in operand1
            operand1 = std::stod(input);

            std::string operatorAndOperand2;
            bool operatorOnly = false;

            // Loop to get a valid operator and second operand from the user
            while (true) {
                operatorAndOperand2 = getInput("Enter an operator (+, -, *, /) followed by the second number or just an operator: ");

                // Handle empty input
                if (operatorAndOperand2.empty()) {
                    std::cout << "Invalid input. You must enter an operator.\n\n";
                    continue;
                }

                // Check if only an operator was entered
                if (operatorAndOperand2.length() == 1 && isValidOperator(operatorAndOperand2[0])) {
                    operatorSymbol = operatorAndOperand2[0];
                    operatorOnly = true;
                    break;
                }

                // Check if both an operator and operand were entered correctly
                else if (parseOperatorAndOperand(operatorAndOperand2, operatorSymbol, operand2)) {
                    break;
                }
                
                // Display an error message if the input is invalid
                std::cout << "Error! Please enter a valid operator followed by a number (e.g., +3) or just an operator.\n\n";
            }

            // If only an operator was entered, ask for the second number
            if (operatorOnly) {
                operand2 = getDoubleInput("Enter the second number: ");
            }
        }

        // Perform the calculation based on the input
        try {
            double result = calculate(operand1, operatorSymbol, operand2);

            // Display the result of the calculation to the user
            std::cout << operand1 << " " << operatorSymbol << " " << operand2 << " = " << result << "\n";
            std::cout << "Total: " << result << "\n\n";
        }

        // Handle invalid argument exceptions, such as division by zero or invalid operators
        catch (const std::invalid_argument& e) {
            std::cout << "ERROR! " << e.what() << "\n\n";
            continue;
        }

        // Handle any other unexpected exceptions
        catch (const std::exception& e) {
            std::cout << "An unexpected error occurred: " << e.what() << "\n\n";
            continue;
        }

        // Ask the user if they want to perform another calculation
        if (!continueCalculation()) {
            std::cout << "Thank you for using the calculator. Goodbye!\n";
            break;
        }
        else {
            // Ask if the user wants to clear the screen
            while (true) {
                std::string clearScreenChoice = getInput("Do you want to clear the screen from previous calculations? (y/n): ");

                // Clear the screen if the user chooses 'y' or 'Y'
                if (clearScreenChoice == "y" || clearScreenChoice == "Y") {
                    std::cout << "\033[2J\033[1;1H"; // ANSI escape codes to clear the screen
                    break;
                }

                // Don't clear the screen if the user chooses 'n' or 'N'
                else if (clearScreenChoice == "n" || clearScreenChoice == "N") {
                    break;
                }

                // Handle invalid input for the clear screen choice
                else {
                    std::cout << "Invalid choice. Please enter 'y' or 'n'.\n";
                }
            }
            displayIntro(); // Display instructions again if the user wants to calculate again
        }
    }
}

// Displays the introduction message and instructions for the calculator
void Calculator::displayIntro() {
    std::cout << "Hello! Welcome to the basic calculator program.\n";
    std::cout << "This program supports the following operations:\n";
    std::cout << "  Addition (+)\n";
    std::cout << "  Subtraction (-)\n";
    std::cout << "  Multiplication (*)\n";
    std::cout << "  Division (/)\n\n";
    std::cout << "Please follow the prompts to enter numbers and choose an operation.\n";
    std::cout << "You can use either the number keypad or the numbers above the letter keys for entering numbers.\n";
    std::cout << "To perform a calculation, follow these steps:\n";
    std::cout << "1. Enter an equation (e.g., 6+3) or the first number when prompted.\n";
    std::cout << "2. If you entered a number, enter an operator followed by the second number (e.g., +3) or just an operator when prompted.\n";
    std::cout << "The program will then display the result of the calculation.\n\n";
}

// Function to get user input as a string
std::string Calculator::getInput(const std::string& prompt) {
    
    //Store the user input
    std::string input;

    // Display the prompt message
    std::cout << prompt;
    std::getline(std::cin, input);

    // Return the user input
    return input;
}

// Function to get user input as a double, with validation and error handling
double Calculator::getDoubleInput(const std::string& prompt) {
    
    //Store the converted number
    double value;

    // Loop to keep prompting for input until a valid number is entered
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            clearInputBuffer();
            return value;
        }
        else {

            //error message if the input is not a valid number
            std::cout << "Invalid input. Please enter a number.\n";

            //// Clear the input to prepare for the next input attempt
            clearInputBuffer();
        }
    }
}

// Function to parse user input and determine if it's an equation or just a number
bool Calculator::parseInput(const std::string& input, double& operand1, char& operatorSymbol, double& operand2) {
    
    // Creates a string stream to parse the input
    std::stringstream ss(input);

    // Temporary character variable
    char dummy;

    if (ss >> operand1 >> operatorSymbol >> operand2) {

        // Check if the extracted operator is valid and both operands are non-negative
        if (isValidOperator(operatorSymbol) && operand1 >= 0 && operand2 >= 0) {

            // Indicate successful of an equation
            return true;
        }
    }

    //// Reset the stringstream and attempt to parse input as operand1, operatorSymbol, and dummy character
    ss.clear();
    ss.str(input);
    if (ss >> operand1 >> operatorSymbol >> dummy) {

        // If a valid operator is found
        if (isValidOperator(operatorSymbol)) {
            std::string secondNumberString = input.substr(input.find(operatorSymbol) + 1);
            operand2 = std::stod(secondNumberString);

            return true;
        }
    }
    // If parsing as an equation fails
    return false;
}

// Function to parse the input string for an operator and a second operand
bool Calculator::parseOperatorAndOperand(const std::string& input, char& operatorSymbol, double& operand2) {

    // Creates a string stream to parse the input
    std::stringstream ss(input);
    if (ss >> operatorSymbol >> operand2) {

        // If the extracted operator is valid
        if (isValidOperator(operatorSymbol)) {
            return true;
        }
    }

    // If the input string is a single character and is a valid operator
    else if (input.length() == 1 && isValidOperator(input[0])) {
        operatorSymbol = input[0];
        return true;
    }
    // If parsing fails
    return false;
}

// Function to perform the calculation based on the provided operands and operator
double Calculator::calculate(double num1, char op, double num2) {
    
    //Switch statement to perform the calculation based on the operator
    switch (op) {
    case '+':
        return num1 + num2;
    case '-':
        return num1 - num2;
    case '*':
        return num1 * num2;
    case '/':
        if (num2 == 0) {
            throw std::invalid_argument("Division by zero is not allowed!");
        }
        return num1 / num2;

        // Invalid operator
    default:
        throw std::invalid_argument("Error: Invalid operator!");
    }
}

// Function to clear the input and reset the input 
void Calculator::clearInputBuffer() {
    
    // Clear error flags
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

//Ask the user if they want to perform another calculation
bool Calculator::continueCalculation() {

    // Loop until a valid input ('y' or 'n') is received
    while (true) {
        std::cout << "Do you want to calculate again? (y/n): ";
        std::string choice;
        std::getline(std::cin, choice);

        // If the user wants to continue
        if (choice == "y" || choice == "Y") {
            return true;
        }
        
        // If the user wants to quit
        else if (choice == "n" || choice == "N") {
            return false;
        }

        // If the input is invalid
        else {
            std::cout << "Invalid choice. Please enter 'y' or 'n'.\n";
        }
    }
}
