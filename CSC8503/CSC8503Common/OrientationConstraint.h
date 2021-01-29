#pragma once
#include "Constraint.h"
#include "GameObject.h"
namespace NCL {
	namespace CSC8503 {
		class GameObjects;

		class OrientationConstraint : public Constraint {
		public:
			OrientationConstraint(GameObject* a, float MaxY) {
				objectA = a;
				Y = MaxY;
				
			}
			~OrientationConstraint() {}

			void  UpdateConstraint(float dt) override;

		protected:
			GameObject* objectA;


			float  Y;
			
		};
	}
}


