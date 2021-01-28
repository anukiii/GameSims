#include "TutorialGame.h"
#include "../CSC8503Common/GameWorld.h"
#include "../../Plugins/OpenGLRendering/OGLMesh.h"
#include "../../Plugins/OpenGLRendering/OGLShader.h"
#include "../../Plugins/OpenGLRendering/OGLTexture.h"
#include "../../Common/TextureLoader.h"
#include "../CSC8503Common/PositionConstraint.h"
#include "../GameTech/StateGameObject.h"
#include "../CSC8503Common/NavigationGrid.h"



using namespace NCL;
using namespace CSC8503;

TutorialGame::TutorialGame()	{
	world		= new GameWorld();
	renderer	= new GameTechRenderer(*world);
	physics		= new PhysicsSystem(*world);

	forceMagnitude	= 10.0f;
	physics->UseGravity(true);
	inSelectionMode = false;
	Bonuses = 0;
	Debug::SetRenderer(renderer);
	scene = 0;

}

/*

Each of the little demo scenarios used in the game uses the same 2 meshes, 
and the same texture and shader. There's no need to ever load in anything else
for this module, even in the coursework, but you can add it if you like!

*/
void TutorialGame::InitialiseAssets() {
	auto loadFunc = [](const string& name, OGLMesh** into) {
		*into = new OGLMesh(name);
		(*into)->SetPrimitiveType(GeometryPrimitive::Triangles);
		(*into)->UploadToGPU();
	};

	loadFunc("cube.msh"		 , &cubeMesh);
	loadFunc("sphere.msh"	 , &sphereMesh);
	loadFunc("Male1.msh"	 , &charMeshA);
	loadFunc("courier.msh"	 , &charMeshB);
	loadFunc("security.msh"	 , &enemyMesh);
	loadFunc("coin.msh"		 , &bonusMesh);
	loadFunc("capsule.msh"	 , &capsuleMesh);
	loadFunc("Cylinder.msh"	, &gooseMesh);
	loadFunc("Male1.msh"	, &appleMesh);

	basicTex	= (OGLTexture*)TextureLoader::LoadAPITexture("checkerboard.png");
	redTex = (OGLTexture*)TextureLoader::LoadAPITexture("redTex.png");
	basicShader = new OGLShader("GameTechVert.glsl", "GameTechFrag.glsl");

	enemyPathfindingSetup();
	currentNode = 0;
	InitCamera();
	InitWorld();
	start = time(0);
	win = 0;
	enemyStuckCounter = 0;

}


TutorialGame::~TutorialGame()	{
	delete cubeMesh;
	delete sphereMesh;
	delete charMeshA;
	delete charMeshB;
	delete enemyMesh;
	delete bonusMesh;
	delete gooseMesh;
	delete appleMesh;

	delete redTex;
	delete basicTex;
	delete basicShader;

	delete physics;
	delete renderer;
	delete world;
}

void TutorialGame::setScene(int newScene) {
	scene = newScene;
}


void TutorialGame::mainMenu(float dt) {
	renderer->ClearFrame();
	Debug::Print("TRIP GIRLS", Vector2(40, 30));
	Debug::Print("Press 1 to start", Vector2(35, 40));
	Debug::Print("Press esc to exit", Vector2(35, 50));
	renderer->Update(dt);

	Debug::FlushRenderables(dt);
	renderer->Render();

}

void NCL::CSC8503::TutorialGame::enemyPathfinding(GameObject* enemy){
	


	if (abs(enemy->GetTransform().GetPosition().x -testNodes.at(currentNode).x) <=20 && abs(enemy->GetTransform().GetPosition().z - testNodes.at(currentNode).z) <= 20) {

		currentNode++;
	}

	enemy->GetPhysicsObject()->AddForce(( testNodes.at(currentNode)- enemy->GetTransform().GetPosition() )*3);

	for (int i = 1; i < testNodes.size(); ++i) {
			Vector3 a = testNodes[i - 1];
			Vector3 b = testNodes[i];


			Debug::DrawLine(a, b, Vector4(0, 1, 0, 1));
		}

}


