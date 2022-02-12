// gamecode.h
// Shell engine version 2020
// Chris Rook
// Last modified 25/09/2018
//   Update to prevent game loop from flipping twice

#pragma once

#include <windows.h>

#include "ErrorType.h"
#include "MyDrawEngine.h"
#include "GameTimer.h"

#include "Ship.h"
#include "KeyMap.h"

// This is a hack for reading keyboard in situations where you don't want to
// use a full input system.
// Note that this makes the class windows-specific, but it already includes
// windows.h.
// Still - nasty.
#define KEYPRESSED(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

// The game class. (Singleton)
// Manages a game loop, as well as menu and paused states.
// The class is not very cohesive - it really needs to be refactored into
// a few separate classes, but this keeps it simple.
class Game {
private:
	// MENU     = start menu
	// PAUSED   = paused
	// RUNNING  = the main game loop
	// GAMEOVER = setting this state causes the program to close after tidying up
	enum GameState{MENU, PAUSED, RUNNING, GAMEOVER};
	GameState m_currentState;

	int m_menuOption; // Tracks the currently selected menu option, during main or pause menu
	GameTimer gt;     // Tracks the time between time marks (for getting FPS)

	Ship* player;
	KeyMap<Ship::Action>* playerKeymap;

	// Disable:
	Game();            // no-arg constructor
	Game(Game& other); // copy constructor
	~Game();           // destructor

	void ChangeState(GameState newState);  // Use to change the state of the game to one of the states above

public:
	static Game instance;          // Singleton instance

	// Starts the game engines - Draw Engine, Sound Engine, Input Engine - singletons
	// This is called soon after the program runs
	ErrorType Setup(bool bFullScreen, HWND hwnd, HINSTANCE hinstance);

	// Terminates the game engines - Draw Engine, Sound Engine, Input Engine
	// This is called just before the program exits
	void Shutdown();

	// This is repeated, called every frame.
	// It will run either Update( ), MainMenu() or PauseMenu() depending on the
	// game state
	ErrorType Main();

	// Called each frame when in the menu state. Manages the main menu,
	// which is currently a basic placeholder.
	ErrorType MainMenu();

    // Called each frame when in the pause state. Manages the pause menu
    // which is currently a basic placeholder
	ErrorType PauseMenu();

	// Called at the start of the game - when changing state from MENU to RUNNING
	// Use this to initialise the core game
	ErrorType StartOfGame();

	// Called each frame when in the RUNNING state.
	// Checks for user pressing escape (which puts the game in the PAUSED state)
	// Flips and clears the back buffer
	// Gameplay programmer will develop this to create an actual game
	ErrorType Update();

	// Called when the player ends the game
	// Currently this is done from the PAUSED state, when returning to the main menu
	// but could be done by the gameplay programmer in other situations
	// This will be used by the gameplay programmer to clean up
	ErrorType EndOfGame();
};
