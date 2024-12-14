/*
 * This file was originally written by Chris Rook, last modified by him 2019-09-06.
 * 
 * It has been further modified in the following ways, with permission:
 * - reformatted
 */

/* Includes
-------------------------------------------------- */

#include <stdlib.h>
#define _CRTDBG_MAP_ALLOC // Turn on debug mode for <crtdbg.h>
#include <crtdbg.h> // For memory leak detection

/* Graphics: Windows
-------------------- */

#include <ddraw.h>    // DirectX
#define WIN32_LEAN_AND_MEAN // Excludes various unnecessary things from <windows.h>
#include <windows.h>  // Windows headers all-in-one
#include <windowsx.h> // Windows macros

/* Engine
-------------------- */

#include "Arguments.h"
#include "ErrorType.h"
#include "ErrorLogger.h"
#include "Game.h"

/* Globals
-------------------------------------------------- */

// Constants

constexpr const wchar_t* WINDOW_CLASS_NAME = L"WINCLASS1";

// Variables

HINSTANCE g_hInstance = NULL;
HWND g_hWnd = NULL;

bool g_ApplicationRunning; // Whether the application is running or stopped
bool g_ApplicationActive;  // Whether the application is active (foreground) or inactive (background)

/* Function Declarations
-------------------------------------------------- */

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
ErrorType CreateDXWindow(const wchar_t* title, int width, int height);
void DestroyDXWindow();

/* Entry Point
-------------------------------------------------- */

int WINAPI WinMain(
    HINSTANCE hInstance,     // A number that represents the instance of this program
    HINSTANCE hPrevInstance, // Not used in W95+ (null)
    LPSTR lpCmdLine,         // Pointer to the full command line as a string
    int nCmdShow             // Integer that tells how the window is to appear, eg. active, maximized, etc.
) {
    /* Activate Memory Leak Detection
    -------------------- */ 

    // This will report in visual studio when program exits.
    // Requires debug configuration.
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    /* Parse Arguments
    -------------------- */

    Arguments args(lpCmdLine);

    /* Set Globals
    -------------------- */ 

    g_hInstance = hInstance;

    bool bFullScreen = true; // Default to fullscreen
#ifdef _DEBUG
    bFullScreen = false;     // Set windowed if debugging
#endif
    if (args.contains("--no-fulscreen")) {
        bFullScreen = false; // Set windowed if specified on the command line
    }

    /* Create Window
    -------------------- */

    if (CreateDXWindow(L"Shell Engine", 800, 600) != SUCCESS) {
        return 0; // Terminate the app immediately
    }
    g_ApplicationRunning = true;

    /* Initialise Game
    -------------------- */

    int gameStatus = Game::instance.Setup(bFullScreen, g_hWnd, g_hInstance); // Initialise the game
    if (gameStatus == FAILURE) g_ApplicationRunning = false; // If game failed to initialise
    g_ApplicationActive = true; // Set window to active

    /* Run Game Loop
    -------------------- */

    int returnValue = 0;
    MSG msg; // Holder for each received message
    memset(&msg, 0, sizeof(msg));

    while (g_ApplicationRunning) {
        // Pop each window message and dispatch it
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg); // Doesn't do what you might expect
            DispatchMessage(&msg);
            if (msg.message == WM_QUIT) {
                returnValue = (int) msg.wParam;
                g_ApplicationRunning = false;
            }
        }

        // If it's still running and has focus
        if (g_ApplicationRunning && g_ApplicationActive) {
            gameStatus = Game::instance.Main();
            if (gameStatus == FAILURE) {
                // Something considered failure - try to avoid this overlapping with common
                // Windows error codes.
                returnValue = 1;
                g_ApplicationRunning = false;
            }
        }
    }

    /* Shutdown Game
    -------------------- */

    Game::instance.Shutdown(); // Clear up the game
    return returnValue;        // Terminate the app
}

/* Window Lifecycle
-------------------------------------------------- */

/**
 * Handle all window event messages.
 *
 * Most messages are ignored for this app. If not ignored, delegate the
 * message to the default handler.
 *
 * Note:
 * - All windows will use this winproc.
 * - This is called once for each message on each frame.
 * - There is no default behaviour (by the default handler) for most messages.
 */
