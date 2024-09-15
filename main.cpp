#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <iterator>
#include <random>
#include <vector>

#include "zonemap.h"

// Specify your path of workload file here
std::string kInputDataPath = "./workloads/W5/workload.dat";
std::string kPointQueriesPath = "./workloads/W5/point_queries.txt";
std::string kRangeQueriesPath = "./workloads/W5/range_queries.txt";
constexpr uint32_t kRuns = 3;

using std::cout;
using std::endl;
using std::ios;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::nanoseconds;

void loadPointQueries(const std::string &input_queries_path, std::vector<int> &queries) {
    queries.clear();
    std::ifstream infile(input_queries_path, ios::in);
    int tmp;
    while (infile >> tmp) {
        queries.push_back(tmp);
    }
    // shuffle indexes
    std::shuffle(queries.begin(), queries.end(), std::mt19937(std::random_device()()));
}

/** Load a file of int,int into a vector */
std::vector<std::pair<int, int>> getRangeQueries(const std::string &input_queries_path) {
    // TODO: reserve size
    std::vector<std::pair<int, int>> queries;
    std::ifstream infile(input_queries_path, ios::in);
    int k1,k2;
    char comma;
    while (infile >> k1 >> comma >> k2) {
        queries.emplace_back(k1, k2);
    }
}

std::vector<int> loadData(const std::string &input_queries_path) {
    std::ifstream file(input_queries_path, std::ios::binary);
    std::vector<int> data;

    file.seekg(0, std::ios::end);
    const std::streampos fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    data.resize(fileSize / sizeof(int));
    file.read(reinterpret_cast<char *>(data.data()), fileSize); // Read raw binary data
    return data;
}


int main(int argc, char *argv[]) {
    std::vector<int> data = loadData(kInputDataPath);

    // 1. ----------------------------- initialize zonemap and build -----------------------------
    // build zonemap
    zonemap zones(data, static_cast<uint32_t>(data.size()) / 100);
    zones.sort_elements();
    zones.build();

    // 2. ----------------------------- point queries -----------------------------
    std::vector<int> queries;
    loadPointQueries(kPointQueriesPath, queries);

    auto start_pq = high_resolution_clock::now();
    for (size_t r = 0; r < kRuns; r++) {
        for (int pq: queries) {
            // query from zonemaps here
            zones.query(pq);
        }

        // shuffle indexes
        std::shuffle(queries.begin(), queries.end(), std::mt19937(std::random_device()()));
    }
    auto stop_pq = high_resolution_clock::now();
    auto duration_pq = duration_cast<nanoseconds>(stop_pq - start_pq);
    auto point_query_time = duration_pq.count();
    std::cout << "Time taken to perform point queries from zonemap = " << point_query_time * 1.0 / kRuns << " ns"
              << endl;

    // 3. ----------------------------- range queries -----------------------------
    unsigned long long range_query_time = 0;
    for (size_t r = 0; r < kRuns; r++) {
        for
    }

    std::cout << "Time taken to perform range query from zonemap = " << range_query_time * 1.0 / kRuns << " ns" << endl;
    return 0;
}
