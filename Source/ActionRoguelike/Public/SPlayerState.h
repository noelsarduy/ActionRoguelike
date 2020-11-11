// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include <Delegates/DelegateCombinations.h>
#include "SPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, ASPlayerState*, PlayerState, int32, NewCredits, int32, Delta);

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, ReplicatedUsing="OnRep_Credits", Category = "Credits")
	int32 Credits;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, ReplicatedUsing ="OnRep_Credits", Category = "Credits")
	int32 MaxCredits;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,/* ReplicatedUsing = "OnRep_Kills", */Category = "Stats")
	int32 Kills;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,/* ReplicatedUsing = "OnRep_Deaths", */Category = "Stats")
	int32 Deaths;

	UFUNCTION()
	void OnRep_Credits(int32 PrevCredits);

// 	UFUNCTION()
// 	void OnRep_Credits(int32 PrevDeaths);
// 	
// 	UFUNCTION()
// 	void OnRep_Credits(int32 PrevKills);

public:

	ASPlayerState();
	
	UFUNCTION(BlueprintCallable, Category = "Credits")
	int32 GetCredits() const;

	UFUNCTION(BlueprintCallable, Category = "Credits")
	bool ApplyCreditsChange(int32 Delta);

	//UFUNCTION(BlueprintCallable, Category = "Stats")
	//bool ApplyStatsChange(AActor* InstigatorActor, float Delta);

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCreditsChanged OnCreditsChanged;
};
