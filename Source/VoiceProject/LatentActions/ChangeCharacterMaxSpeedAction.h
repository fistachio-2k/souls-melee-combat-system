#pragma once

#include "LatentActions.h"

class FChangeCharacterMaxSpeedAction : public FPendingLatentAction
{
	float TargetMaxSpeed;
	float& SourceMaxSpeed;
	float ActionDuration;
	float ElapsedDuration;
	bool bIsDone;

public:
	FChangeCharacterMaxSpeedAction(float MaxSpeed, float& MaxSpeedToChange, float Duration)
		: TargetMaxSpeed(MaxSpeed), SourceMaxSpeed(MaxSpeedToChange), ActionDuration(Duration), ElapsedDuration(0.f), bIsDone(false) {}

	virtual void UpdateOperation(FLatentResponse& Response) override
	{
		ElapsedDuration += Response.ElapsedTime();
		SourceMaxSpeed = FMath::Lerp(SourceMaxSpeed, TargetMaxSpeed, Response.ElapsedTime());
		Response.DoneIf(ElapsedDuration >= ActionDuration || bIsDone);
	}

	void Done()
	{
		bIsDone = true;
	}
};
