#include "Level.h"

#include "Controller.h"
#include "BasicCollision.h"
#include "Events.h"
#include "KeyMap.h"

#include "Ship.h"
#include "Bullet.h"
#include "GlobalUI.h"

Level::Level(LevelSpec::Ptr spec) : objectManager(spec->objectManager) {}

const ObjectFactory Level::factory = [](ObjectSpec::UPtr spec) {
    return GameObject::Ptr(new Level(static_unique_pointer_cast<LevelSpec>(move(spec))));
};

void Level::afterCreate() {
    // Lock the object manager for the whole function
    ObjectManager::Ptr objectManager = this->objectManager.lock();

    /* Load Resources
    -------------------- */

    PictureIndex playerSprite = MyDrawEngine::GetInstance()->LoadPicture(L"assets\\basic.bmp");
    PictureIndex bulletSprite = MyDrawEngine::GetInstance()->LoadPicture(L"assets\\bullet.bmp");

    /* Register Factories
    -------------------- */

    ObjectFactoryManager& objectFactory = objectManager->getObjectFactoryManager();
    // Lifecycle Points & Controllers
    objectFactory.registerFactory(BasicCollisionSpec::BASIC_COLLISION_SPEC, BasicCollision::factory);
    objectFactory.registerFactory(ControllerSpec::CONTROLLER, Controller::factory);

    // Objects
    objectFactory.registerFactory(ShipSpec::SHIP, Ship::factory);
    objectFactory.registerFactory(BulletSpec::BULLET, Bullet::factory);
    objectFactory.registerFactory(GlobalUISpec::GLOBAL_UI, GlobalUI::factory);

    /* Define Lifecycle Points
    -------------------- */

    objectManager->addLifecyclePoint(Events::create());

    // Create collision system (GameObject and LifecyclePoint)
    BasicCollision::Ptr basicCollision = std::static_pointer_cast<BasicCollision>(
        objectManager->createObject(BasicCollisionSpec::create())
    );
    objectManager->addLifecyclePoint(basicCollision);

    /* Game Setup
    -------------------- */

    // Global UI
    objectManager->createObject(GlobalUISpec::UPtr(new GlobalUISpec()));

    // Create player
    GameObject::Ptr player = objectManager->createObject(ShipSpec::UPtr(new ShipSpec(
        Vector2D(0.0f, 0.0f), // Centre of the world
        Vector2D(0.0f, 1.0f), // Facing up
        playerSprite,
        bulletSprite
    )));

    // Create player controller (a key map)
    KeyMap::UPtr playerKeymap = KeyMap::create(std::dynamic_pointer_cast<HasEventHandler>(player));
    playerKeymap->bind(new KeyboardControl(ControlType::HOLD, DIK_W), new ShipEventHandler::MainThrustEventEmitter());
    playerKeymap->bind(new KeyboardControl(ControlType::HOLD, DIK_A), new ShipEventHandler::TurnLeftThrustEventEmitter());
    playerKeymap->bind(new KeyboardControl(ControlType::HOLD, DIK_D), new ShipEventHandler::TurnRightThrustEventEmitter());
    playerKeymap->bind(new KeyboardControl(ControlType::HOLD, DIK_SPACE), new ShipEventHandler::FireEventEmitter());
    playerKeymap->bind(new KeyboardControl(ControlType::PRESS, DIK_P), new UpgradeEventEmitter(ShipUpgrade::LOAD_OPTIMISATION));

    objectManager->createObject(ControllerSpec::create(move(playerKeymap)));
}

// Implement LifecyclePoint

void Level::objectCreated(GameObject::Ptr object) {
    if (auto levelActor = std::dynamic_pointer_cast<LevelActor>(object)) {
        levelActor->setLevel(ref().lock());
    }
}
