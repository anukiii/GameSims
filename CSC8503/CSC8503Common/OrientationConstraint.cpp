#include "OrientationConstraint.h"
#include "PositionConstraint.h"
#include <vector>

namespace NCL {
	namespace CSC8503 {
		void OrientationConstraint::UpdateConstraint(float dt)
		{

			Vector3 orientation = objectA->GetTransform().GetOrientation().ToEuler();

			

			if (Y<=0 && orientation.y <= Y) {
				objectA->GetPhysicsObject()->AddTorque(Vector3(0, -objectA->GetPhysicsObject()->GetTorque().y * 3, 0));
			}
			else if  (Y > 0 && orientation.y >= Y) {
				objectA->GetPhysicsObject()->AddTorque(Vector3(0, -objectA->GetPhysicsObject()->GetTorque().y * 3, 0));
			}


		}
	}
}