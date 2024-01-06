// COPYRIGHT 2022 Graham Stelzer
#ifndef MARKOVSIMWRITER_HPP_
#define MARKOVSIMWRITER_HPP_

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <numeric>
#include <utility>

#include <random>
#include <chrono>
#include <memory>

class MarkovSimWriter {
 public:
    // Create a Markov model of order k from given text
    // Assume that text has length at least k.
    MarkovSimWriter(std::string text, int k);

    int orderK() const;  // Order k of Markov model

    // Number of occurences of kgram in text
    // Throw an exception if kgram is not length k
    int freq(std::string kgram) const;

    // Number of times that character c follows kgram
    // if order=0, return num of times that char c appears
    // (throw an exception if kgram is not of length k)
    int freq(std::string kgram, char c) const;

    // Random character following given kgram
    // (throw an exception if kgram is not of length k)
    // (throw an exception if no such kgram)
    char kRand(std::string kgram);

    // Generate a string of length L characters by simulating a trajectory
    // through the corresponding Markov chain. Thza2e first k characters of
    // the newly generated string should be the argument kgram.
    // Throw an excpetion if kgram is not of length k.
    // Assume that L is at least k
    std::string generate(std::string kgram, int L);

 private:
    std::map<std::string, std::vector<std::pair<char, int> > > symbolTable;
    int _k;
};


    // Overload the stream insertion operator << and display the internal state
    // of the Markov model. Print out the order, alphabet, and the frequencies
    // of the k-grams and k+1-



MarkovSimWriter::MarkovSimWriter(std::string text, int k) {
    _k = k;
    std::string temp;
    size_t length = text.size();
    char nextChar;

    for (size_t i = 0; i < length; i++) {
        if (i + k >= length) {
            temp = text.substr(i, k) + text.substr(0, (i + k) % length);
            nextChar = text[(i + k) % length];
        } else {
            temp = text.substr(i, k);
            nextChar = text[i + k];
        }

        if (symbolTable.find(temp) != symbolTable.end()) {
            bool found = false;
            // loop through the vector at temp and increment the 2nd value of the pair stored at the temp value
            for (size_t j = 0; j < symbolTable[temp].size(); j++) {
                if (symbolTable[temp][j].first == nextChar) {
                    symbolTable[temp][j].second++;
                    found = true;
                }
            }
            if (!found) {
                symbolTable[temp].push_back(std::make_pair(nextChar, 1));
            }
        } else {
            symbolTable[temp].push_back(std::make_pair(nextChar, 1));
        }
    }
}

int MarkovSimWriter::orderK() const {
    return _k;
}

int MarkovSimWriter::freq(std::string kgram) const {
    // return number of times kgram appears in the text
    // (throw an exception if kgram is not of length k)
    if (kgram.size() != static_cast<size_t>(_k)) {
        throw std::invalid_argument("kgram is not of length k");
    }

    if (symbolTable.find(kgram) != symbolTable.end()) {
        // kgram is in the map
        int count = 0;
        for (size_t i = 0; i < symbolTable.at(kgram).size(); i++) {
            count += symbolTable.at(kgram)[i].second;
        }
        return count;
    } else {
        // kgram is not in the map
        return 0;
    }
}

int MarkovSimWriter::freq(std::string kgram, char c) const {
    // return the number of times c follows the kgram
    if (kgram.size() != static_cast<size_t>(_k)) {
        throw std::invalid_argument("kgram is not of length k");
    }
    int count;
    if (symbolTable.find(kgram) != symbolTable.end()) {
        // kgram is in the map

        // find the char in the vector
        auto it = std::find_if(symbolTable.at(kgram).begin(), symbolTable.at(kgram).end(),
        [c](std::pair<char, int> p) { return p.first == c; });
        
        if (it != symbolTable.at(kgram).end()) {
            count = it->second;
        }
    } else {
        // kgram is not in the map
        count = 0;
    }
    return count;
}

char MarkovSimWriter::kRand(std::string kgram) {
    // generate a random char following the kgram
    if (kgram.size() != static_cast<size_t>(_k)) {
        throw std::invalid_argument("kgram is not of length k");
    }
    // (throw an exception if no such kgram)
    if (symbolTable.find(kgram) == symbolTable.end()) {
        throw std::invalid_argument("no such kgram");
    }

    int sum = std::accumulate(symbolTable.at(kgram).begin(),
    symbolTable.at(kgram).end(), 0,
    [](int sum, std::pair<char, int> p) { return sum + p.second; });

    unsigned int seed
    = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 g2(seed);  // Mersenne Twister
    std::uniform_int_distribution<int> dist(0, sum);
    int rand = (dist(g2) % sum);
    // std::cout << "rand value is: " << rand << std::endl;

    for (size_t i = 0; i < symbolTable.at(kgram).size(); i++) {
        rand -= symbolTable.at(kgram)[i].second;
        if (rand < 0) {
            return symbolTable.at(kgram)[i].first;
        }
    }
    return ' ';  // should never reach here
}

std::string MarkovSimWriter::generate(std::string kgram, int L) {
    if (kgram.size() != static_cast<size_t>(_k)) {
        throw std::invalid_argument("kgram is not of length k");
    }
    if (L < _k) {
        throw std::invalid_argument("L is less than k");
    }

    std::string output = kgram;
    for (int i = 0; i < L - _k; i++) {
        // if (i % 50 == 0) {
        //     output += '\n';
        // }
        output += kRand(kgram);
        kgram = output.substr(output.size() - _k, _k);
    }
    return output;
}

#endif  // MARKOVSIMWRITER_HPP_
