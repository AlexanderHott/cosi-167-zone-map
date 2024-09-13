#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

#include "zonemap.h"

// Specify your path of workload file here
std::string kInputDataPath = "./workloads/W4/workload.dat";
std::string kPointQueriesPath = "./workloads/W4/point_queries.txt";
std::string kRangeQueriesPath = "./workloads/W4/range_queries.txt";
constexpr uint32_t kRuns = 3;

using std::cout;
using std::endl;
using std::ios;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
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

int main(int argc, char *argv[]) {
    // read data
    std::ifstream ifs;
    std::vector<int> data;

    ifs.open(kInputDataPath, std::ios::binary);
    ifs.seekg(0, std::ios::end);
    std::ifstream::pos_type filesize = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    data.resize(filesize / sizeof(int));
    ifs.read(reinterpret_cast<char *>(data.data()), filesize);

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
    std::cout << "Time taken to perform point queries from zonemap = " << point_query_time * 1.0 / kRuns
              << " ns" << endl;

    // 3. ----------------------------- range queries -----------------------------
    unsigned long long range_query_time = 0;
    // Your code starts here ...

    std::cout << "Time taken to perform range query from zonemap = " << range_query_time * 1.0 / kRuns
              << " ns" << endl;
    return 0;
}
