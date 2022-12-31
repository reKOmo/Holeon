#pragma once

#ifdef HOLO_DEV
#define RESOURCE_PATH(path) "game/resources/" path
#else
#define RESOURCE_PATH(path) "resources/" path
#endif // HOLO_DEV

constexpr auto SecToMs = 1000.0;

#include <random>
#include <set>


namespace Engine {
    inline void initHelpers() {
        std::srand(std::time(nullptr));
    }

	template <typename T>
    /*
        Returns index of elemetn in vector if found, -1 otherwise
    */
	int findIndex(std::vector<T> vec, T key) {
        auto it = find(vec.begin(), vec.end(), key);

        if (it != vec.end()) {
            return it - vec.begin();
        } else {
            return -1;
        }
	}

    inline float rand() {
        return (float)std::rand() / (float)(RAND_MAX);
    }
}