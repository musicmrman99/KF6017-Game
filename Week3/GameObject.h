#pragma once

#include <memory>

class GameObject {
public:
    using Ptr = std::shared_ptr<GameObject>;
    using UPtr = std::unique_ptr<GameObject>;
    using WPtr = std::weak_ptr<GameObject>;

public:
    virtual ~GameObject();

    // Lifecycle
    
    // Run once just after creation
    virtual void afterCreate();

    // Run once just before each frame
    virtual void beforeFrame();

    // Run once just after each frame
    virtual void afterFrame();

    // Run once just before destruction
    virtual void beforeDestroy();
};
