// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SGameStateBase.generated.h"

class USFactions;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "Factions")
	TSet<USFactions*> AllPreSetFactions;

public:



// 	UFUNCTION(BlueprintCallable, Category = "Faction Affinity")
// 	void SetAffinityBetweenFactions(USFactions* Faction_1, USFactions* Faction_2, int32 Delta);

	ASGameStateBase();


};
