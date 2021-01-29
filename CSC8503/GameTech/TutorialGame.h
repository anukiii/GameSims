#pragma once
#include "GameTechRenderer.h"
#include "../GameTech/StateGameObject.h"
#include "../CSC8503Common/PhysicsSystem.h"


namespace NCL {
	namespace CSC8503 {
		class TutorialGame		{
		public:
			TutorialGame();
			~TutorialGame();

			virtual void UpdateGame(float dt);
			void setScene(int newScene);
			int getScore() { return score; }
			void InitialiseAssets();
			int isWin() { return win; }
			void setGameType(int newGame) { gameType = newGame; }

		protected:
			StateGameObject* AddStateObjectToWorld(const  Vector3& position);

			void InitCamera();


			void InitWorld();

			void InitGameExamples();

			void InitSphereGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, float radius);
			void InitMixedGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing);
			void InitCubeGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, const Vector3& cubeDims);
			void InitMoveGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing);
			void InitBonusWorld(int numRows, int numCols, float rowSpacing, float colSpacing);
			void InitDefaultFloor();
			void BridgeConstraintTest(Vector3 Pos);
	
			bool SelectObject();
			void MoveSelectedObject();
			void LockedObjectMovement(float dt);
			
			void enemyPathfindingSetup();
			void enemyPathfinding(GameObject * enemy);
			
			GameObject* addEndToWorld(const Vector3& position);
			GameObject* AddFloorToWorld(const Vector3& position);
			GameObject* AddSphereToWorld(const Vector3& position, float radius, float inverseMass = 10.0f);
			GameObject* AddCubeToWorld(const Vector3& position, Vector3 dimensions, float inverseMass = 10.0f);
			GameObject* AddMovableCubeToWorld(const Vector3& position, Vector3 dimensions, float inverseMass);
			GameObject* AddCapsuleToWorld(const Vector3& position, float halfHeight, float radius, float inverseMass = 10.0f);

			GameObject* AddPlayerToWorld(const Vector3& position);
			GameObject* AddEnemyToWorld(const Vector3& position);
			GameObject* AddBonusToWorld(const Vector3& position);
			
			void endScreen(float dt);
			void winScreen(float dt);
			void mainGame(float dt);
			void mainMenu(float dt);
			



			GameTechRenderer*	renderer;
			PhysicsSystem*		physics;
			GameWorld*			world;

			bool useGravity;
			bool inSelectionMode;
			int win;
			int gameType;

			float		forceMagnitude;

			GameObject* selectionObject = nullptr;

			OGLMesh*	capsuleMesh = nullptr;
			OGLMesh*	cubeMesh	= nullptr;
			OGLMesh*	sphereMesh	= nullptr;
			OGLTexture* basicTex	= nullptr;
			OGLTexture* redTex	= nullptr;
			OGLTexture* purpleTex	= nullptr;
			OGLTexture* blueTex	= nullptr;
			OGLShader*	basicShader = nullptr;
			OGLMesh*	gooseMesh = nullptr;
			OGLMesh*	appleMesh = nullptr;

			//Coursework Meshes
			OGLMesh*	charMeshA	= nullptr;
			OGLMesh*	charMeshB	= nullptr;
			OGLMesh*	enemyMesh	= nullptr;
			OGLMesh*	bonusMesh	= nullptr;

			vector<StateGameObject*> StateObjects;

			vector <Vector3 > testNodes;
			int currentNode;
			int enemyStuckCounter;

			//Coursework Additional functionality	
			GameObject* lockedObject = nullptr;
			Vector3 lockedOffset		= Vector3(0, 10, 30);
			void LockCameraToObject(GameObject* o) {
				lockedObject = o;
			}
			time_t  start;
			int Bonuses;
			int score;
			int scene; // 0 is main menu, 1, is level, 2 is end screen, 3 is win
			float YMax;
		};
	}
}

