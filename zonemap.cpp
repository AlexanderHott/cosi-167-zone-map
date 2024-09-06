#include "zonemap.h"

template<typename T>
zonemap<T>::zonemap(std::vector<T> _elements, const uint _num_elements_per_zone) :
    elements(_elements), num_elements_per_zone(_num_elements_per_zone) {
}

template<typename T>
void zonemap<T>::build() {
    for (auto i = 0; i < elements.size(); i += num_elements_per_zone) {
        std::vector<T> zoneElements(elements.begin() + i, elements.begin() + i + num_elements_per_zone);
        auto min = *std::min_element(zoneElements.begin(), zoneElements.end());
        auto max = *std::max_element(zoneElements.begin(), zoneElements.end());

        zone<T> zone{min, max, zoneElements, zoneElements.size()};
        zones.push_back(zone);
    }
}


template<typename T>
size_t zonemap<T>::query(T key) {
    // Your code starts here ...
    return 0;
}
