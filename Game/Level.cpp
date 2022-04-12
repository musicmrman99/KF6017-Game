#include "Level.h"

#include "Events.h"
#include "Physics.h"
#include "BasicCollision.h"
#include "Graphics.h"

#include "Bullet.h"
#include "GlobalUI.h"

#include "Controller.h"
#include "KeyMap.h"
#include "Game.h"

Level::Level(LevelSpec::Ptr spec) : objectManager(spec->objectManager) {}

const ObjectFactory Level::factory = [](ObjectSpec::UPtr spec) {
    return GameObject::Ptr(new Level(static_unique_pointer_cast<LevelSpec>(move(spec))));
};

void Level::afterCreate() {
    MyDrawEngine* de = MyDrawEngine::GetInstance();

    // Lock the object manager pointer for the whole function
    ObjectManager::Ptr objectManager = this->objectManager.lock();

    /* Register Factories
    -------------------- */

    ObjectFactoryManager& objectFactory = objectManager->getObjectFactoryManager();
    // Lifecycle Points & General Purpose
    objectFactory.registerFactory(BasicCollisionSpec::BASIC_COLLISION_SPEC, BasicCollision::factory);
    objectFactory.registerFactory(ControllerSpec::CONTROLLER, Controller::factory);

    // UI & World
    objectFactory.registerFactory(GlobalUISpec::GLOBAL_UI, GlobalUI::factory);
    objectFactory.registerFactory(StarFieldSpec::STAR_FIELD, StarField::factory);

    // Actors
    objectFactory.registerFactory(ShipSpec::SHIP, Ship::factory);

    // Objects
    objectFactory.registerFactory(BulletSpec::BULLET, Bullet::factory);

    /* Define Lifecycle Points
    -------------------- */

    objectManager->addLifecyclePoint(Events::create());
    objectManager->addLifecyclePoint(Physics::create());

    // Create collision system (GameObject and LifecyclePoint)
    BasicCollision::Ptr basicCollision = std::static_pointer_cast<BasicCollision>(
        objectManager->createObject(BasicCollisionSpec::create())
    );
    objectManager->addLifecyclePoint(basicCollision);

    objectManager->addLifecyclePoint(Graphics::create());

    /* Load Resources
    -------------------- */

    PictureIndex playerSprite = de->LoadPicture(L"assets\\basic.bmp");
    PictureIndex bulletSprite = de->LoadPicture(L"assets\\bullet.bmp");
    PictureIndex starSprite = de->LoadPicture(L"assets\\star.bmp");

    /* Game Setup
    -------------------- */

    // Global UI
    objectManager->createObject(GlobalUISpec::UPtr(new GlobalUISpec()));

    // Player
    player = std::static_pointer_cast<Ship>(
        objectManager->createObject(ShipSpec::UPtr(new ShipSpec(
            Vector2D(0.0f, 0.0f), // Centre of the world
            Vector2D(0.0f, 1.0f), // Facing up
            playerSprite,
            bulletSprite
        )))
    );

    setCameraFocus(std::dynamic_pointer_cast<HasPhysOf<NewtonianPhysModel>>(player));
    de->UseCamera(true);

    // Player controller (a key map)
    KeyMap::UPtr playerKeymap = KeyMap::create(std::dynamic_pointer_cast<HasEventHandler>(player));
    playerKeymap->bind(new KeyboardControl(ControlType::HOLD, DIK_W), new ShipEventHandler::MainThrustEventEmitter());
    playerKeymap->bind(new KeyboardControl(ControlType::HOLD, DIK_A), new ShipEventHandler::TurnLeftThrustEventEmitter());
    playerKeymap->bind(new KeyboardControl(ControlType::HOLD, DIK_D), new ShipEventHandler::TurnRightThrustEventEmitter());
    playerKeymap->bind(new KeyboardControl(ControlType::HOLD, DIK_SPACE), new ShipEventHandler::FireEventEmitter());
    playerKeymap->bind(new KeyboardControl(ControlType::PRESS, DIK_P), new UpgradeEventEmitter(ShipUpgrade::LOAD_OPTIMISATION));

    objectManager->createObject(ControllerSpec::create(move(playerKeymap)));

    // Star field
    starField = std::static_pointer_cast<StarField>(
        objectManager->createObject(StarFieldSpec::create(
            Vector2D(0.0f, 0.0f),
            Vector2D(2000 * (de->GetScreenWidth() / (float) de->GetScreenHeight()), 2000),
            0.000007f, // 7 / 1,000,000
            starSprite
        ))
    );
    starField->setNumLayers(5);
}

// Modifying global state

void Level::setCameraFocus(HasPhysOf<NewtonianPhysModel>::Ptr physObject) {
    if (physObject) cameraFocusObject = physObject;
}

// Implement LifecyclePoint

void Level::objectCreated(GameObject::Ptr object) {
    if (auto levelActor = std::dynamic_pointer_cast<LevelActor>(object)) {
        levelActor->setLevel(ref().lock());
    }
}

void Level::run() {
    // Camera focus (track player)
    latencyQueue.push(cameraFocusObject->physModel().pos());
    MyDrawEngine::GetInstance()->theCamera.PlaceAt(latencyQueue.front());
    if (latencyQueue.size() > LATENCY) latencyQueue.pop();

    // Stars (track player)
    starField->physModel().setPos(player->physModel().pos());
}
