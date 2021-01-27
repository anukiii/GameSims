#pragma once
#include "PushdownState.h"
#include "../gameTech/TutorialGame.h"

using namespace NCL;
using namespace CSC8503;

class  PauseScreen : public  PushdownState {
    PushdownResult  OnUpdate(float dt,PushdownState * *newState ) override {
        if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::U)) { 
            return  PushdownResult::Pop; 
        }
        return  PushdownResult::NoChange; 
    }    

    void  OnAwake()  override {
        std::cout << "Press U to  unpause  game!\n"; 
    }
};