#pragma once
#include <stack>
#include "../GameTech/TutorialGame.h"

namespace NCL {
	namespace CSC8503 {
		class PushdownState;

		class PushdownMachine
		{
		public:
			PushdownMachine();
			PushdownMachine(PushdownState* initialState, TutorialGame* g) { this->initialState = initialState; this->g = g; }
			

			~PushdownMachine();

			bool  Update(float dt);

		protected:
			PushdownState * activeState;
			PushdownState * initialState;
			TutorialGame  * g;

			std::stack<PushdownState*> stateStack;
		};
	}
}

