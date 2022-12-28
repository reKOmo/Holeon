#pragma once
#include "raylib-cpp.hpp"
#include "Helpers.h"
#include <map>
#include <vector>
#include <set>

namespace Engine {
    template <class V>
    class SpatialHashGrid {
    public:
        SpatialHashGrid(raylib::Rectangle bounds) : bounds(bounds) {
            chunkedSize.x = bounds.width / cellSize;
            chunkedSize.y = bounds.height / cellSize;
        }

        void addObject(raylib::Rectangle key, V val) {
            raylib::Vector2 ltPos = getGridPosition({ key.x, key.y });
            raylib::Vector2 rbPos = getGridPosition({ key.x + key.width, key.y + key.height });

            for (int i = ltPos.x; i <= rbPos.x; i++) {
                for (int j = ltPos.y; j <= rbPos.y; j++) {
                    int hash = getHashPos({ (float)i, (float)j });
                    auto& elem = grid[hash];
                    elem.insert(val);
                }
            }
        }

        void removeObject(raylib::Rectangle key, V val) {
            raylib::Vector2 ltPos = getGridPosition({ key.x, key.y });
            raylib::Vector2 rbPos = getGridPosition({ key.x + key.width, key.y + key.height });

            for (int i = ltPos.x; i <= rbPos.x; i++) {
                for (int j = ltPos.y; j <= rbPos.y; j++) {
                    int hash = getHashPos({ (float)i, (float)j });
                    auto& vec = grid[hash];
                    vec.erase(val);
                }
            }
        }

        std::set<V> getObjectsInRange(raylib::Vector2 pos, int range) {
            raylib::Vector2 ltPos = getGridPosition({ pos.x - (range / 2), pos.y - (range / 2) });
            raylib::Vector2 rbPos = getGridPosition({pos.x + (range / 2), pos.y + (range / 2)});

            std::set<V> found;

            for (int i = ltPos.x; i <= rbPos.x; i++) {
                for (int j = ltPos.y; j <= rbPos.y; j++) {
                    int hash = getHashPos({ (float)i, (float)j });
                    for (auto& r : grid[hash]) {
                        found.insert(r);
                    }
                }
            }

            return found;
        }

    private:
        raylib::Rectangle bounds;
        raylib::Vector2 chunkedSize;
        const int cellSize = 32;
        std::map<int, std::set<V>> grid;

        raylib::Vector2 getGridPosition(raylib::Vector2 pos) {
            float xI = (pos.x - bounds.x) / bounds.width;
            float yI = (pos.y - bounds.y) / bounds.height;
            raylib::Vector2 p = { xI * chunkedSize.x, yI * chunkedSize.y };

            return p;
        }

        int getHashPos(raylib::Vector2 pos) {
            return pos.y * chunkedSize.y + pos.x;
        }
    };
}