// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_ProjectileAttack.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include <Kismet/KismetMathLibrary.h>
#include <TimerManager.h>




USAction_ProjectileAttack::USAction_ProjectileAttack()
{
	HandSocketName = "Muzzle_01";
	AttackAnimDelay = 0.18f;
}


void USAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	//Cast to Character to get the mesh location
	ACharacter* Character = Cast<ACharacter>(Instigator);
	if (Character)
	{
		//Play Attack Animation
		Character->PlayAnimMontage(AttackAnim);
		//Muzzle Flash
		UGameplayStatics::SpawnEmitterAttached(CastingEffect, Character->GetMesh(), HandSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
		//Timer Delay before Projectile Spawn
		FTimerHandle TimerHandle_AttackDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "AttackDelay_Elapsed", Character);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackAnimDelay, false);
	}
}


void USAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	if (ensureAlways(ProjectileClass))
	{
		//Get Camera Manager
		//APlayerCameraManager* CurrentCamera = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
		//Get Camera information for raycast & set up collision channel
		FHitResult Hit;

		FVector CamLoc = InstigatorCharacter->GetPawnViewLocation();
		FRotator CamRot = InstigatorCharacter->GetControlRotation();

		//Raycast Length
		FVector End = CamLoc + (CamRot.Vector() * 10000);
		FVector HitEnd;

		FCollisionShape Shape;
		Shape.SetSphere(10.0f);

		//Raycast must ignore actor
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(InstigatorCharacter);

		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);

		bool bBlockingHit = GetWorld()->SweepSingleByObjectType(Hit, CamLoc, End, FQuat::Identity, ObjParams, Shape, Params);

		//If hit get vector to hit location
		if (bBlockingHit)
		{
			End = Hit.Location;
		}

		//Set up Spawn Location and Rotation for Projectile
		FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);
		FRotator FinalRot = UKismetMathLibrary::FindLookAtRotation(HandLocation, End);
		FTransform SpawnTM = FTransform(FinalRot, HandLocation);

		//Spawn Projectile
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = InstigatorCharacter;

		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
	}

	StopAction(InstigatorCharacter);
}	
