#include "zonemap.h"
#include <algorithm>

template<typename T>
zonemap<T>::zonemap(std::vector<T> _elements, const uint32_t _num_elements_per_zone) :
    elements(_elements), zones(elements.size() / _num_elements_per_zone), num_elements_per_zone(_num_elements_per_zone) {
}

template<typename T>
void zonemap<T>::build() {
    zones.clear();
    for (auto i = 0; i < elements.size(); i += num_elements_per_zone) {
        std::vector<T> zoneElements(elements.begin() + i, elements.begin() + i + num_elements_per_zone);
        auto min = *std::min_element(zoneElements.begin(), zoneElements.end());
        auto max = *std::max_element(zoneElements.begin(), zoneElements.end());

        zone<T> zone{zoneElements, min, max, zoneElements.size()};
        zones.push_back(zone);
    }
}

template<typename T>
void zonemap<T>::sort_elements() {
    std::sort(elements.begin(), elements.end());
}


template<typename T>
size_t zonemap<T>::query(T key) {
    size_t total = 0;
    for (auto zone : zones) {
        if (key >= zone.min && key <= zone.max) {
            // TODO: check if the zone is sorted and do a binary search
            total += std::count(zone.elements.begin(), zone.elements.end(), key);
        }
    }
    return total;
}
