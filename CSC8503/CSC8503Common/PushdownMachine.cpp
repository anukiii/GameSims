#include "PushdownMachine.h"
#include "PushdownState.h"
#include "../GameTech/TutorialGame.h"

using namespace NCL::CSC8503;

PushdownMachine::PushdownMachine()
{
	activeState = nullptr;
	initialState = nullptr;
	g = nullptr;
}

PushdownMachine::~PushdownMachine()
{
}

bool  PushdownMachine::Update(float dt) {
	if (activeState) {
		PushdownState* newState = nullptr;
		PushdownState::PushdownResult result = activeState->OnUpdate(dt , &newState,g );
		switch (result) {
			case PushdownState::Pop: {
				activeState->OnSleep(g,dt);
				stateStack.pop();
				if (stateStack.empty()) {
					return false;
				}
				else {
					activeState = stateStack.top();
					activeState->OnAwake(g,dt);
				}
			}break;
			case PushdownState::Push: {
				activeState->OnSleep(g,dt);
				stateStack.push(newState);
				activeState = newState;
				newState->OnAwake(g,dt);
			}break;
		}
	}
	else {
		stateStack.push(initialState);
		activeState = initialState;
		activeState->OnAwake(g,dt);
	}
	return  true;
}