#include "SceneMap.hpp"

namespace LimeEngine
{
	SceneMap::SceneMap(Engine* engine) : engine(engine) {}

	void SceneMap::Load() {}

	void SceneMap::Update() {}

	void SceneMap::UpdateMap()
	{
		Update();
		for (auto&& object : objects)
		{
			object->UpdateObject();
		}
	}

	void SceneMap::AttachObject(std::unique_ptr<SceneObject>&& object)
	{
		objects.push_back(std::move(object));
	}
}