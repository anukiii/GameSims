#pragma once
#include "PushdownState.h"
#include "../gameTech/TutorialGame.h"
#include "../../Common/Window.h"
#include "PauseScreen.h"
#include "GameScreen.h"

using namespace NCL;
using namespace CSC8503;

class  IntroScreen : public  PushdownState {
	    PushdownResult  OnUpdate(float dt,  PushdownState * *newState ,TutorialGame* g) override {

			g->UpdateGame(dt);

		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::NUM1)) { 
			* newState = new  GameScreen(); 
			g->setGameType(1);

			return  PushdownResult::Push;  
		}
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::NUM2)) {
			*newState = new  GameScreen();
			g->setGameType(2);

			return  PushdownResult::Push;
		}
			if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::ESCAPE)) {  
				return  PushdownResult::Pop; 
			} 
				return  PushdownResult::NoChange;  

	};
		void  OnAwake(TutorialGame* g, float dt)  override {
			g->setScene(0);
		}
};