// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <PhysicsEngine/RadialForceComponent.h>
#include <Components/SphereComponent.h>
#include "SSingularity.generated.h"

class USphereComponent;
class UParticleSystemComponent;
class UAudioComponent;
class URadialForceComponent;
class USActionEffect;

UCLASS()
class ACTIONROGUELIKE_API ASSingularity : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASSingularity();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components");
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components");
	UParticleSystemComponent* EffectComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components");
	UAudioComponent* SoundComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components");
	URadialForceComponent* PullComponent;

	UPROPERTY(EditAnywhere, Category = "StasisAnim")
	UAnimSequence* StasisAnim_1;

	UPROPERTY(EditAnywhere, Category = "StasisAnim")
	UAnimSequence* StasisAnim_2;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<USActionEffect> DamageActionClass;

	FTimerHandle SingularityDurationTimerHandle;

	bool bUpdatePosition;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	float SingularityDuration;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EndSingularity();


public:	

	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnActorEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;


};
