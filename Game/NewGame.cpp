#include "NewGame.h"

#include "Level.h"

void NewGame::initialise(ObjectManager::Ptr objectManager) {
    objectManager->getObjectFactoryManager().registerFactory(LevelSpec::LEVEL_SPEC, Level::factory);
    Level::Ptr level = std::static_pointer_cast<Level>(
        objectManager->createObject(LevelSpec::create(objectManager))
    );
    objectManager->addLifecyclePoint(level);
}
