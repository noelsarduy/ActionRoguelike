// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAffinityFactionComponent.generated.h"

class USFactions;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USAffinityFactionComponent : public UActorComponent
{

	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USAffinityFactionComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Faction Data")
	USFactions* OwnerFaction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aggro Data")
	TMap<AActor*, int32> Aggresors;

	UFUNCTION(BlueprintCallable, Category = "Faction Data")
	static USAffinityFactionComponent* GetFactionComponents(AActor* FromActor);
	
	UFUNCTION(BlueprintCallable, Category = "Faction Data")
	USFactions* GetFaction() const;

	UFUNCTION(BlueprintCallable, Category = "Aggro Data")
	void InitializeFaction (USFactions* OwningFaction);
	
	UFUNCTION(BlueprintCallable, Category = "Aggro Data")
 	int32 GetAggro(AActor* Instigator) const;

	UFUNCTION(BlueprintCallable, Category = "Aggro Data")
	void AddAggresion(AActor* Instigator, int32 AggroLevel);

	UFUNCTION(BlueprintCallable, Category = "Aggro Data")
	void RemoveAggresor(AActor* Instigator);
 		
 	UFUNCTION(BlueprintCallable, Category = "Aggro Data")
 	void SetAggro(AActor* Instigator, int32 AggroAmount);
 	 	
	UFUNCTION(BlueprintNativeEvent, Category = "Individual Affinity")
	bool IsEnemy(AActor* Instigator);
	
	UFUNCTION(BlueprintNativeEvent, Category = "Individual Affinity")
	bool IsFriendly(AActor* Instigator);
	
	UFUNCTION(BlueprintNativeEvent, Category = "Individual Affinity")
	bool IsNeutral(AActor* Instigator);

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aggro Data")
	int32 IndividualFriendlyThreshold;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aggro Data")
	int32 IndividualEnemyThreshold;



protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
