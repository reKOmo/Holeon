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
	private:
		std::vector<int> m_Map;
		int m_Width = 0;
		int m_Height = 0;
		int m_Size = 0;
	};
}