#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>
#include <vector>

#include "benchmark.h"
#include "Timer.h"
#include "ThreadSafeFields.h"
#include "Generator.h"

void GenerateFiles() {
    constexpr int count = 200000;

    const std::vector<Operation> a_ops = {
        {"read 0", 0.20}, {"write 0 1", 0.05},
        {"read 1", 0.20}, {"write 1 1", 0.05},
        {"read 2", 0.20}, {"write 2 1", 0.05},
        {"string", 0.25}
    };

    const std::vector<Operation> b_ops = {
        {"read 0", 0.20}, {"write 0 1", 0.20},
        {"read 1", 0.20}, {"write 1 1", 0.20},
        {"read 2", 0.20}, {"write 2 1", 0.20},
        {"string", 0.0}
    };

    const std::vector<Operation> c_ops = {
        {"read 0", 0.05}, {"write 0 1", 0.45},
        {"read 1", 0.05}, {"write 1 1", 0.43},
        {"read 2", 0.01}, {"write 2 1", 0.01},
        {"string", 0.0}
    };

    for (int i = 1; i <= 3; ++i) {
        GenerateFile("a_" + std::to_string(i) + ".txt", a_ops, count);
        GenerateFile("b_" + std::to_string(i) + ".txt", b_ops, count);
        GenerateFile("c_" + std::to_string(i) + ".txt", c_ops, count);
    }
}


template <typename FieldType>
void RunTestSet(const std::string& label,
    const std::vector<std::string>& files) {
    std::cout << "--- " << label << " ---\n";

    ThreadSafeFields<FieldType> ts;

    std::vector<std::thread> threads;
    std::vector<double> times(files.size());

    for (int i = 0; i < files.size(); i++) {
        threads.emplace_back(
            [&, i]() { times[i] = BenchmarkFile<FieldType>(files[i], ts); });
    }

    for (auto& t : threads) t.join();

    double sum = 0;
    for (int i = 0; i < files.size(); i++) {
        std::cout << "File " << files[i] << ": " << times[i] << " ms\n";
        sum += times[i];
    }

    std::cout << "Total time = " << sum << " ms\n\n";
}

int main() {
    std::cout << "Generating files...\n";
    GenerateFiles();

    std::cout << "Benchmarking...\n";

    using T = int;

    RunTestSet<T>("A distribution, 1 thread", { "a_1.txt" });
    RunTestSet<T>("A distribution, 2 threads", { "a_1.txt", "a_2.txt" });
    RunTestSet<T>("A distribution, 3 threads", { "a_1.txt", "a_2.txt", "a_3.txt" });

    RunTestSet<T>("B equal, 1 thread", { "b_1.txt" });
    RunTestSet<T>("B equal, 2 threads", { "b_1.txt", "b_2.txt" });
    RunTestSet<T>("B equal, 3 threads", { "b_1.txt", "b_2.txt", "b_3.txt" });

    RunTestSet<T>("C custom, 1 thread", { "c_1.txt" });
    RunTestSet<T>("C custom, 2 threads", { "c_1.txt", "c_2.txt" });
    RunTestSet<T>("C custom, 3 threads", { "c_1.txt", "c_2.txt", "c_3.txt" });

    return 0;
}