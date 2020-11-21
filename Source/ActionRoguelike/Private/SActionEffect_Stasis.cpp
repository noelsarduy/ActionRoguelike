// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionEffect_Stasis.h"
#include "AI/SAICharacter.h"
#include <AIController.h>
#include "BrainComponent.h"
#include <GameFramework/Character.h>


USActionEffect_Stasis::USActionEffect_Stasis()
{
/*	bIsCreatedWhileRunning = GIsRunning;*/
	bUpdatePosition = true;
}

void USActionEffect_Stasis::StartAction_Implementation(AActor* Instigator)
{
	Victim = Instigator;

	Super::StartAction_Implementation(Instigator);

	ASAICharacter* TargetPawn = Cast<ASAICharacter>(Instigator);
	VictimMesh = TargetPawn->GetMesh();
	VictimCapsule = TargetPawn->GetCapsuleComponent();

	DefaultLocation = VictimMesh->GetRelativeLocation();
	DefaultRotation = VictimMesh->GetRelativeRotation();

	VictimMesh->SetAllBodiesBelowSimulatePhysics("pelvis", true);
	VictimMesh->SetAllBodiesBelowPhysicsBlendWeight("pelvis", 1.0f);
	VictimMesh->SetCollisionProfileName("Ragdoll");

	VictimCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AAIController* AIC = Cast<AAIController>(TargetPawn->GetController());
	if (AIC)
	{
		AIC->GetBrainComponent()->StopLogic("Stasis");
	}
}

void USActionEffect_Stasis::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);
	
	bUpdatePosition = false;
	ASAICharacter* TargetPawn = Cast<ASAICharacter>(Instigator);

	
	USAttributeComponent* InstAttributeComp = USAttributeComponent::GetAttributes(Instigator);
	
	if (InstAttributeComp->IsAlive())
	{
		WorldLocation = VictimMesh->GetSocketLocation("pelvis");
		/*WorldLocation.Z = WorldLocation.Z + 90;*/
		WorldRotation = VictimMesh->GetSocketRotation("pelvis");
		WorldRotation.Pitch = 0;
		WorldRotation.Roll = 0;

// 		VictimCapsule->SetWorldLocation(WorldLocation, false);
// 		VictimCapsule->SetWorldRotation(WorldRotation, false);
/*		WorldLocation = VictimCapsule->GetComponentLocation();*/
		WorldLocation.Z = WorldLocation.Z + 90;

/*		WorldRotation = VictimCapsule->GetComponentRotation();*/

		VictimCapsule->SetWorldLocation(WorldLocation);
		VictimCapsule->SetWorldRotation(WorldRotation);
		
		VictimMesh->SetWorldRotation(VictimCapsule->GetComponentRotation());
		
		VictimMesh->SetAllBodiesBelowSimulatePhysics("pelvis", false);
		VictimMesh->SetCollisionProfileName("None");
		
		VictimMesh->AttachTo(VictimCapsule,"None",EAttachLocation::SnapToTarget);

		VictimMesh->SetRelativeLocation(DefaultLocation);
		VictimMesh->SetRelativeRotation(DefaultRotation);



		VictimCapsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		AAIController* AIC = Cast<AAIController>(TargetPawn->GetController());
		if (AIC)
		{
			AIC->GetBrainComponent()->StartLogic();
		}
	}

}


// void USActionEffect_Stasis::Tick(float DeltaTime)
// {
// 	if(bUpdatePosition)
// 	{
// 		USAttributeComponent* InstAttributeComp = USAttributeComponent::GetAttributes(Victim);
// 		if (InstAttributeComp->IsAlive())
// 		{
// 
// 			WorldLocation = VictimMesh->GetSocketLocation("pelvis");
// 			/*WorldLocation.Z = WorldLocation.Z + 90;*/
// 			WorldRotation = VictimMesh->GetSocketRotation("pelvis");
// 
// 			VictimCapsule->SetWorldLocation(WorldLocation, false);
// 			VictimCapsule->SetWorldRotation(WorldRotation, false);
// 		}
// 	}
// }

// bool USActionEffect_Stasis::IsTickable() const
// {
// 	return bIsCreatedWhileRunning;
// }
// 
// TStatId USActionEffect_Stasis::GetStatId() const
// {
// 	return UObject::GetStatID();
// }
