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

// Multiplier for how much the camera focus object's acceleration shifts the camera away from it.
const float Level::CAMERA_SHIFT = 40.0f;

//      0 = completely elastic (camera doesn't 'pull' back to camera focus object)
// (0, 1) = somewhat elastic   (camera 'pulls' back to camera focus object at given rate)
//      1 = completely rigid   (camera 'pulls' back to camera focus object immediately)
const float Level::CAMERA_ELASTICITY = 0.05f;

Level::Level(LevelSpec::Ptr spec) : objectManager(spec->objectManager) {}

const ObjectFactory Level::factory = [](ObjectSpec::UPtr spec) {
    Level::Ptr level = Level::Ptr(new Level(static_unique_pointer_cast<LevelSpec>(move(spec))));
    level->setRef(level);
    return level;
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
    objectFactory.registerFactory(PlayerShipSpec::PLAYER_SHIP, PlayerShip::factory);

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

    // Also a GameObject and LifecyclePoint, but NewGame does the createObject() for us
    objectManager->addLifecyclePoint(ref().lock());

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
    player = std::static_pointer_cast<PlayerShip>(
        objectManager->createObject(PlayerShipSpec::UPtr(new PlayerShipSpec(
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
    playerKeymap->bind(new KeyboardControl(ControlType::HOLD, DIK_W), new PlayerShipEventHandler::MainThrustEventEmitter());
    playerKeymap->bind(new KeyboardControl(ControlType::HOLD, DIK_A), new PlayerShipEventHandler::TurnLeftThrustEventEmitter());
    playerKeymap->bind(new KeyboardControl(ControlType::HOLD, DIK_D), new PlayerShipEventHandler::TurnRightThrustEventEmitter());
    playerKeymap->bind(new KeyboardControl(ControlType::HOLD, DIK_SPACE), new PlayerShipEventHandler::FireEventEmitter());
    playerKeymap->bind(new KeyboardControl(ControlType::PRESS, DIK_P), new UpgradeEventEmitter(PlayerShipUpgrade::LOAD_OPTIMISATION));

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
    cameraOffset += (
        -cameraOffset * CAMERA_ELASTICITY                       // Camera drag        > 0 <
        - cameraFocusObject->physModel().accel() * CAMERA_SHIFT // Acceleration shift < 0 >
    );
    MyDrawEngine::GetInstance()->theCamera.PlaceAt(
        cameraFocusObject->physModel().pos() + cameraOffset
    );

    // Stars (track player)
    starField->physModel().setPos(player->physModel().pos());
}
