#include "GlobalUI.h"

#include "ptrcast.h"

#include "MyDrawEngine.h"
#include "Game.h"

/* Show FPS Graphics Model
-------------------------------------------------- */

void FrameRateUIModel::draw() {
    if (SHOWFPS) {
        if (fpsUpdateCountdown == 0) {
            fpsUpdateCountdown = fpsUpdateRate;
            fps = fpsAvg;
            fpsAvg = 0.0f;
        }
        fpsAvg += 1.0f / (float) Game::gt.mdFrameTime;
        fpsUpdateCountdown--;

        MyDrawEngine* draw = MyDrawEngine::GetInstance();
        const Rectangle2D& viewport = draw->GetViewport();
        draw->WriteDouble(viewport.GetTopRight() - Vector2D(40, 0), fps / fpsUpdateRate, MyDrawEngine::CYAN);
    }
}

/* Global UI Graphics Model
-------------------------------------------------- */

void GlobalUIModel::addWidget(GraphicsModel::Ptr widget) {
    widgets.push_back(widget);
}

void GlobalUIModel::draw() {
    for (const GraphicsModel::Ptr& widget : widgets) {
        widget->draw();
    }
}

/* Global UI
-------------------------------------------------- */

GlobalUIModel& GlobalUI::uiGraphicsModel() {
    return static_cast<GlobalUIModel&>(GameObject::uiGraphicsModel());
}

GlobalUI::GlobalUI(GlobalUISpec::UPtr spec)
    : GameObject(
        GraphicsModel::UPtr(new GlobalUIModel())
    ) {
    uiGraphicsModel().addWidget(FrameRateUIModel::Ptr(new FrameRateUIModel()));
}

const ObjectFactory GlobalUI::factory = [](ObjectSpec::UPtr spec) {
    return GameObject::Ptr(new GlobalUI(static_unique_pointer_cast<GlobalUISpec>(move(spec))));
};
