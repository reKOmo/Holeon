#pragma once
#include <map>

namespace Engine {
	template <typename K>
	class Collection {
	public:
		~Collection() {
			for (auto const& ob : objects) {
				delete ob.second;
			}
		}


		template<typename T, typename... Args>
		T* emplace(K key, Args&&... args) {
			if (objects.count(key)) {
				delete objects[key];
			}
			T* n = new T(std::forward<Args>(args)...);
			objects[key] = static_cast<void*>(n);

			return n;
		}
		template <typename T>
		T* get(K key) {
			if (objects.count(key))
				return static_cast<T*>(objects[key]);
			else
				return nullptr;
		}

		void remove(K key) {
			if (objects.count(key) == 0)
				return;

			void* v = objects[key];
			objects.erase(key);
			delete v;
		}
		bool has(K key) {
			return objects.count(key) != 0;
		}
	private:
		std::map<K, void*> objects;
	};
}