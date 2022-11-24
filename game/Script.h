#pragma once
#include "Entity.h"

namespace Engine {
	class Script {
	public:
		Entity m_Owner;

	protected:
		virtual void onCreate() {}
		virtual void onUpdate(float delta) {}
		virtual void onDestroy() {}
	private:
		friend class Scene;
	};
}