// COPYRIGHT 2022 Graham Stelzer

#include <iostream>
#include <string>
#include <fstream>

#include "MarkovSimWriter.hpp"

int main(int argc, char* argv[]) {
    // Get k and L values
    int k = std::stoi(argv[1]);
    int L = std::stoi(argv[2]);
    // read inputfile into string
    std::ifstream file(argv[3]);
    std::string line;
    std::string text;
    while (std::getline(file, line)) {
        text += line;
    }
    // Create MarkovSimWriter object
    MarkovSimWriter writer(text, k);
    // Generate L random strings
    std::string start = text.substr(0, k);

    std::ofstream outFile("output.txt");
    std::string output = writer.generate(start, L);
    for (unsigned int i = 0; i < output.size(); i++) {
        if (i % 90 == 0) {
            output.insert(i, "\n");
        }
    }
    outFile << output;
    // std::cout << writer.generate(start, L) << std::endl;

    // std::string text;
    // std::cin >> text;
    // std::cout << text << std::endl;

    // int k = std::stoi(argv[1]);
    // int L = std::stoi(argv[2]);

    // // create a MarkovSimWriter object with order 3
    // MarkovSimWriter rw(text, k);

    // std::string start = text.substr(0, k);
    // std::cout << rw.generate(start, L) << std::endl;

    return 0;
}
