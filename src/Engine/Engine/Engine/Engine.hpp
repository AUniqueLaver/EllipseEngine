#pragma once

#include "Core/Base.hpp"
#include "System.hpp"
#include "Module.hpp"

class Engine
{
   public:
    Engine() = default;
    virtual ~Engine() = default;

    virtual void addSystem(SharedPtr<ISystem> system) = 0;
    virtual ISystem& getSystem(const char* name) = 0;

    virtual void addModule(IModule& module) = 0;
    virtual void getModule(const char* name) = 0;

    static UniquePtr<Engine> createEngine();

   private:
};
