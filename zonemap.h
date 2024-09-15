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
    /** Constructs a zonemap. The zonemap is not valid until `build` is called.
     *
     * @param elements Elements to construct the `zonemap` over.
     * @param num_elements_per_zone Number of elements per `zone`.
     */
    zonemap(std::vector<T> elements, const uint32_t num_elements_per_zone) :
        num_elements_per_zone(num_elements_per_zone), elements(elements),
        zones(elements.size() / num_elements_per_zone) {}

    /** Builds the zonemap. */
    void build() {
        zones.clear();
        for (size_t i = 0; i < elements.size(); i += num_elements_per_zone) {
            std::vector<T> zoneElements(elements.begin() + i, elements.begin() + i + num_elements_per_zone);
            auto min = *std::min_element(zoneElements.begin(), zoneElements.end());
            auto max = *std::max_element(zoneElements.begin(), zoneElements.end());

            zone<T> zone{zoneElements, min, max};
            zones.push_back(zone);
        }
    }

    /** Sorts elements. Invalidates the zonemap, so you must call `build` before querying. */
    void sort_elements() {
        std::sort(elements.begin(), elements.end());
        sorted = true;
    }

    /** Returns the number of occurrences of `key` in the `zonemap` */
    size_t query(T key) {
        size_t total = 0;
        if (sorted) {
            for (zone<T> &zone: zones) {
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
        if (sorted) {
            for (zone<T> &zone: zones) {
                std::find_if(zone.elements.begin(), zone.elements.end(),
                             [low, high](T key) { return key >= low && key <= high; });
            }
        } else {
            for (zone<T> &zone: zones) {
                if (low > zone.max || high < zone.min) {
                    continue;
                }
                // TODO: check if the zone is sorted and use `std::find_if`
                std::copy_if(zone.elements.begin(), zone.elements.end(), std::back_inserter(results),
                             [low, high](T key) { return key >= low && key <= high; });
            }
        }
        return results;
    }
};
