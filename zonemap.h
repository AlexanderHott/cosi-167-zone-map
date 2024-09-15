/**
 * Implementation done in header file because generic classes cannot be implemented in .cpp files.
 */
#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <vector>


template<typename T>
struct zone {
    /** Elements in the `zone` */
    std::vector<T> elements;
    /** Minimum element in the `zone` */
    T min;
    /** Maximum element in the `zone` */
    T max;
};

template<typename T>
class zonemap {
    /** Number of elements per `zone` */
    uint32_t num_elements_per_zone;
    /** Elements that the `zonemap` holds */
    std::vector<T> elements; // vector/list that will hold all elements. Can convert to array for faster processing
    /** `zone`s */
    std::vector<zone<T>> zones;
    /** Whether the `zonemap` is sorted */
    bool sorted = false;

public:
    /** Constructs a zonemap, moving from `elements`. The zonemap is not valid until `build` is called.
     *
     * @param elements Elements to construct the `zonemap` over.
     * @param num_elements_per_zone Number of elements per `zone`.
     */
    zonemap(std::vector<T> &&elements, const uint32_t num_elements_per_zone) :
        num_elements_per_zone(num_elements_per_zone), elements(std::move(elements)),
        zones(elements.size() / num_elements_per_zone) {}


    /** Builds the zonemap. */
    void build() {
        zones.clear();
        zones.reserve(elements.size() / num_elements_per_zone);

        for (size_t i = 0; i < elements.size(); i += num_elements_per_zone) {
            auto start = elements.begin() + i;
            auto end = (i + num_elements_per_zone > elements.size()) ? elements.end() : start + num_elements_per_zone;

            // Avoid copying elements, work directly with iterators
            auto [min, max] = std::minmax_element(start, end);

            zones.push_back({std::vector<T>(std::make_move_iterator(start), std::make_move_iterator(end)), *min, *max});
        }
    }

    /** Returns the number of occurrences of `key` in the `zonemap` */
    size_t query(T key) {
        size_t total = 0;
        if (sorted) {
            for (auto &zone: zones) {
                auto range = std::equal_range(zone.elements.begin(), zone.elements.end(), key);
                total += std::distance(range.first, range.second);
            }
        } else {
            for (auto &zone: zones) {
                if (key >= zone.min && key <= zone.max) {
                    total += std::count(zone.elements.begin(), zone.elements.end(), key);
                }
            }
        }
        return total;
    }


    /** Returns all the occurrences of keys in the `zonemap` between `low` and `high` (inclusive). */
    std::vector<T> query(T low, T high) {
        std::vector<T> results;
        for (auto &zone: zones) {
            if (low > zone.max || high < zone.min) {
                continue;
            }

            if (sorted) {
                auto low_bound = std::lower_bound(zone.elements.begin(), zone.elements.end(), low);
                auto high_bound = std::upper_bound(zone.elements.begin(), zone.elements.end(), high);
                std::copy(low_bound, high_bound, std::back_inserter(results));
            } else {
                std::copy_if(zone.elements.begin(), zone.elements.end(), std::back_inserter(results),
                             [low, high](T key) { return key >= low && key <= high; });
            }
        }
        return results;
    }
};
