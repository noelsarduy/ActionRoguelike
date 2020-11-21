// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include <EnvironmentQuery/EnvQuery.h>
#include "SGameModeBase.generated.h"

class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;
class USAffinityFactionComponent;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> MinionClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve;
	
	FTimerHandle TimerHandle_SpawnBots;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	float RespawnTimeDelay;

	// Read/write access as we could change this as our difficulty increases via Blueprint
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
		int32 CreditsPerKill;

	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
		UEnvQuery* PowerupSpawnQuery;

	/* All power-up classes used to spawn with EQS at match start */
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
		TArray<TSubclassOf<AActor>> PowerupClasses;

	/* Distance required between power-up spawn locations */
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
		float RequiredPowerupDistance;

	/* Amount of powerups to spawn during match start */
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
		int32 DesiredPowerupCount;
	
	UFUNCTION()
	void SpawnBotTimerElapsed();

	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void OnPowerupSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus); 
		
	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);

	UPROPERTY(EditAnywhere, Category = "Factions")
	TSet<TSubclassOf<USFactions>> AssignableGameFactions;

	UPROPERTY(EditAnywhere, Category = "Factions")
	TSet<USFactions*> GameFactions;
;
public:
	UFUNCTION(BlueprintCallable, Category = "Faction Affinity")
	void SetAffinityBetweenFactions(USFactions* Faction_1, USFactions* Faction_2, int32 Delta);

	UFUNCTION(BlueprintCallable, Category = "Faction Affinity")
	TSet<USFactions*> GetGameFactions();
// 
// 	UFUNCTION(BlueprintCallable, Category = "Actions")
// 	void AddFaction(TSubclassOf<USFactions> Faction);
// 
// 	UFUNCTION(BlueprintCallable, Category = "Actions")
// 	void RemoveFaction(USFactions* FactionToRemove);

	
	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);


	
	ASGameModeBase();
	
	virtual void StartPlay() override;

	UFUNCTION(Exec)
	void KillAll();
};
