#include "TerrainMap.h"
#include "fstream"
#include "string"

void Engine::TerrainMap::loadMap(std::string src) {
	std::fstream file;
	
	file.open(src, std::ios::in);

	if (file.is_open()) {
		std::string line, digit;
		int i, lineSize;
		m_Map.clear();
		m_Width = 0;
		m_Height = 0;
		while (std::getline(file, line)) {
			m_Height++;
			for (i = 0, lineSize = line.size(); i < lineSize; i++) {
				if ((line[i] >= '0' && line[i] <= '9') || line[i] == '-') {
					digit += line[i];
				} else {
					m_Map.push_back(std::stoi(digit));
					digit.clear();
					m_Size++;
					if (m_Height == 1) {
						m_Width++;
					}
				}
			}
			m_Map.push_back(std::stoi(digit));
			digit.clear();
			m_Size++;
			if (m_Height == 1) {
				m_Width++;
			}
		}


	} else {
		throw std::invalid_argument("Invalid path");
	}

	file.close();

}

int Engine::TerrainMap::operator()(int x, int y)
{
	if (x < 0 || x >= m_Width || y < 0 || y >= m_Height)
		return -1;
	return m_Map[y * m_Width + x];
}

std::ostream& Engine::operator<<(std::ostream& os, const TerrainMap& map)
{
	os << "{ Size: " << map.m_Size << ", Width: " << map.m_Width << ", Height: " << map.m_Height << ", Elements: ";
	for (int i = 0; i < map.m_Size; i++) {
		os << map.m_Map[i] << ",";
	}
	os << "}";
	return os;
}
