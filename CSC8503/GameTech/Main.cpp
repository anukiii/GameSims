#include "../../Common/Window.h"

#include "../CSC8503Common/StateMachine.h"
#include "../CSC8503Common/StateTransition.h"
#include "../CSC8503Common/State.h"

#include "../CSC8503Common/NavigationGrid.h"

#include "TutorialGame.h"
#include "../CSC8503Common/Intro.h"
#include "../CSC8503Common/GameScreen.h"
#include "../CSC8503Common/PushdownMachine.h"
#include "../CSC8503Common/PushdownState.h"
#include "../CSC8503Common/BehaviourAction.h"
#include "../CSC8503Common/BehaviourSequence.h"
#include "../CSC8503Common/BehaviourSelector.h"

using namespace NCL;
using namespace CSC8503;






//example of behaviour tree, not implemented but functioning in console
void  TestBehaviourTree() {
	float  behaviourTimer; 
	float  distanceToTarget;


	BehaviourAction * findKey = new  BehaviourAction("Find  Key",
		[&](float dt, BehaviourState  state)->BehaviourState {
			if (state == Initialise) {
				std::cout << "Looking  for a key!\n"; 
				behaviourTimer = rand() % 100;
				state = Ongoing; 
			}
			else if (state == Ongoing) {
				behaviourTimer -= dt;
				if (behaviourTimer <= 0.0f) {
					std::cout << "Found a key!\n";
					return  Success;
				}
			} 


			return  state; //will be �ongoing � until  success
		}
	);

	BehaviourAction* goToRoom = new  BehaviourAction("Go To Room",
		[&](float dt, BehaviourState  state)->BehaviourState {
			if (state == Initialise) {
				std::cout << "Going to the  loot  room!\n";
				state = Ongoing;
			}
			else if (state == Ongoing) {
				distanceToTarget -= dt;
				if (distanceToTarget <= 0.0f) {
					std::cout << "Reached  room!\n";
					return  Success; 
				}
			}
			return  state; //will be �ongoing � until  success    
		}   
	);

	BehaviourAction* openDoor = new  BehaviourAction("Open  Door",    
		[&](float dt, BehaviourState  state)->BehaviourState {
			if (state == Initialise) {
				std::cout << "Opening  Door!\n";
				return  Success;
			}
			return  state;      
		}
	);

	BehaviourAction* lookForTreasure = new  BehaviourAction(
		"Look  For  Treasure",
		[&](float dt, BehaviourState  state)->BehaviourState {
			if (state == Initialise) {
				std::cout << "Looking  for  treasure !\n";
				return  Ongoing; 
			}  
			else if (state == Ongoing) {
				bool  found = rand() % 2; 
				if (found) { 
					std::cout << "I found  some  treasure !\n";
					return  Success;
				}
				std::cout << "No  treasure  in here ...\n";
				return  Failure;
			}
			return  state;
		}
	);

	BehaviourAction* lookForItems = new  BehaviourAction(
		"Look  For  Items",
		[&](float dt, BehaviourState  state)->BehaviourState {
			if (state == Initialise) {
				std::cout << "Looking  for  items!\n";
				return  Ongoing;
			}
			else if (state == Ongoing) {
				bool  found = rand() % 2;
				if (found) {
					std::cout << "I found  some  items!\n";
					return  Success;
				}
				std::cout << "No  items in here ...\n";
				return  Failure;
			}
			return  state;
		}
	);

	BehaviourSequence* sequence = new  BehaviourSequence("Room  Sequence");
	sequence->AddChild(findKey);
	sequence->AddChild(goToRoom);
	sequence->AddChild(openDoor);
	
	BehaviourSelector * selection =
		new  BehaviourSelector("Loot  Selection");
	selection->AddChild(lookForTreasure);
	selection->AddChild(lookForItems);
	BehaviourSequence * rootSequence =
		new  BehaviourSequence("Root  Sequence");
	rootSequence->AddChild(sequence);
	rootSequence->AddChild(selection);

	for (int i = 0; i < 5; ++i) {
		rootSequence->Reset();
		behaviourTimer = 0.0f;
		distanceToTarget = rand() % 250;
		BehaviourState  state = Ongoing;
		std::cout << "We�re  going on an  adventure !\n";
		while (state == Ongoing) {
			state = rootSequence->Execute(1.0f); //fake dt
		}
		if (state ==  Success) {
			std::cout  << "What a successful  adventure !\n";
		}
		else if (state ==  Failure){
			std::cout  << "What a waste of time!\n";
		}
	}
	std::cout  << "All  done!\n";
 }




int main() {
	TestBehaviourTree();
	


	Window*w = Window::CreateGameWindow("CSC8503 Game technology!", 1920, 1080);
	if (!w->HasInitialised()) {
		return -1;
	}
	srand(time(0));
	w->ShowOSPointer(false);
	w->LockMouseToWindow(true);

	TutorialGame* g = new TutorialGame();
	w->GetTimer()->GetTimeDeltaSeconds(); //Clear the timer so we don't get a larget first dt!

	PushdownMachine  machine(new  IntroScreen(),g);

	while (w->UpdateWindow() && !Window::GetKeyboard()->KeyDown(KeyboardKeys::ESCAPE)) {
		

		float dt = w->GetTimer()->GetTimeDeltaSeconds();

		if (!machine.Update(dt)) {
			if (dt > 0.1f) {
				std::cout << "Skipping large time delta" << std::endl;
				continue; //must have hit a breakpoint or something to have a 1 second frame time!
			}
			if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::PRIOR)) {
				w->ShowConsole(true);
			}
			if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::NEXT)) {
				w->ShowConsole(false);
			}

			if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::T)) {
				w->SetWindowPosition(0, 0);
			}

			w->SetTitle("Gametech frame time:" + std::to_string(1000.0f * dt));


			g->UpdateGame(dt);
		}

	}
	Window::DestroyGameWindow();
}