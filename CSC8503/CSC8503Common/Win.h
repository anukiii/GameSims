#pragma once
#include "PushdownState.h"
#include "../gameTech/TutorialGame.h"
#include "../../Common/Window.h"


using namespace NCL;
using namespace CSC8503;

class  Win : public  PushdownState {
	PushdownResult  OnUpdate(float dt, PushdownState** newState, TutorialGame* g) override {

		g->UpdateGame(dt);

		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::NUM1)) {
			return  PushdownResult::Pop;
		}
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::ESCAPE)) {
			return  PushdownResult::Pop;
		}
		return  PushdownResult::NoChange;

	};
	void  OnAwake(TutorialGame* g, float dt)  override {
		g->setScene(3);
	}
};