#include "Level.h"

#include "Events.h"
#include "Physics.h"
#include "BasicCollision.h"
#include "Graphics.h"

#include "GlobalUI.h"
#include "FighterShip.h"
#include "Bullet.h"

#include "Controller.h"
#include "KeyMap.h"

#include "NearestUntilDestroyedTS.h"
#include "NearestUntilDestroyedTD.h"
#include "BasicMS.h"
#include "BasicMD.h"
#include "SprayAS.h"
#include "SprayAD.h"

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
    objectFactory.registerFactory(FighterShipSpec::FIGHTER_SHIP, FighterShip::factory);

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

    spriteMap.insert({ "player", de->LoadPicture(L"assets\\basic.bmp") });
    spriteMap.insert({ "fighter", de->LoadPicture(L"assets\\harrasser.bmp") });
    spriteMap.insert({ "bullet", de->LoadPicture(L"assets\\bullet.bmp") });
    spriteMap.insert({ "plasma", de->LoadPicture(L"assets\\plasma.bmp") });
    spriteMap.insert({ "star", de->LoadPicture(L"assets\\star.bmp") });

    /* Game Setup
    -------------------- */

    // Global UI
    objectManager->createObject(GlobalUISpec::UPtr(new GlobalUISpec()));

    // Star field
    starField = std::static_pointer_cast<StarField>(
        objectManager->createObject(StarFieldSpec::create(
            Vector2D(0.0f, 0.0f),
            Vector2D(2000 * (de->GetScreenWidth() / (float) de->GetScreenHeight()), 2000),
            0.000007f, // 7 / 1,000,000
            spriteMap["star"]
        ))
    );
    starField->setNumLayers(5);

    // Player
    player = std::static_pointer_cast<PlayerShip>(
        objectManager->createObject(PlayerShipSpec::UPtr(new PlayerShipSpec(
            Vector2D(0.0f, 0.0f), // Centre of the world
            Vector2D(0.0f, 1.0f), // Facing up
            spriteMap["player"],
            1000.0f,              // Ship integrity
            spriteMap["bullet"],
            3.0f                  // Bullet damage
        )))
    );

    setCameraFocus(std::dynamic_pointer_cast<HasPhysOf<NewtonianPhysModel>>(player));
    de->UseCamera(true);

    // Player controller (a key map)
    KeyMap::UPtr playerKeymap = KeyMap::create(std::dynamic_pointer_cast<HasEventHandler>(player));
    playerKeymap->bind(new KeyboardControl(ControlType::HOLD, DIK_W), new BasicMovement::MainThrustEventEmitter());
    playerKeymap->bind(new KeyboardControl(ControlType::HOLD, DIK_A), new BasicMovement::TurnLeftThrustEventEmitter());
    playerKeymap->bind(new KeyboardControl(ControlType::HOLD, DIK_D), new BasicMovement::TurnRightThrustEventEmitter());
    playerKeymap->bind(new KeyboardControl(ControlType::HOLD, DIK_SPACE), new SprayAttack::FireEventEmitter());
    playerKeymap->bind(new KeyboardControl(ControlType::PRESS, DIK_P), new UpgradeEventEmitter(PlayerShipUpgrade::HEAVY_SHELLS));

    objectManager->createObject(ControllerSpec::create(move(playerKeymap)));

    // A controller for all fighters
    // The strategies used do not implement flocking behaviour, but if they did, then multple AIs (flocks) would be needed.
    fighterAI = BasicAI::create(
        NearestUntilDestroyedTS::create(),
        BasicMS::create(),
        SprayAS::create()
    );
    objectManager->createObject(ControllerSpec::create(fighterAI));

    spawnFighter();
    spawnFighter();
    spawnFighter();
}

// Level Management

void Level::spawnFighter() {
    // Lock the object manager pointer for the whole function
    ObjectManager::Ptr objectManager = this->objectManager.lock();

    // An enemy
    Vector2D offset;
    offset.setBearing(
        (rand() % 1000) * (2.0f * (float)M_PI / 1000.0f),
        1200.0f + rand() % 1000
    );

    FighterShip::Ptr enemy = std::static_pointer_cast<FighterShip>(
        objectManager->createObject(FighterShipSpec::UPtr(new FighterShipSpec(
            player->physModel().pos() + offset, // Random position just off-screen
            Vector2D(0.0f, 1.0f),               // Facing up
            spriteMap["fighter"],
            100.0f,                             // Ship integrity
            spriteMap["plasma"],
            5.0f                                // Plasma damage
        )))
    );

    // Add it to the AI for its type with relevant parameters
    fighterAI->add(
        enemy,
        NearestUntilDestroyedTD::create(),
        BasicMD::create(
            5.0f,                                    // maximum speed
            (1.0f / 24.0f) * 2.0f * (float)M_PI,     // max angle before accel = 1/24 2pi radians = 15 degrees
            400.0f,                                  // optimal distance
            0.03f + (rand() % 100) / 100.0f * 0.12f, // rotation velocity
            60.0f,                                   // offset amplitude,
            1.5f                                     // offset frequency (Hz)
        ),
        SprayAD::create(
            (1.0f / 72.0f) * 2.0f * (float)M_PI,     // max angle before fire = 1/72 2pi radians = 5 degrees
            false,                                   // can/cannot rotate its gun
            0.0f                                     // gun requested rotation speed
        )
    );
    fighterAI->targettingStrategy()->addTarget(player);
}

// Global state

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
        -cameraOffset * CAMERA_ELASTICITY                      // Camera drag        > 0 <
        -cameraFocusObject->physModel().accel() * CAMERA_SHIFT // Acceleration shift < 0 >
    );
    MyDrawEngine::GetInstance()->theCamera.PlaceAt(
        cameraFocusObject->physModel().pos() + cameraOffset
    );

    // Stars (track player)
    starField->physModel().setPos(player->physModel().pos());
}
