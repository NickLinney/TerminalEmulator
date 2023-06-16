#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include <stdlib.h>
#include "json.hpp"
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

using json = nlohmann::json;

void executeCommand(const std::vector<std::string>& parameters) {
    std::string commandName = parameters[0];

    // Get the Mongo URI from Replit Secrets
    char* mySecret = getenv("MONGO_URI");

    // Connect to the MongoDB Atlas database
    mongocxx::instance instance{};
    mongocxx::uri uri(mySecret);
    mongocxx::client client(uri);

    // Access the "commands" collection
    mongocxx::database db = client["MUDTerm"];
    mongocxx::collection collection = db["commands"];

    // Prepare the query to find the command by name
    bsoncxx::builder::stream::document filter_builder{};
    filter_builder << "name" << commandName;
    bsoncxx::document::value filter_value = filter_builder.view();

    // Find the command document in the collection
    bsoncxx::stdx::optional<bsoncxx::document::value> result = collection.find_one(filter_value);
    if (result) {
        bsoncxx::document::view document_view = result.value().view();
        std::string execValue = document_view["exec"].get_utf8().value.to_string();
        std::cout << execValue << std::endl;
    } else {
        std::cout << "Command not found." << std::endl;
    }
}

int main() {
    std::unordered_map<std::string, std::function<void(const std::vector<std::string>&)>> commandMap;
    commandMap["execute"] = executeCommand;

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

            // Check if the command is "exit"
            if (command == "exit") {
                break;
            }

            // Extract the parameters (excluding the command)
            std::vector<std::string> parameters(tokens.begin() + 1, tokens.end());

            // Find the command in the map and call the associated function
            auto it = commandMap.find(command);
            if (it != commandMap.end()) {
                it->second(parameters);  // Call the function with parameters
            } else {
                std::cout << "Invalid command. Try again." << std::endl;
            }
        }
    }

    return 0;
}
