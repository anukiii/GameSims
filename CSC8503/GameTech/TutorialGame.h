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
			bool isWin() { return win; }


		protected:
			StateGameObject* AddStateObjectToWorld(const  Vector3& position);

			void InitCamera();
			void UpdateKeys();

			void InitWorld();

			void InitGameExamples();

			void InitSphereGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, float radius);
			void InitMixedGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing);
			void InitCubeGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, const Vector3& cubeDims);
			void InitDefaultFloor();
			void BridgeConstraintTest();
	
			bool SelectObject();
			void MoveSelectedObject();
			void DebugObjectMovement();
			void LockedObjectMovement();
			
			
			
			GameObject* addEndToWorld(const Vector3& position);
			GameObject* AddFloorToWorld(const Vector3& position);
			GameObject* AddSphereToWorld(const Vector3& position, float radius, float inverseMass = 10.0f);
			GameObject* AddCubeToWorld(const Vector3& position, Vector3 dimensions, float inverseMass = 10.0f);
			
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
			bool win;

			float		forceMagnitude;

			GameObject* selectionObject = nullptr;

			OGLMesh*	capsuleMesh = nullptr;
			OGLMesh*	cubeMesh	= nullptr;
			OGLMesh*	sphereMesh	= nullptr;
			OGLTexture* basicTex	= nullptr;
			OGLTexture* redTex	= nullptr;
			OGLShader*	basicShader = nullptr;
			OGLMesh*	gooseMesh = nullptr;
			OGLMesh*	appleMesh = nullptr;

			//Coursework Meshes
			OGLMesh*	charMeshA	= nullptr;
			OGLMesh*	charMeshB	= nullptr;
			OGLMesh*	enemyMesh	= nullptr;
			OGLMesh*	bonusMesh	= nullptr;

			StateGameObject* testStateObject = nullptr;


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

		};
	}
}

