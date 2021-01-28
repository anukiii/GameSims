#pragma once
#include "State.h"
#include "../GameTech/TutorialGame.h"

namespace NCL {
	namespace CSC8503 {
		class PushdownState
		{
		public:
			enum PushdownResult {
				Push, Pop, NoChange
			};
			PushdownState()							{}
			virtual ~PushdownState()				{}
			virtual  PushdownResult  OnUpdate(float dt, PushdownState** pushFunc ,TutorialGame *g) = 0;

			virtual void OnAwake(TutorialGame *g, float dt) {} //By default do nothing
			virtual void OnSleep(TutorialGame* g, float dt) {} //By default do nothing


		};
	}
}

