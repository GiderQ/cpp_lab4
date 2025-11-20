#ifndef CPP_LAB4_BENCHMARK_H
#define CPP_LAB4_BENCHMARK_H

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "ThreadSafeFields.h"

template <typename FieldType>
double BenchmarkFile(const std::string& filename,
    ThreadSafeFields<FieldType>& ds) {
    std::vector<std::string> commands;
    {
        std::ifstream in(filename);
        std::string line;
        while (std::getline(in, line)) {
            commands.push_back(line);
        }
    }

    auto execTime = Timer([&commands, &ds]() {
        for (const auto& cmd : commands) {
            std::istringstream iss(cmd);
            std::string op;
            iss >> op;

            if (op == "write") {
                int field;
                FieldType val;
                iss >> field >> val;
                ds.Write(field, val);
            }
            else if (op == "read") {
                int field;
                iss >> field;
                volatile FieldType r = ds.Read(field);
            }
            else if (op == "string") {
                auto s = static_cast<std::string>(ds);
                (void)s;
            }
        }
        });

    return execTime.count();
}


#endif  // CPP_LAB4_BENCHMARK_H