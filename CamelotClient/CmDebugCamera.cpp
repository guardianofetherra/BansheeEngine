#include "CmDebugCamera.h"
#include "CmInput.h"
#include "CmVector3.h"
#include "CmTime.h"
#include "CmMath.h"
#include "CmGameObject.h"
#include "CmCamera.h"

namespace CamelotEngine
{
	const float DebugCamera::START_SPEED = 40.0f;
	const float DebugCamera::TOP_SPEED = 130.0f;
	const float DebugCamera::ACCELERATION = 10.0f;
	const float DebugCamera::FAST_MODE_MULTIPLIER = 2.0f;
	const float DebugCamera::ROTATION_SPEED = 0.5f; // Degrees/pixel

	DebugCamera::DebugCamera(GameObjectPtr parent)
		:Component(parent), mGoingForward(false), mGoingBack(false), mGoingLeft(false), mGoingRight(false), mFastMove(false), mCameraRotating(false)
	{
		mCamera = gameObject()->getComponent<Camera>();
		mCamera->setNearClipDistance(5);

		//gameObject()->setPosition(Vector3(0,0,5050));
		//gameObject()->lookAt(Vector3(0,0,-300));

		gameObject()->setPosition(Vector3(0,0,0));
		gameObject()->lookAt(Vector3(0,0,-1));

		gInput().onKeyDown.connect(boost::bind(&DebugCamera::keyDown, this, _1));
		gInput().onKeyUp.connect(boost::bind(&DebugCamera::keyUp, this, _1));

		gInput().onMouseDown.connect(boost::bind(&DebugCamera::mouseDown, this, _1, _2));
		gInput().onMouseUp.connect(boost::bind(&DebugCamera::mouseUp, this, _1, _2));
	}

	void DebugCamera::keyDown(KeyCode keyCode)
	{
		if (keyCode == KC_W || keyCode == KC_UP) 
			mGoingForward = true;
		else if (keyCode == KC_S || keyCode== KC_DOWN) 
			mGoingBack = true;
		else if (keyCode == KC_A || keyCode == KC_LEFT) 
			mGoingLeft = true;
		else if (keyCode == KC_D || keyCode == KC_RIGHT) 
			mGoingRight = true;
		else if (keyCode == KC_LSHIFT) 
			mFastMove = true;
	}

	void DebugCamera::keyUp(KeyCode keyCode)
	{
		if (keyCode == KC_W || keyCode == KC_UP) 
			mGoingForward = false;
		else if (keyCode == KC_S || keyCode== KC_DOWN) 
			mGoingBack = false;
		else if (keyCode == KC_A || keyCode == KC_LEFT) 
			mGoingLeft = false;
		else if (keyCode == KC_D || keyCode == KC_RIGHT) 
			mGoingRight = false;
		else if (keyCode == KC_LSHIFT) 
			mFastMove = false;
	}

	void DebugCamera::mouseDown(const MouseEvent& event, MouseButton buttonID)
	{
		if(buttonID == MB_Right)
		{
			mCameraRotating = true;
		}
	}

	void DebugCamera::mouseUp(const MouseEvent& event, MouseButton buttonID)
	{
		if(buttonID == MB_Right)
		{
			mCameraRotating = false;
		}
	}

	void DebugCamera::update()
	{
		Vector3 direction = Vector3::ZERO;
		if (mGoingForward) direction += GO()->getForward();
		if (mGoingBack) direction -= GO()->getForward();
		if (mGoingRight) direction += GO()->getRight();
		if (mGoingLeft) direction -= GO()->getRight();

		if (direction.squaredLength() != 0)
		{
			direction.normalise();

			float multiplier = 1.0f;
			if(mFastMove)
				multiplier = FAST_MODE_MULTIPLIER;

			mCurrentSpeed = Math::Clamp(mCurrentSpeed + ACCELERATION * gTime().getFrameDelta(), START_SPEED, TOP_SPEED);
			mCurrentSpeed *= multiplier;
		}
		else
		{
			mCurrentSpeed = 0.0f;
		}

		float tooSmall = std::numeric_limits<float>::epsilon();
		if(mCurrentSpeed > tooSmall)
		{
			Vector3 velocity = direction * mCurrentSpeed;
			GO()->move(velocity * gTime().getFrameDelta());
		}

		if(mCameraRotating)
		{
			//float horizontalSpeed = event.relX / gTime().getFrameDelta();

			GO()->yaw(Degree(gInput().getHorizontalAxis() * ROTATION_SPEED));
			GO()->pitch(Degree(gInput().getVerticalAxis() * ROTATION_SPEED));

			// Prevent roll due to inprecision
			Vector3 validRight = GO()->getForward().crossProduct(Vector3::UP);

			if(GO()->getForward().dotProduct(Vector3::UP) > 0.5f)
			{
				validRight = GO()->getForward().crossProduct(-Vector3::FORWARD);
			}

			Quaternion rightRot = GO()->getRight().getRotationTo(validRight);
			//if(validRight.dotProduct(Vector3::RIGHT) < 0.0f)
			//{
			//	rightRot.w = -rightRot.w;
			//}


			GO()->setRotation(GO()->getRotation() * rightRot);
		}
	}
}