void TutorialGame::mainGame(float dt) {


	for (int i = 0; i < world->getGameObjects().size(); i++) {
		if (world->getGameObjects().at(i)->getType() == "Enemy") {



			enemyPathfinding(world->getGameObjects().at(i));
			if (world->getGameObjects().at(i)->isColiding() != nullptr) {

				if (world->getGameObjects().at(i)->isColiding()->getType() != "Floor" && world->getGameObjects().at(i)->GetTransform().GetPosition().y < 5) {
					enemyStuckCounter++;
					world->getGameObjects().at(i)->GetPhysicsObject()->AddForce(Vector3(0, 2000, 0));//Avoiods obstacles by jumping over them
					if (enemyStuckCounter > 500 || world->getGameObjects().at(i)->isColiding()->GetTransform().GetPosition().y < -5) {							//enemy restart if falls off or is stuck
						world->getGameObjects().at(i)->GetTransform().SetPosition(Vector3(560, 0, 560));
					}
				}
				else {
					enemyStuckCounter = 0;
				}
			}
		}
		if (world->getGameObjects().at(i)->getType() == "Player" && world->getGameObjects().at(i)->GetTransform().GetPosition().y < -5) {
			win = -1;
		}
		if (world->getGameObjects().at(i)->isColiding() != nullptr) {
			if (world->getGameObjects().at(i)->isColiding()->getType() == "Floor") {
				world->getGameObjects().at(i)->GetPhysicsObject()->AplyFriction();							//FRICTION WITH FLOOR
			}
			else if (world->getGameObjects().at(i)->isColiding()->getType() == "Player" && world->getGameObjects().at(i)->getType() == "Bonus") {
				Bonuses++;
				world->getGameObjects().at(i)->setType("Collected");
				world->getGameObjects().at(i)->removeObject();
			}
			else if (world->getGameObjects().at(i)->isColiding()->getType() == "Enemy" && world->getGameObjects().at(i)->getType() == "Bonus") {
				world->getGameObjects().at(i)->setType("Collected");
				world->getGameObjects().at(i)->removeObject();
				Bonuses--;
			}



			else if (world->getGameObjects().at(i)->isColiding()->getType() == "Player" && world->getGameObjects().at(i)->getType() == "End") {
				win = 1;
			}
			else if (world->getGameObjects().at(i)->isColiding()->getType() == "Enemy" && world->getGameObjects().at(i)->getType() == "End") {
				win = -1;
			}

			}

		
	}



	//if (testStateObject) { 
		testStateObject->Update(dt); 
	//}

	if (!inSelectionMode) {
		world->GetMainCamera()->UpdateCamera(dt);
	}
	score = 1000 + Bonuses * 100 - difftime(time(0), start) * 10;

	Debug::Print(std::to_string(score), Vector2(80, 10));
	UpdateKeys();


	SelectObject();
	MoveSelectedObject();
	physics->Update(dt);

	if (lockedObject != nullptr) {
		Vector3 objPos = lockedObject->GetTransform().GetPosition();
		Vector3 camPos = objPos + lockedOffset;

		Matrix4 temp = Matrix4::BuildViewMatrix(camPos, objPos, Vector3(0, 1, 0));

		Matrix4 modelMat = temp.Inverse();

		Quaternion q(modelMat);
		Vector3 angles = q.ToEuler(); //nearly there now!

		world->GetMainCamera()->SetPosition(camPos);
		world->GetMainCamera()->SetPitch(angles.x);
		world->GetMainCamera()->SetYaw(angles.y);

		Debug::DrawAxisLines(lockedObject->GetTransform().GetMatrix(), 2.0f);
	}

	world->UpdateWorld(dt);
	renderer->Update(dt);

	Debug::FlushRenderables(dt);
	renderer->Render();

}

void TutorialGame::endScreen(float dt) {
	Debug::Print("GAME OVER", Vector2(40, 30));
	Debug::Print("Press 1 to start", Vector2(35, 40));
	Debug::Print("Press esc to exit", Vector2(35, 50));
	renderer->Update(dt);

	Debug::FlushRenderables(dt);
	renderer->Render();

}

void TutorialGame::winScreen(float dt) {
	Debug::Print("WINNER WINNER", Vector2(40, 30));
	Debug::Print("Your final score was " + score, Vector2(40, 30));

	Debug::Print("Press 1 to try again", Vector2(35, 50));
	Debug::Print("Press esc to exit", Vector2(35, 60));
	renderer->Update(dt);

	Debug::FlushRenderables(dt);
	renderer->Render();

}


