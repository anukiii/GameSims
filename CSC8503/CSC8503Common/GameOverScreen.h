#pragma once
#pragma once
#include "PushdownState.h"
#include "../gameTech/TutorialGame.h"


using namespace NCL;
using namespace CSC8503;

class  GameOverScreen : public  PushdownState {
    PushdownResult  OnUpdate(float dt, PushdownState** newState, TutorialGame* g) override {
        g->UpdateGame(dt);

        if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::NUM1)) {
            g->setGameType(1);
            return  PushdownResult::Pop;
        }       
        if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::NUM2)) {
            g->setGameType(2);
            return  PushdownResult::Pop;
        }
        if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::ESCAPE)) {
            return  PushdownResult::Pop;
        }
        return  PushdownResult::NoChange;
    }

    void  OnAwake(TutorialGame* g, float dt)  override {
        g->setScene(2);
       // std::cout << "Press U to  unpause  game!\n";
    }
};