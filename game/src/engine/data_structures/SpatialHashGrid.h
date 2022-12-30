#pragma once
#include "raylib-cpp.hpp"
#include "Helpers.h"
#include <map>
#include <vector>
#include <set>

namespace Engine {
    template <typename V>
    class SpatialHashGrid {
    public:
        SpatialHashGrid() {}

        SpatialHashGrid(raylib::Rectangle bounds, int chunkSize = 32) 
            : bounds(bounds), cellSize(chunkSize)
        {
            chunkedSize.x = bounds.width / cellSize;
            chunkedSize.y = bounds.height / cellSize;
        }

        SpatialHashGrid(SpatialHashGrid&& other) noexcept
            : bounds(other.bounds), cellSize(other.chunkSize)
        {
            chunkedSize = other.chunkedSize;
            grid = std::move(other.grid);
        }

        SpatialHashGrid& operator=(SpatialHashGrid&& other) noexcept {
            if (this != &other) {
                bounds = other.bounds;
                cellSize = other.cellSize;
                chunkedSize = other.chunkedSize;
                grid = std::move(other.grid);
            }

            return *this;
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
        raylib::Rectangle bounds = {0.0, 0.0};
        raylib::Vector2 chunkedSize = { 0.0, 0.0 };
        int cellSize = 0;
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