void NCL::CSC8503::TutorialGame::enemyPathfindingSetup(){
	NavigationGrid  grid("TestGrid1.txt");

	NavigationPath  outPath;

	Vector3  startPos(550, 0, 550);

	Vector3  endPos(30, 0, 30);

	bool  found = grid.FindPath(startPos, endPos, outPath);

	Vector3  pos;
	while (outPath.PopWaypoint(pos)) {
		testNodes.push_back(pos);
	}
	testNodes.pop_back(); //gets rid of teh last one since it causes glitches
}



void TutorialGame::UpdateGame(float dt) {


	switch (scene) {
	case 0:
		mainMenu(dt); break;

	case 1:
		mainGame(dt);
		break;
	case 2:
		endScreen(dt);
		break;
	case 3:
		winScreen(dt);
		break;
	}



}



void TutorialGame::UpdateKeys() {


	if (lockedObject) {
		LockedObjectMovement();
	}

}

void TutorialGame::LockedObjectMovement() {
	Matrix4 view		= world->GetMainCamera()->BuildViewMatrix();
	Matrix4 camWorld	= view.Inverse();

	Vector3 rightAxis = Vector3(camWorld.GetColumn(0)); //view is inverse of model!

	//forward is more tricky -  camera forward is 'into' the screen...
	//so we can take a guess, and use the cross of straight up, and
	//the right axis, to hopefully get a vector that's good enough!

	Vector3 fwdAxis = Vector3::Cross(Vector3(0, 1, 0), rightAxis);
	fwdAxis.y = 0.0f;
	fwdAxis.Normalise();

	Vector3 charForward  = lockedObject->GetTransform().GetOrientation() * Vector3(0, 0, 1);
	Vector3 charForward2 = lockedObject->GetTransform().GetOrientation() * Vector3(0, 0, 1);

	float force = 20.0f;

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::LEFT)) {
		lockedObject->GetPhysicsObject()->AddForce(-rightAxis * force);

		

		if (lockedObject->GetTransform().GetOrientation().y <= 0.65) {
			lockedObject->GetPhysicsObject()->AddTorque(Vector3(0, 6, 0));
		}
		else { lockedObject->GetPhysicsObject()->AddTorque(Vector3(0, -10, 0)); }
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::RIGHT)) {
		lockedObject->GetPhysicsObject()->AddForce(rightAxis * force);
		
		if (lockedObject->GetTransform().GetOrientation().y>=-0.65) {
			lockedObject->GetPhysicsObject()->AddTorque(Vector3(0, -6, 0));
		}
		else { lockedObject->GetPhysicsObject()->AddTorque(Vector3(0, 10, 0));
		 }
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::UP)) {
		lockedObject->GetPhysicsObject()->AddForce(fwdAxis * force);
		if (lockedObject->GetTransform().GetOrientation().y >= 0) {
			lockedObject->GetPhysicsObject()->AddTorque(Vector3(0, -6, 0));
		}
		else {
			lockedObject->GetPhysicsObject()->AddTorque(Vector3(0, 10, 0));
		}
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::DOWN)) {
		lockedObject->GetPhysicsObject()->AddForce(-fwdAxis * force);
		if (lockedObject->GetTransform().GetOrientation().y <= 0 && lockedObject->GetTransform().GetOrientation().y >=-0.97) {
			lockedObject->GetPhysicsObject()->AddTorque(Vector3(0, -6, 0));
		}
		else if (lockedObject->GetTransform().GetOrientation().y <= 0 && lockedObject->GetTransform().GetOrientation().y >= -1) {
			lockedObject->GetPhysicsObject()->AddTorque(Vector3(0, 10, 0));

		}
		else if (lockedObject->GetTransform().GetOrientation().y > 0 &&lockedObject->GetTransform().GetOrientation().y <= 0.97){
			lockedObject->GetPhysicsObject()->AddTorque(Vector3(0, 6, 0));
		}
		else {
			lockedObject->GetPhysicsObject()->AddTorque(Vector3(0, -10, 0));
		}
	}


}


void TutorialGame::InitCamera() {
	world->GetMainCamera()->SetNearPlane(0.1f);
	world->GetMainCamera()->SetFarPlane(500.0f);
	world->GetMainCamera()->SetPitch(-15.0f);
	world->GetMainCamera()->SetYaw(315.0f);
	world->GetMainCamera()->SetPosition(Vector3(-60, 40, 60));
	lockedObject = nullptr;
}

