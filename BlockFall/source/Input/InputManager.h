#pragma once
#include <SDL3/SDL.h>
#include <unordered_map>

enum class KeyState
{
    None,       // not pressed
    Pressed,    // pressed this frame
    Held,       // held down
    Released    // released this frame
};

struct KeyRepeatData
{
    float dasTimer = 0.0f;   // time since key held
    float arrTimer = 0.0f;   // time since last repeat
    bool dasTriggered = false;
};

class InputManager
{
public:
    InputManager() = default;
    ~InputManager() = default;

    void processInput(const SDL_Event& event);

    // Keyboard queries
    bool isKeyPressed(SDL_Scancode key) const;
    bool isKeyHeld(SDL_Scancode key) const;
    bool isKeyReleased(SDL_Scancode key) const;

    // Mouse queries
    bool isMouseButtonPressed(Uint8 button) const;
    bool isMouseButtonHeld(Uint8 button) const;
    bool isMouseButtonReleased(Uint8 button) const;
    inline int getMouseX() const { return _mouseX; }
    inline int getMouseY() const { return _mouseY; }

    bool keyActionIfNotOnCooldown(SDL_Scancode key, float das, float arr , float deltaTime);

    // Exit handling
    bool shouldQuit() const;

private:
    std::unordered_map<SDL_Scancode, KeyState> _keyStates;
    std::unordered_map<Uint8, KeyState> _mouseStates;
    std::unordered_map<SDL_Scancode, KeyRepeatData> _keyCooldowns;
    int _mouseX = 0, _mouseY = 0;

    void updateKeyState(SDL_Scancode key, bool down);
    void updateMouseState(Uint8 button, bool down);
};