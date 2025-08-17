#pragma once
#include <string>

class Scene
{
public:
    virtual ~Scene() = default;

    virtual void init() = 0;

    virtual void handleInput(float dt) = 0;

    virtual void update(float deltaTime) = 0;

    virtual void render() = 0;

    virtual void cleanup() = 0;

    // Optional: for debugging
    virtual std::string getName() const = 0;
};