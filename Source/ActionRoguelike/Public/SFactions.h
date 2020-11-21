// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SFactions.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API USFactions : public UObject
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USFactions();

protected:

	// Name of the faction
	UPROPERTY(EditAnywhere, Category = "Faction Data")
		FName FactionName;

	UPROPERTY(EditAnywhere, Category = "Faction Data")
		TMap<TSubclassOf<USFactions>, int32> AssignableFactionAffinites;
	
	UPROPERTY(EditAnywhere, Category = "Faction Data")
		TMap<USFactions*, int32> FactionAffinites;

	UPROPERTY(EditAnywhere, Category = "Faction Data")
		TArray<TSubclassOf<AActor>> FactionMemebersAssignable;

	UPROPERTY(EditAnywhere, Category = "Faction Data")
		TArray<AActor*> FactionMemebers;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Faction Affinity")
		int32 FactionFriendlyThreshold;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Faction Affinity")
		int32 FactionNeutralThreshold;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Faction Affinity")
		int32 FactionEnemyThreshold;

public:

	UFUNCTION(BlueprintCallable, Category = "Faction Affinity")
	int32 GetFactionAffinity(USFactions* Faction) const;

	UFUNCTION(BlueprintCallable, Category = "Faction Affinity")
	FName GetFactionName() const;
	
	UFUNCTION(BlueprintCallable, Category = "Faction Affinity")
	TArray<AActor*> GetFactionMemebers();
	
	UFUNCTION(BlueprintCallable, Category = "Faction Affinity")
	int32 GetFactionFriendlyThreshold() const;

	UFUNCTION(BlueprintCallable, Category = "Faction Affinity")
	int32 GetFactionNeutralThreshold() const;

	UFUNCTION(BlueprintCallable, Category = "Faction Affinity")
	int32 GetFactionEnemyThreshold() const;

	UFUNCTION(BlueprintCallable, Category = "Faction Affinity")
	TMap<USFactions*, int32>GetFactionAffinities() const;

	UFUNCTION(BlueprintCallable, Category = "Faction Affinity")
	void SetFactionAffinities(USFactions* OtherFaction, int32 Delta);

	UFUNCTION(BlueprintCallable, Category = "Faction Affinity")
	void InitializeGameFactions(TSet < TSubclassOf<USFactions>> FactionsFromGameMode, TSet<USFactions*> GameFactions);
	
// 	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Faction Affinity")
// 	void AddCharacterToFaction(TSubclassOf<AActor> Character);
// 
// 	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Faction Affinity")
// 	void RemoveCharacterFromFaction(TSubclassOf<AActor> Character);
// 
// 	UFUNCTION(BlueprintCallable, Category = "Faction Affinity")
// 	TMap<TSubclassOf<USFactions>, int32> GetDefaultFactionAffinites();
};