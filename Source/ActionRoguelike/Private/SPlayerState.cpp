// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"
#include <Math/UnrealMathUtility.h>

ASPlayerState::ASPlayerState()
{
	Credits = 0;
	Kills = 0;
	Deaths = 0;
	MaxCredits = 1000000;
}

int32 ASPlayerState::GetCredits() const
{
	return Credits;
}

bool ASPlayerState::ApplyCreditsChange(int32 Delta)
{

	int32 PrevCredits = Credits;

	Credits = FMath::Clamp(Credits + Delta, 0, MaxCredits);
	int32 ActualDelta = Credits - PrevCredits;

	OnCreditsChanged.Broadcast(this, Credits, ActualDelta);

	return ActualDelta != 0;
}