void TutorialGame::InitWorld() {
	world->ClearAndErase();
	physics->Clear();
	AddBonusToWorld(Vector3(520, 0, 520));
	addEndToWorld(Vector3(30, 10, 30));
	InitSphereGridWorld(10, 9, 70, 70, 5);
	InitCubeGridWorld(5, 5, 100, 100, Vector3(10,10,10));
	AddPlayerToWorld(Vector3(550, 10, 550));
	AddEnemyToWorld(Vector3(560, 10, 560));
	InitMoveGridWorld(2, 2, 250, 250);

	InitDefaultFloor();

	testStateObject = AddStateObjectToWorld(Vector3(0, 10, 0));

}

void TutorialGame::BridgeConstraintTest() {
	
		Vector3  cubeSize = Vector3(8, 8, 8);

		float     invCubeMass = 5; //how  heavy  the  middle  pieces  are
		int        numLinks      = 10;   
		float     maxDistance   = 30; // constraint  distance    
		float     cubeDistance = 20; // distance  between  links 
		
		Vector3  startPos = Vector3 (50, 50,  50); 
			
		GameObject* start = AddCubeToWorld(startPos + Vector3(0, 0, 0), cubeSize , 0);  
		GameObject* end = AddCubeToWorld(startPos + Vector3 (( numLinks + 2) * cubeDistance , 0, 0), cubeSize , 0);
		GameObject* previous = start;

		for (int i = 0; i < numLinks; ++i) {   
			GameObject* block = AddCubeToWorld(startPos + Vector3 ((i + 1) * cubeDistance , 0, 0), cubeSize , invCubeMass );       
			PositionConstraint* constraint =new  PositionConstraint(previous , block , maxDistance );    
			world ->AddConstraint(constraint );
			previous = block;   
		}  
		PositionConstraint* constraint = new  PositionConstraint(previous , end , maxDistance );     
		world ->AddConstraint(constraint );

}

/*

A single function to add a large immoveable cube to the bottom of our world

*/
GameObject* TutorialGame::AddFloorToWorld(const Vector3& position) {
	GameObject* floor = new GameObject();

	Vector3 floorSize	= Vector3(300, 2, 300);
	AABBVolume* volume	= new AABBVolume(floorSize);
	floor->SetBoundingVolume((CollisionVolume*)volume);
	floor->GetTransform()
		.SetScale(floorSize * 2)
		.SetPosition(position);
	floor->setType("Floor");
	floor->SetRenderObject(new RenderObject(&floor->GetTransform(), cubeMesh, basicTex, basicShader));
	floor->SetPhysicsObject(new PhysicsObject(&floor->GetTransform(), floor->GetBoundingVolume()));

	floor->GetPhysicsObject()->SetInverseMass(0);
	floor->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(floor);

	return floor;
}


StateGameObject* NCL::CSC8503::TutorialGame::AddStateObjectToWorld(const Vector3& position)
{
	StateGameObject* apple = new StateGameObject();

	SphereVolume* volume = new SphereVolume(1.5f);
	apple->SetBoundingVolume((CollisionVolume*)volume);
	apple->setType("Moving");

	apple->GetTransform()
		.SetScale(Vector3(3, 3, 3))
		.SetPosition(position);

	apple->SetRenderObject(new RenderObject(&apple->GetTransform(), gooseMesh, nullptr, basicShader));
	apple->SetPhysicsObject(new PhysicsObject(&apple->GetTransform(), apple->GetBoundingVolume()));

	apple->GetPhysicsObject()->SetInverseMass(0.3f);
	apple->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(apple);

	return apple;
}



GameObject* TutorialGame::AddSphereToWorld(const Vector3& position, float radius, float inverseMass) {
	GameObject* sphere = new GameObject();

	Vector3 sphereSize = Vector3(radius, radius, radius);
	SphereVolume* volume = new SphereVolume(radius);
	sphere->SetBoundingVolume((CollisionVolume*)volume);
	sphere->setType("Sphere");
	sphere->GetTransform()
		.SetScale(sphereSize)
		.SetPosition(position);

	sphere->SetRenderObject(new RenderObject(&sphere->GetTransform(), sphereMesh, redTex, basicShader));
	sphere->SetPhysicsObject(new PhysicsObject(&sphere->GetTransform(), sphere->GetBoundingVolume()));

	sphere->GetPhysicsObject()->SetInverseMass(inverseMass);
	sphere->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(sphere);

	return sphere;
}

