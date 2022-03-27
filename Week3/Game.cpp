// GameCode.cpp        

#include "Game.h"

#include <time.h>

#include "ptrcast.h"

#include "ErrorLogger.h"
#include "MyDrawEngine.h"
#include "MySoundEngine.h"
#include "MyInputs.h"
#include "GameTimer.h"
#include "shapes.h"

#include "Ship.h"
#include "Bullet.h"
#include "GlobalUI.h"

#include "Controller.h"
#include "KeyMap.h"

Game::Game() :
    m_currentState(GameState::MENU),
    m_menuOption(0) {
}
Game::~Game() {}

Game Game::instance; // Singleton instance

GameTimer Game::gt; // Public static instance

/*
==================================================
Process Lifecycle
==================================================
*/

/*
 * Starts the game engines - Draw Engine, Sound Engine, Input Engine - singletons
 * This is called soon after the program runs.
 */
ErrorType Game::Setup(bool bFullScreen, HWND hwnd, HINSTANCE hinstance) {
    // Create the engines - this should be done before creating other DDraw objects
    if (FAILED(MyDrawEngine::Start(hwnd, bFullScreen))) {
        ErrorLogger::Writeln(L"Failed to start MyDrawEngine");
        return FAILURE;
    }

    if (FAILED(MySoundEngine::Start(hwnd))) {
        ErrorLogger::Writeln(L"Failed to start MySoundEngine");
        return FAILURE;
    }

    if (FAILED(MyInputs::Start(hinstance, hwnd))) {
        ErrorLogger::Writeln(L"Failed to start MyInputs");
        return FAILURE;
    }

    return SUCCESS;
}

/*
 * Terminates the game engines - Draw Engine, Sound Engine, Input Engine.
 * This is called just before the program exits.
 */
void Game::Shutdown() {
    // Terminate Engines
    MyDrawEngine::Terminate();
    MySoundEngine::Terminate();
    MyInputs::Terminate();
}

/*
 * This is repeated, called every frame.
 * It will run either Update(), MainMenu() or PauseMenu() depending on the
 * game state.
 */
ErrorType Game::Main() {
    // Flip and clear the back buffer
    MyDrawEngine* pTheDrawEngine = MyDrawEngine::GetInstance();
    pTheDrawEngine->Flip();
    pTheDrawEngine->ClearBackBuffer();

    // Run the relevant screen updater
    ErrorType err = SUCCESS;
    switch (m_currentState) {
        case MENU:     err = MainMenu(); break;
        case PAUSED:   err = PauseMenu(); break;
        case RUNNING:  err = Update(); break;
        case GAMEOVER: err = FAILURE; break;     // Small hack to end the game
        default:       err = FAILURE;            // Invalid state
    }
    return err;
}

/*
 * Use to change the state of the game. Private function.
 */
void Game::ChangeState(GameState newState) {
    // Very crude state system
    // Close old state
    // TODO: Reformat when something actually needs to go in here
    switch (m_currentState) {
        case MENU: break; // Not needed
        case PAUSED: break; // Not needed
        case RUNNING: break; // Not needed
    }

    // Change the state
    m_currentState = newState;
    m_menuOption = 0;

    // Transition to new state
    // TODO: Reformat when something actually needs to go in here
    switch (m_currentState) {
        case MENU: break; // Not needed
        case PAUSED: break; // Not needed
        case RUNNING: break; // Not needed
    }
}

/*
==================================================
Game Menus
==================================================
*/

/*
 * Called each frame when in the menu state. Manages the menu,
 * which is currently a basic placeholder.
 */
ErrorType Game::MainMenu() {
    MyDrawEngine::GetInstance()->WriteText(450, 220, L"Main menu", MyDrawEngine::WHITE);

    const int NUMOPTIONS = 2;
    const int MAXOPTIONLEN = 15;
    wchar_t options[NUMOPTIONS][MAXOPTIONLEN] = { L"Start game", L"Exit" };

    // Display the options
    for (int i = 0; i < NUMOPTIONS; i++) {
        int colour = MyDrawEngine::GREY;
        if (i == m_menuOption) {
            colour = MyDrawEngine::WHITE;
        }
        MyDrawEngine::GetInstance()->WriteText(450, 300 + 50 * i, options[i], colour);
    }

    // Get keyboard input
    MyInputs* pInputs = MyInputs::GetInstance();

    // Get user input
    pInputs->SampleKeyboard();

    if (pInputs->KeyPressed(DIK_UP)) {
        m_menuOption--;
    }

    if (pInputs->KeyPressed(DIK_DOWN)) {
        m_menuOption++;
    }

    if (m_menuOption < 0) {
        m_menuOption = 0;
    }
    else if (m_menuOption >= NUMOPTIONS) {
        m_menuOption = NUMOPTIONS - 1;
    }

    // User selects an option
    if (pInputs->KeyPressed(DIK_RETURN)) {
        // Play
        if (m_menuOption == 0) {
            StartOfGame();        // Initialise the game
            ChangeState(RUNNING); // Run it
        }

        // Quit
        if (m_menuOption == 1) {
            ChangeState(GAMEOVER);
        }
    }

    return SUCCESS;
}

/*
 * Called each frame when in the pause state. Manages the pause menu
 * which is currently a basic placeholder
 */
