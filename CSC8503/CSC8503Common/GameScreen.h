#pragma once
#include "PushdownState.h"
#include "../gameTech/TutorialGame.h"
#include "../../Common/Window.h"
#include "PauseScreen.h"
#include "GameOverScreen.h"
#include "Win.h"

using namespace NCL;
using namespace CSC8503;

	class  GameScreen : public  PushdownState { 
	PushdownResult  OnUpdate(float dt, 
	PushdownState * *newState, TutorialGame* g) override {

	

		g->UpdateGame(dt);
	
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::P)) {
		* newState = new  PauseScreen();
		return   PushdownResult::Push;  
	}
	
	if (g->getScore() <= 0 || g->isWin() ==-1) {
		*newState = new  GameOverScreen();
		return   PushdownResult::Push;
	}
	if (g->isWin() ==1 ) {
		*newState = new  Win();
		return   PushdownResult::Push;

	}



	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::F1)) { 
		std::cout << "Returning  to main  menu!\n"; 
		return  PushdownResult::Pop; 
	}

	return  PushdownResult::NoChange;
};

void  OnAwake(TutorialGame* g, float dt)  override {
	g->setScene(1);
	g->InitialiseAssets();

}

};