GameObject* TutorialGame::addEndToWorld(const Vector3& position) {
	GameObject* sphere = new GameObject();


	SphereVolume* volume = new SphereVolume(5.0f);
	sphere->SetBoundingVolume((CollisionVolume*)volume);
	sphere->setType("End");

	sphere->GetTransform()
		.SetScale(Vector3(10,10,10))
		.SetPosition(position);

	sphere->SetRenderObject(new RenderObject(&sphere->GetTransform(), appleMesh, redTex, basicShader));
	sphere->SetPhysicsObject(new PhysicsObject(&sphere->GetTransform(), sphere->GetBoundingVolume()));

	sphere->GetPhysicsObject()->SetInverseMass(0);
	sphere->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(sphere);

	return sphere;
}



GameObject* TutorialGame::AddCapsuleToWorld(const Vector3& position, float halfHeight, float radius, float inverseMass) {
	GameObject* capsule = new GameObject();

	CapsuleVolume* volume = new CapsuleVolume(halfHeight, radius);
	capsule->SetBoundingVolume((CollisionVolume*)volume);
	capsule->setType("Capsule");
	capsule->GetTransform()
		.SetScale(Vector3(radius* 2, halfHeight, radius * 2))
		.SetPosition(position);

	capsule->SetRenderObject(new RenderObject(&capsule->GetTransform(), capsuleMesh, basicTex, basicShader));
	capsule->SetPhysicsObject(new PhysicsObject(&capsule->GetTransform(), capsule->GetBoundingVolume()));

	capsule->GetPhysicsObject()->SetInverseMass(inverseMass);
	capsule->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(capsule);

	return capsule;

}

GameObject* TutorialGame::AddCubeToWorld(const Vector3& position, Vector3 dimensions, float inverseMass) {
	GameObject* cube = new GameObject();

	AABBVolume* volume = new AABBVolume(dimensions);

	cube->SetBoundingVolume((CollisionVolume*)volume);
	cube->setType("Cube");
	cube->GetTransform()
		.SetPosition(position)
		.SetScale(dimensions * 2);

	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, basicTex, basicShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	cube->GetPhysicsObject()->SetInverseMass(0.0f);
	cube->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(cube);

	return cube;
}

void TutorialGame::InitSphereGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, float radius) {
	for (int x = 0; x < numCols; ++x) {
		for (int z = 0; z < numRows; ++z) {
			Vector3 position = Vector3(x * colSpacing, 10.0f, z * rowSpacing);
			AddSphereToWorld(position, radius, 1.0f);
		}
	}

}
void TutorialGame::InitMoveGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing) {
	for (int x = 0; x < numCols; ++x) {
		for (int z = 0; z < numRows; ++z) {
			Vector3 position = Vector3(x * colSpacing, 10.0f, z * rowSpacing);
			AddStateObjectToWorld(position);
		}
	}

}

void TutorialGame::InitMixedGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing) {
	float sphereRadius = 1.0f;
	Vector3 cubeDims = Vector3(1, 1, 1);

	for (int x = 0; x < numCols; ++x) {
		for (int z = 0; z < numRows; ++z) {
			Vector3 position = Vector3(x * colSpacing, 10.0f, z * rowSpacing);

			if (rand() % 2) {
				AddCubeToWorld(position, cubeDims);
			}
			else {
				AddSphereToWorld(position, sphereRadius);
			}
		}
	}
}

void TutorialGame::InitCubeGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, const Vector3& cubeDims) {
	for (int x = 1; x < numCols+1; ++x) {
		for (int z = 1; z < numRows+1; ++z) {
			Vector3 position = Vector3(x * colSpacing, 5.0f, z * rowSpacing);
			AddCubeToWorld(position, cubeDims, 1.0f);
		}
	}
}

void TutorialGame::InitDefaultFloor() {
	AddFloorToWorld(Vector3(300, -2, 300));
}

void TutorialGame::InitGameExamples() {
	AddPlayerToWorld(Vector3(0, 5, 0));
	AddEnemyToWorld(Vector3(5, 5, 0));
	AddBonusToWorld(Vector3(10, 5, 0));
}

