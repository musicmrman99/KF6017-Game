#include "NewGame.h"

#include "Level.h"

void NewGame::initialise(ObjectManager::Ptr objectManager) {
    objectManager->getObjectFactoryManager().registerFactory(LevelSpec::LEVEL_SPEC, Level::factory);
    objectManager->createObject(LevelSpec::create(objectManager));
}
