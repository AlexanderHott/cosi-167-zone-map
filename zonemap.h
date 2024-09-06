#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>


template<typename T>
struct zone {
    std::vector<T> elements; // vector/list that holds elements in this zone
    T min; // minimum element in the zone
    T max; // maximum element in the zone
    size_t size; // no. of elements in current zone
};

template<typename T>
class zonemap {
    std::vector<T> elements; // vector/list that will hold all elements. Can convert to array for faster processing
    std::vector<zone<T>> zones;
    uint32_t num_zones{0}; // keeps track of the number of zones/partitions to the array/vector of elements
    uint32_t num_elements_per_zone{0};
    // keeps track of number of elements per zone (items in every zone has to be less than or equal to this counter)

public:
    /** Constructs a zonemap. The zonemap is not valid until `build` is called. */
    zonemap(std::vector<T> _elements, uint32_t _num_elements_per_zone);

    /** Builds the zonemap. */
    void build();

    /** Sorts elements. Invalidates the zonemap, so you must call `build`. */
    void sort_elements();

    /**
     * Purpose: Query a key using the zonemap
     * Param: Key to be queried
     * returns: the number of found keys
     */
    size_t query(T key);

    /**
     * Purpose: Query a range of keys using the zonemap that are between a lower and higher limit
     * Param: Lower and upper limit of range to be queried
     * returns: list of elements found using the query
     */
    std::vector<T> query(T low, T high);
};
