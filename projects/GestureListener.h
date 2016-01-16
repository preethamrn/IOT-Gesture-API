#ifndef _GESTURELISTENER_H_
#define _GESTURELISTENER_H_

#include <myo/myo.hpp>

class GestureListener : public myo::DeviceListener {
public:

	GestureListener();
	void onUnpair(myo::Myo* myo, uint64_t timestamp);
	void onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat);
	void onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose);
	void onArmSync(myo::Myo* myo, uint64_t timestamp, myo::Arm arm, myo::XDirection xDirection, float rotation,
		myo::WarmupState warmupState);
	void onArmUnsync(myo::Myo* myo, uint64_t timestamp);
	void onUnlock(myo::Myo* myo, uint64_t timestamp);
	void onLock(myo::Myo* myo, uint64_t timestamp);
	void print();

	// These values are set by onArmSync() and onArmUnsync() above.
	bool GestureListener::onArm;
	myo::Arm whichArm;

	// This is set by onUnlocked() and onLocked() above.
	bool GestureListener::isUnlocked;

	// These values are set by onOrientationData() and onPose() above.
	int roll_w, pitch_w, yaw_w;
	myo::Pose currentPose; 
};

#endif