ErrorType Game::PauseMenu() {
    MyDrawEngine::GetInstance()->WriteText(450,220, L"Paused", MyDrawEngine::WHITE);

    const int NUMOPTIONS = 2;
    const int MAXOPTIONLEN = 11;
    wchar_t options[NUMOPTIONS][MAXOPTIONLEN] = {L"Resume", L"Main menu"};

    // Display menu options
    for (int i = 0; i < NUMOPTIONS; i++) {
        int colour = MyDrawEngine::GREY;        // Unselected -> grey
        if (i == m_menuOption) {
            colour = MyDrawEngine::WHITE;       // Selected -> white
        }
        MyDrawEngine::GetInstance()->WriteText(450,300+50*i, options[i], colour);
    }

    MyInputs* pInputs = MyInputs::GetInstance();

    // Get user input
    pInputs->SampleKeyboard();

    // Move choice up and down
    if (pInputs->KeyPressed(DIK_UP)) {
        m_menuOption--;
    }

    if (pInputs->KeyPressed(DIK_DOWN)) {
        m_menuOption++;
    }

    if (m_menuOption < 0) {
        m_menuOption = 0;
    } else if (m_menuOption >= NUMOPTIONS) {
        m_menuOption = NUMOPTIONS - 1;
    }

    // If player chooses an option
    if (pInputs->KeyPressed(DIK_RETURN)) {
        if (m_menuOption == 0) {  // Resume
            ChangeState(RUNNING); // Go back to running the game
        }
        
        if (m_menuOption == 1) {  // Quit
            EndOfGame();          // Clear up the game
            ChangeState(MENU);    // Go back to the menu
        }
    }

    return SUCCESS;
}

/*
==================================================
Game Lifecycle
==================================================
*/

/*
 * Called at the start of the game - when changing state from MENU to RUNNING.
 * Use this to initialise the core game.
 */
ErrorType Game::StartOfGame() {
    // Tick the timer (twice consecutively to initialise to 0 difference)
    gt.mark();
    gt.mark();

    // Game setup

    // Load Resources
    PictureIndex playerSprite = MyDrawEngine::GetInstance()->LoadPicture(L"assets\\basic.bmp");
    PictureIndex bulletSprite = MyDrawEngine::GetInstance()->LoadPicture(L"assets\\bullet.bmp");

    // Objects
    objectManager = ObjectManager::create();

      // Register Factories
    ObjectFactoryManager& objectFactory = objectManager->getObjectFactoryManager();
      // Controllers
    objectFactory.registerFactory(ControllerSpec::CONTROLLER, Controller::factory);
      // Objects
    objectFactory.registerFactory(ShipSpec::SHIP, Ship::factory);
    objectFactory.registerFactory(BulletSpec::BULLET, Bullet::factory);
    objectFactory.registerFactory(GlobalUISpec::GLOBAL_UI, GlobalUI::factory);

      // Create player
    GameObject::Ptr player = objectManager->createObject(ShipSpec::UPtr(new ShipSpec(
        Vector2D(0.0f, 0.0f), // Centre of the world
        Vector2D(0.0f, 1.0f), // Facing up
        playerSprite,
        bulletSprite
    )));

      // Player Keymap
    KeyMap::UPtr playerKeymap = KeyMap::create(std::dynamic_pointer_cast<HasEventHandler>(player));
    playerKeymap->bind(new KeyboardControl(ControlType::HOLD, DIK_W), new ShipEventHandler::MainThrustEventEmitter());
    playerKeymap->bind(new KeyboardControl(ControlType::HOLD, DIK_A), new ShipEventHandler::TurnLeftThrustEventEmitter());
    playerKeymap->bind(new KeyboardControl(ControlType::HOLD, DIK_D), new ShipEventHandler::TurnRightThrustEventEmitter());
    playerKeymap->bind(new KeyboardControl(ControlType::HOLD, DIK_SPACE), new ShipEventHandler::FireEventEmitter());
    playerKeymap->bind(new KeyboardControl(ControlType::PRESS, DIK_P), new UpgradeEventEmitter(ShipUpgrade::LOAD_OPTIMISATION));

    objectManager->createObject(ControllerSpec::create(move(playerKeymap)));

      // Global UI
    objectManager->createObject(GlobalUISpec::UPtr(new GlobalUISpec()));

    return SUCCESS;
}

/*
 * Called when the player ends the game, before returning to main menu.
 * Currently this is done from the PAUSED state, when returning to the main menu,
 * but could be done by the gameplay programmer in other situations.
 * This will be used by the gameplay programmer to clean up.
 */
ErrorType Game::EndOfGame() {
    return SUCCESS;
}

/*
 * Called each frame when in the RUNNING state.
 * Checks for user pressing escape (which puts the game in the PAUSED state).
 * Flips and clears the back buffer.
 * Gameplay programmer will develop this to create an actual game.
 */
ErrorType Game::Update() {
    // Check for entry to pause menu
    static bool escapePressed = true;
    if (KEYPRESSED(VK_ESCAPE)) {
        if (!escapePressed) {
            ChangeState(PAUSED);
        }
        escapePressed = true;
    } else {
        escapePressed = false;
    }

    // Tick the timer
    gt.mark();

    /* Game code
    -------------------------------------------------- */

    MyInputs* input = MyInputs::GetInstance();
    input->SampleKeyboard();

    objectManager->run();

    return SUCCESS;
}