GameObject* TutorialGame::AddPlayerToWorld(const Vector3& position) {
	float meshSize = 3.0f;
	float inverseMass = 0.9f;

	GameObject* character = new GameObject();

	AABBVolume* volume = new AABBVolume(Vector3(0.3f, 0.85f, 0.3f) * meshSize);

	character->SetBoundingVolume((CollisionVolume*)volume);
	character->setType("Player");
	character->GetTransform()
		.SetScale(Vector3(meshSize, meshSize, meshSize))
		.SetPosition(position);

	if (rand() % 2) {
		character->SetRenderObject(new RenderObject(&character->GetTransform(), charMeshA, nullptr, basicShader));
	}
	else {
		character->SetRenderObject(new RenderObject(&character->GetTransform(), charMeshB, nullptr, basicShader));
	}
	character->SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));

	character->GetPhysicsObject()->SetInverseMass(inverseMass);
	character->GetPhysicsObject()->InitSphereInertia();
	lockedObject = character;
	world->AddGameObject(character);

	return character;
}

GameObject* TutorialGame::AddEnemyToWorld(const Vector3& position) {
	float meshSize		= 3.0f;
	float inverseMass	= 0.1f;

	GameObject* character = new GameObject();

	AABBVolume* volume = new AABBVolume(Vector3(0.3f, 0.9f, 0.3f) * meshSize);
	character->SetBoundingVolume((CollisionVolume*)volume);
	character->setType("Enemy");
	character->GetTransform()
		.SetScale(Vector3(meshSize, meshSize, meshSize))
		.SetPosition(position);

	character->SetRenderObject(new RenderObject(&character->GetTransform(), enemyMesh, nullptr, basicShader));
	character->SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));

	character->GetPhysicsObject()->SetInverseMass(inverseMass);
	character->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(character);

	return character;
}


GameObject* TutorialGame::AddBonusToWorld(const Vector3& position) {
	GameObject* apple = new GameObject();

	SphereVolume* volume = new SphereVolume(1.0f);
	apple->SetBoundingVolume((CollisionVolume*)volume);
	apple->setType("Bonus");

	apple->GetTransform()
		.SetScale(Vector3(0.25, 0.25, 0.25))
		.SetPosition(position);

	apple->SetRenderObject(new RenderObject(&apple->GetTransform(), bonusMesh, nullptr, basicShader));
	apple->SetPhysicsObject(new PhysicsObject(&apple->GetTransform(), apple->GetBoundingVolume()));

	apple->GetPhysicsObject()->SetInverseMass(1.0f);
	apple->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(apple);

	return apple;
}



bool TutorialGame::SelectObject() {
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::Q)) {
		inSelectionMode = !inSelectionMode;
		if (inSelectionMode) {
			Window::GetWindow()->ShowOSPointer(true);
			Window::GetWindow()->LockMouseToWindow(false);
		}
		else {
			Window::GetWindow()->ShowOSPointer(false);
			Window::GetWindow()->LockMouseToWindow(true);
		}
	}
	if (inSelectionMode) {
		renderer->DrawString("Press Q to change to camera mode!", Vector2(5, 85));

		if (Window::GetMouse()->ButtonDown(NCL::MouseButtons::LEFT)) {
			if (selectionObject) {	//set colour to deselected;
				selectionObject->GetRenderObject()->SetColour(Vector4(1, 1, 1, 1));
				selectionObject = nullptr;

			}

			Ray ray = CollisionDetection::BuildRayFromMouse(*world->GetMainCamera());

			RayCollision closestCollision;
			if (world->Raycast(ray, closestCollision, true)) {
				selectionObject = (GameObject*)closestCollision.node;
				selectionObject->GetRenderObject()->SetColour(Vector4(0, 1, 0, 1));
				return true;
			}
			else {
				return false;
			}
		}
	}
	else {
		renderer->DrawString("Press Q to change to select mode!", Vector2(5, 85));
	}


	return false;
}



void TutorialGame::MoveSelectedObject() {
	//forceMagnitude  +=  Window :: GetMouse()-> GetWheelMovement () * 100.0f;

		if (! selectionObject) {
			renderer->DrawString("Press an object to get info", Vector2(10, 20));

			return;
		}
			Ray  ray = CollisionDetection::BuildRayFromMouse(* world->GetMainCamera());  
				RayCollision  closestCollision; 
				if (world->Raycast(ray, closestCollision, true)) {

				//if (closestCollision.node == selectionObject) {										  //If enabled, will only show if hovering over selected object
						renderer->DrawString("Object is " + selectionObject->getType(), Vector2(10, 20));

				//}
			}
		
}