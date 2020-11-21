// Fill out your copyright notice in the Description page of Project Settings.


#include "SSingularity.h"
#include <GameFramework/Pawn.h>
#include <AIController.h>
#include <GameFramework/Character.h>
#include <Particles/ParticleSystemComponent.h>
#include <Components/AudioComponent.h>
#include "BrainComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/SAICharacter.h"
#include <DrawDebugHelpers.h>
#include "SActionComponent.h"
#include "SActionEffect.h"
#include <GameFramework/Actor.h>
#include <Math/UnrealMathUtility.h>

// Sets default values
ASSingularity::ASSingularity()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetSphereRadius(800.0f);
	
	//SphereComp->SetCollisionProfileName("RadialEffects");
	RootComponent = SphereComp;

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(RootComponent);

	SoundComponent = CreateDefaultSubobject<UAudioComponent>("SoundComp");
	SoundComponent->SetupAttachment(RootComponent);
	SoundComponent->bStopWhenOwnerDestroyed = true;

	PullComponent = CreateDefaultSubobject <URadialForceComponent>("Shockwave");
	PullComponent->SetupAttachment(SphereComp);
	PullComponent->ForceStrength = -80000.0f;
	PullComponent->Radius = 600.0f;
	PullComponent->bIgnoreOwningActor;	

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASSingularity::OnActorOverlap);
	SphereComp->OnComponentEndOverlap.AddDynamic(this, &ASSingularity::OnActorEndOverlap);
// 	
// 	SingularityDuration = 6.0f;

	PrimaryActorTick.bCanEverTick = true;
	bUpdatePosition = true;
}



void ASSingularity::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	USActionComponent* ActionComp = Cast<USActionComponent>(OtherActor->GetComponentByClass(USActionComponent::StaticClass()));
	if (ActionComp && OtherActor != GetInstigator()) 
	{
		ActionComp->AddAction(OtherActor, StasisActionClass);
	}
	
// 	DrawDebugSphere(GetWorld(), OtherActor->GetActorLocation(), 20, 32, FColor::Red, false, 2.0f);
// 	if (OtherActor && OtherActor != GetInstigator())
// 	{	
// 		if (OtherActor->IsA(ASAICharacter::StaticClass()))
// 		{	
// 			
// 			ASAICharacter* TargetPawn = Cast<ASAICharacter>(OtherActor);
// 			
// 			AAIController* AIC = Cast<AAIController>(TargetPawn->GetController());
// 			if (AIC)
// 			{
// 				AIC->GetBrainComponent()->StopLogic("Stasis");
// 			}
// 
// 			USActionComponent* ActionComp = Cast<USActionComponent>(OtherActor->GetComponentByClass(USActionComponent::StaticClass()));
// 			if (ActionComp)
// 			{
// 				ActionComp->AddAction(GetInstigator(), DamageActionClass);
// 			}
// 			
// 			USkeletalMeshComponent* MeshComp = TargetPawn->GetMesh();
// 
// 			if (MeshComp)
// 			{
// // 				FTimerHandle AnimationDelayHandle;
// // 				float AnimDelay = 3.0f;
// // 
// //				TargetPawn->PlayAnimMontage(StasisAnim_1);
// 				MeshComp->SetAllBodiesSimulatePhysics(true);
// 				MeshComp->SetCollisionProfileName("Ragdoll");
// 				TargetPawn->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
// 				
// 				Victims.Add(OtherActor);
// 			}	
// 			
// 		}
// 	}
}


// void ASSingularity::EndSingularity_Implementation()
// {
// // 	PullComponent->Deactivate();
// // 	bUpdatePosition = false;
// // 	for (AActor* Victim: Victims)
// // 	{	
// // 		SetUpRagdoll_Implementation(Victim);
// // 	}
// 
// 	Destroy();
// }


void ASSingularity::OnActorEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	/*SetUpRagdoll_Implementation(OtherActor);*/
}


// void ASSingularity::SetUpRagdoll_Implementation(AActor* VictimActor)
// {
// 	if (VictimActor && VictimActor != GetInstigator())
// 	{
// 		if (VictimActor->IsA(ASAICharacter::StaticClass()))
// 		{
// 			ASAICharacter* TargetPawn = Cast<ASAICharacter>(VictimActor);
// 
// 			USkeletalMeshComponent* MeshComp = TargetPawn->GetMesh();
// 			if (MeshComp)
// 			{
// 				USAttributeComponent* InstAttributeComp = USAttributeComponent::GetAttributes(VictimActor);
// 				if (InstAttributeComp->IsAlive())
// 				{
// 					
// 
// 					FVector DefaultLocation = FVector(0.0f, 0.0f, -90.0f);
// 					FRotator DefaultRotation = FRotator(0.0f, -90.0f, 0.0f);
// 
// 					
// 
// // 					MeshComp->SetWorldLocation(TargetPawn->GetCapsuleComponent()->GetComponentLocation());
// // 					MeshComp->SetWorldRotation(TargetPawn->GetCapsuleComponent()->GetComponentRotation());
// // 					
// // 					MeshComp->SetRelativeLocation(DefaultLocation);
// // 					MeshComp->SetRelativeRotation(DefaultRotation);
// 					
// 					MeshComp->SetAllBodiesSimulatePhysics(false);
// 					MeshComp->SetCollisionProfileName("None");
// 
// 					TargetPawn->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
// 
// 					//MeshComp->PlayAnimation(StasisAnim_2, false);
// 
// 
// 					AAIController* AIC = Cast<AAIController>(TargetPawn->GetController());
// 					if (AIC)
// 					{
// 						AIC->GetBrainComponent()->StartLogic();
// 					}
// 					
// 					Victims.Remove(VictimActor);
// 				}
// 			}
// 		}
// 	}
// }



void ASSingularity::BeginPlay()
{
	Super::BeginPlay();
	
/*	GetWorldTimerManager().SetTimer(SingularityDurationTimerHandle, this, &ASSingularity::EndSingularity, SingularityDuration);*/
	
}

void ASSingularity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
// 	if(bUpdatePosition)
// 	{
// 		for (AActor* Victim : Victims)
// 		{
// 			USAttributeComponent* InstAttributeComp = USAttributeComponent::GetAttributes(Victim);
// 			if (Victim != GetInstigator() && InstAttributeComp->IsAlive())
// 			{
// 				ASAICharacter* TargetPawn = Cast<ASAICharacter>(Victim);
// 				USkeletalMeshComponent* MeshComp = TargetPawn->GetMesh();
// 				UCapsuleComponent* TargetCapsule = TargetPawn->GetCapsuleComponent();
// 				
// 				FVector WorldLocation = MeshComp->GetSocketLocation("pelvis");
// 				WorldLocation.Z = WorldLocation.Z + 90;
// 
// 				FRotator WorldRotation = MeshComp->GetSocketRotation("pelvis");
//  				FVector ModifiedRotation = WorldRotation.Vector();
//  				ModifiedRotation[0] = 0;
//  				ModifiedRotation[2] = 0;
// 				WorldRotation = ModifiedRotation.Rotation();
// 				
// 				TargetCapsule->SetWorldLocation(WorldLocation, false);
// 				TargetCapsule->SetWorldRotation(WorldRotation, false);
// 
// 			}
// 		}
// 	}
}