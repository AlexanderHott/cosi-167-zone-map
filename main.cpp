#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

#include "zonemap.h"

using std::cout;
using std::endl;
using std::ios;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::nanoseconds;

struct WorkloadResults {
    /** Build time in ns */
    long build_time;
    /** Point query time in ns */
    long double pq_time;
    /** Range query time in ns */
    long double rq_time;
};

class Workload {
    std::vector<int> data;
    std::vector<int> pointQueries;
    std::vector<std::pair<int, int>> rangeQueries;
    uint32_t runs;

    static std::vector<int> getData(const std::string &input_queries_path) {
        std::ifstream file(input_queries_path, std::ios::binary);
        std::vector<int> data;

        file.seekg(0, std::ios::end);
        const std::streampos fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        data.resize(fileSize / sizeof(int));
        file.read(reinterpret_cast<char *>(data.data()), fileSize); // Read raw binary data
        return data;
    }

    static std::vector<int> getPointQueries(const std::string &input_queries_path) {
        std::vector<int> queries;
        std::ifstream infile(input_queries_path, ios::in);
        int tmp;
        while (infile >> tmp) {
            queries.push_back(tmp);
        }
        // shuffle indexes
        std::shuffle(queries.begin(), queries.end(), std::mt19937(std::random_device()()));
        return queries;
    }

    static std::vector<std::pair<int, int>> getRangeQueries(const std::string &input_queries_path) {
        // TODO: reserve size
        std::vector<std::pair<int, int>> queries;
        std::ifstream infile(input_queries_path, ios::in);
        int k1, k2;
        char comma;
        while (infile >> k1 >> comma >> k2) {
            queries.emplace_back(k1, k2);
        }
        return queries;
    }

public:
    bool sorted;

    Workload(const std::string &dataPath, const std::string &pointQueryPath, const std::string &rangeQueryPath,
             const uint32_t runs, const bool sorted) :
        data(getData(dataPath)), pointQueries(getPointQueries(pointQueryPath)),
        rangeQueries(getRangeQueries(rangeQueryPath)), runs(runs), sorted(sorted) {}

    WorkloadResults run() {
        const auto build_start = high_resolution_clock::now();
        zonemap zones((std::move(data)), static_cast<uint32_t>(data.size()) / 100, sorted);
        zones.build();
        const auto build_stop = high_resolution_clock::now();
        const auto build_duration = duration_cast<nanoseconds>(build_stop - build_start);

        const auto pq_start = high_resolution_clock::now();
        for (size_t r = 0; r < runs; r++) {
            for (const int key: pointQueries) {
                zones.query(key);
            }

            // shuffle indexes
            std::shuffle(pointQueries.begin(), pointQueries.end(), std::mt19937(std::random_device()()));
        }
        const auto pq_stop = high_resolution_clock::now();
        const auto pq_duration = duration_cast<nanoseconds>(pq_stop - pq_start);
        // std::cout << "Time taken to perform point queries from zonemap = " << point_query_time * 1.0 / kRuns << " ns"
        //           << endl;

        const auto rq_start = high_resolution_clock::now();
        for (size_t r = 0; r < runs; r++) {
            for (auto [start, end]: rangeQueries) {
                zones.query(start, end);
            }
        }
        const auto rq_end = high_resolution_clock::now();
        const auto rq_duration = duration_cast<nanoseconds>(rq_end - rq_start);

        // std::cout << "Time taken to perform range query from zonemap = " << range_query_time * 1.0 / kRuns << " ns"
        //           << endl;
        return WorkloadResults{build_duration.count(), pq_duration.count() * 1.0L / runs,
                               rq_duration.count() * 1.0L / runs};
    }
};


int main(int argc, char *argv[]) {
    std::vector workloads{
            Workload("./workloads/W1/workload.dat", "./workloads/W1/point_queries.txt",
                     "./workloads/W1/range_queries.txt", 3, true),
            Workload("./workloads/W2/workload.dat", "./workloads/W2/point_queries.txt",
                     "./workloads/W2/range_queries.txt", 3, true),
            Workload("./workloads/W3/workload.dat", "./workloads/W3/point_queries.txt",
                     "./workloads/W3/range_queries.txt", 3, true),
            // Workload("./workloads/W4/workload.dat", "./workloads/W4/point_queries.txt",
            //          "./workloads/W4/range_queries.txt", 3, false),
            // Workload("./workloads/W5/workload.dat", "./workloads/W5/point_queries.txt",
            //          "./workloads/W5/range_queries.txt", 3, false),
            // Workload("./workloads/W6/workload.dat", "./workloads/W6/point_queries.txt",
            //          "./workloads/W6/range_queries.txt", 3, false),
    };
    for (auto workload: std::move(workloads)) {
        auto [build_time, pq_time, rq_time] = workload.run();
        std::cout << "Time taken to build zonemap                      = " << std::to_string(build_time) << " ns"
                  << endl;
        std::cout << "Time taken to perform point queries from zonemap = " << std::to_string(pq_time) << " ns" << endl;
        std::cout << "Time taken to perform range queries from zonemap = " << std::to_string(rq_time) << " ns" << endl;
    }


    return 0;
}
