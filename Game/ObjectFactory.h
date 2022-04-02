#pragma once

#include <memory>
#include <functional>

class GameObject;
struct ObjectSpec;

using GameObjectPtr = std::shared_ptr<GameObject>;
using ObjectSpecUPtr = std::unique_ptr<ObjectSpec>;

using ObjectFactory = std::function<GameObjectPtr(ObjectSpecUPtr)>;
