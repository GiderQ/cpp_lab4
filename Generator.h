#ifndef CPP_LAB4_GENERATOR_H
#define CPP_LAB4_GENERATOR_H

#include <fstream>
#include <random>

struct Operation {
    std::string text;
    double probability;
};

inline void GenerateFile(const std::string& filename, const std::vector<Operation>& ops, const int count) {
    std::ofstream out(filename);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0.0, 1.0);

    for (int i = 0; i < count; ++i) {
        const double r = dist(gen);
        double cumulative = 0.0;

        for (const auto& [text, probability] : ops) {
            cumulative += probability;
            if (r <= cumulative) {
                out << text << "\n";
                break;
            }
        }
    }
}

#endif  // CPP_LAB4_GENERATOR_H