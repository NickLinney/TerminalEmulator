#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <functional>

void runCommand(const std::vector<std::string>& parameters) {
    std::cout << "You commanded me to run." << std::endl;
    // Access and use the parameters as needed
}

void walkCommand(const std::vector<std::string>& parameters) {
    std::cout << "You commanded me to walk." << std::endl;
    // Access and use the parameters as needed
}

void speakCommand(const std::vector<std::string>& parameters) {
    std::cout << "You commanded me to speak." << std::endl;
    // Access and use the parameters as needed
}

int main() {
    std::unordered_map<std::string, std::function<void(const std::vector<std::string>&)>> commandMap;
    commandMap["run"] = runCommand;
    commandMap["walk"] = walkCommand;
    commandMap["speak"] = speakCommand;

    std::string input;

    while (true) {
        // Display the input prompt
        std::cout << "> ";

        // Read the user's input
        std::getline(std::cin, input);

        // Split the input into tokens
        std::vector<std::string> tokens;
        std::istringstream iss(input);
        std::string token;
        while (iss >> token) {
            tokens.push_back(token);
        }

        // Check if there's at least one token
        if (!tokens.empty()) {
            // Extract the command
            std::string command = tokens[0];

            // Extract the parameters (if any)
            std::vector<std::string> parameters(tokens.begin() + 1, tokens.end());

            // Find the command in the map and call the associated function
            auto it = commandMap.find(command);
            if (it != commandMap.end()) {
                it->second(parameters);  // Call the function with parameters
            } else if (command == "exit") {
                break;
            } else {
                std::cout << "Invalid command. Try again." << std::endl;
            }
        }
    }

    return 0;
}