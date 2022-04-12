#include "Graphics.h"

#include "mydrawengine.h"

Graphics::Ptr Graphics::create() {
    return Ptr(new Graphics());
}

// Track graphics and UI objects

void Graphics::objectCreated(GameObject::Ptr object) {
    if (auto graphicsObject = std::dynamic_pointer_cast<HasGraphics>(object)) {
        graphicsObjects.push_back(graphicsObject);
    }
    if (auto uiObject = std::dynamic_pointer_cast<HasUI>(object)) {
        uiObjects.push_back(uiObject);
    }
}
void Graphics::objectDestroyed(GameObject::Ptr object) {
    if (auto graphicsObject = std::dynamic_pointer_cast<HasGraphics>(object)) {
        graphicsObjects.remove(graphicsObject);
    }
    if (auto uiObject = std::dynamic_pointer_cast<HasUI>(object)) {
        uiObjects.remove(uiObject);
    }
}

// Run the lifecycle point

void Graphics::run() {
    MyDrawEngine* draw = MyDrawEngine::GetInstance();

    // Don't keep trying to draw if drawing goes wrong on this frame
    if (draw->BeginDraw() == SUCCESS) {
        for (HasGraphics::Ptr& object : graphicsObjects) object->beforeDraw();
        for (HasGraphics::Ptr& object : graphicsObjects) object->draw();
        if (draw->EndDraw() == SUCCESS) {

            if (draw->BeginDraw() == SUCCESS) {
                for (HasUI::Ptr& object : uiObjects) object->beforeDrawUI();
                for (HasUI::Ptr& object : uiObjects) object->drawUI();
                draw->EndDraw();
            }
        }
    }
    // ... but do keep going through the lifecycle methods
}