LRESULT CALLBACK WindowProc(
    HWND hwnd,     // The target window
    UINT msg,      // The message sent
    WPARAM wparam, // Parameters of the message
    LPARAM lparam
) {
    switch (msg) {
    /* Core Lifecycle
    ---------- */

    // App is Opening
    case WM_CREATE: {
        // No initialisation to do
        return 0; // Success

    // Focus was Acquired/Lost
    } case WM_ACTIVATEAPP: {
        g_ApplicationActive = (wparam == TRUE) ? true : false;
        [[fallthrough]];
    } case WM_SETCURSOR: {
        // No mouse cursor while mouse is over the app, even in windowed mode.
        // If you need a cursor, create one yourself as a GameObject.
        SetCursor(NULL);
        return 0; // Success

    // App is Closing
    } case WM_DESTROY: {
        PostQuitMessage(0); // Sends a quit message to the app's message queue
        return 0;  // Success

    /* Graphics & Input - handled elsewhere
    ---------- */

    } case WM_PAINT: {
        // Open/close the Windows API paint operation, but don't paint anything.
        // Drawing is done by the Graphics subsystem using D3D.
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps); // Validate the window
        EndPaint(hwnd, &ps);
        return 0; // Success

    } case WM_CHAR: {
        // Input is done by the input components in the Events subsystem using
        // DirectInput.
        return 0; // Success

    /* Other Messages - use default behaviour
    ---------- */

    } default: {
        break;
    }
    }

    // Delegated all messages not handled here to the default handler
    return (DefWindowProc(hwnd, msg, wparam, lparam));
}

/**
 * Create the window.
 */
ErrorType CreateDXWindow(const wchar_t* title, int width, int height) {
    // Configure window class
    WNDCLASS wc;
    ZeroMemory(&wc, sizeof(WNDCLASS));

    wc.hInstance     = g_hInstance;                        // Use the instance for this program
    wc.lpszClassName = WINDOW_CLASS_NAME;                  // Use the class name we defined earlier
    wc.lpfnWndProc   = (WNDPROC) WindowProc;               // WndProc handles messages
    wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; // Redraw on size, and own DC for window
    wc.cbClsExtra    = 0;                                  // No extra class data
    wc.cbWndExtra    = 0;                                  // No extra window data
    wc.hIcon         = LoadIcon(NULL, IDI_WINLOGO);        // Use the default icon
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);        // Use the arrow pointer
    wc.hbrBackground = NULL;                               // No background (transparent before opening)
    wc.lpszMenuName  = nullptr;                            // No menu

    // Register window class
    if (!RegisterClass(&wc)) {
        ErrorLogger::Writeln(L"Failed to register the window class.");
        return FAILURE;
    }

    // Set window size
    RECT WindowRect;
    WindowRect.left   = (long) 0;
    WindowRect.right  = (long) width;
    WindowRect.top    = (long) 0;
    WindowRect.bottom = (long) height;

    // Set window style
    DWORD dwStyle = WS_POPUP;          // Windows Style - remove all window borders
    DWORD dwExStyle = WS_EX_APPWINDOW; // Window Extended Style

    // Adjust window size based on style
    AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);

    // Create window
    g_hWnd = CreateWindowEx(
        dwExStyle,                          // Extended Window Style
        WINDOW_CLASS_NAME,                  // Class Name
        title,                              // Window Title
        dwStyle,                            // Window Style
        WindowRect.left, WindowRect.top,    // Window Position - must be (0, 0) in fullscreen
        WindowRect.right - WindowRect.left, // Window Width (Calculate)
        WindowRect.bottom - WindowRect.top, // Window Height (Calculate)
        NULL,                               // No Parent Window
        NULL,                               // No Menu
        g_hInstance,                        // Instance
        NULL                                // Dont pass anything to WM_CREATE
    );
    if (!g_hWnd) {
        DestroyDXWindow(); // Reset the display
        ErrorLogger::Writeln(L"Failed to create the window.");
        return FAILURE;
    }

    ShowWindow(g_hWnd, SW_SHOW);

    return SUCCESS;
}

/**
 * Destroy the window cleanly.
 */
void DestroyDXWindow() {
    // Switch back to the desktop
    ChangeDisplaySettings(NULL, 0);

    // Show mouse pointer
    ShowCursor(TRUE);

    // Are we able to destroy the window?
    if (g_hWnd && !DestroyWindow(g_hWnd)) {
        ErrorLogger::Writeln(L"Could Not Destroy Window.");
        g_hWnd = NULL; // Set m_hWnd To NULL
    }

    // Are we able to unregister the class?
    if (!UnregisterClass(WINDOW_CLASS_NAME, g_hInstance)) {
        ErrorLogger::Writeln(L"Could Not Unregister Class.");
        g_hInstance = NULL;
    }
}
