// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SActionEffect.h"
#include "SActionEffect_Stasis.generated.h"

class UCapsuleComponent;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USActionEffect_Stasis : public USActionEffect
{
	GENERATED_BODY()

public:
	//Enable Tick on Object to Update Ragdoll Position
// 	virtual void Tick(float DeltaTime) override;
// 	
// 	virtual bool IsTickable() const override;
// 	
// 	virtual TStatId GetStatId() const override;

	//End of adding ticking functionality

	void StartAction_Implementation(AActor* Instigator) override;

	void StopAction_Implementation(AActor* Instigator) override;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	bool bUpdatePosition;

	USActionEffect_Stasis();

	UPROPERTY(EditDefaultsOnly, Category = "RagdollSetup")
	FVector DefaultLocation;

	UPROPERTY(EditDefaultsOnly, Category = "RagdollSetup")
	FRotator DefaultRotation;

	UPROPERTY(EditDefaultsOnly, Category = "RagdollSetup")
	FVector WorldLocation;

	UPROPERTY(EditDefaultsOnly, Category = "RagdollSetup")
	FRotator WorldRotation;

	UPROPERTY(EditDefaultsOnly, Category = "CapsuleComponentSetup")
	AActor* Victim;

	UPROPERTY(EditDefaultsOnly, Category = "CapsuleComponentSetup")
	USkeletalMeshComponent* VictimMesh;

	UPROPERTY(EditDefaultsOnly, Category = "CapsuleComponentSetup")
	UCapsuleComponent* VictimCapsule;

private:
// 	bool bIsCreatedWhileRunning = false;
// 	
};
