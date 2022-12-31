#pragma once
#include <vector>
#include <string>
#include <iostream>

namespace Engine {
	class TerrainMap {
	public:
		TerrainMap() {}
		TerrainMap(const TerrainMap&) = default;
		void loadMap(std::string src);
		void setMap(std::vector<int>& m, int width, int height) {
			m_Map = std::move(m);
			m_Height = height;
			m_Width = width;
			m_Size = width * height;
		}
		int operator () (int x, int y);
		int width() const {
			return m_Width;
		}
		int height() const {
			return m_Height;
		}
		int size() const {
			return m_Size;
		}
		friend std::ostream& operator<<(std::ostream& os, const TerrainMap& map);
		std::vector<int> walkableTiles;
	private:
		std::vector<int> m_Map;
		int m_Width = 0;
		int m_Height = 0;
		int m_Size = 0;
	};
}