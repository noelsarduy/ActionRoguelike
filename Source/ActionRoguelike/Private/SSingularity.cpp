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
#include <TimerManager.h>
#include <GameFramework/Actor.h>
#include <Components/CapsuleComponent.h>

// Sets default values
ASSingularity::ASSingularity()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetSphereRadius(600.0f);
	
	//SphereComp->SetCollisionProfileName("RadialEffects");
	RootComponent = SphereComp;

	//SphereComp->OnComponentDestroyed(true).AddDynamic(this, &ASSingularity::OnActorEndOverlap);

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
	
	SingularityDuration = 6.0f;

	bUpdatePosition = true;
}



void ASSingularity::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	DrawDebugSphere(GetWorld(), OtherActor->GetActorLocation(), 20, 32, FColor::Red, false, 2.0f);
	if (OtherActor && OtherActor != GetInstigator())
	{
		
		if (OtherActor->IsA(ASAICharacter::StaticClass()))
		{	
			DrawDebugSphere(GetWorld(), OtherActor->GetActorLocation(), 20, 32, FColor::Green, false, 2.0f);
			
			ASAICharacter* TargetPawn = Cast<ASAICharacter>(OtherActor);
			
			AAIController* AIC = Cast<AAIController>(TargetPawn->GetController());
			if (AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Stasis");
			}

			USActionComponent* ActionComp = Cast<USActionComponent>(OtherActor->GetComponentByClass(USActionComponent::StaticClass()));
			if (ActionComp)
			{
				ActionComp->AddAction(GetInstigator(), DamageActionClass);
			}
			
			//TargetPawn->GetCharacterMovement()->DisableMovement();

		
			
			USkeletalMeshComponent* MeshComp = TargetPawn->GetMesh();

			if (MeshComp)
			{
				FTimerHandle AnimationDelayHandle;
				float AnimDelay = 3.0f;

				//MeshComp->PlayAnimation(StasisAnim_1, false);
				MeshComp->SetAllBodiesSimulatePhysics(true);
				MeshComp->SetCollisionProfileName("Ragdoll");
			}	
			
		}
	}
}


void ASSingularity::EndSingularity_Implementation()
{
	PullComponent->Deactivate();

	bUpdatePosition = false;

	TArray<AActor*> Victims;
	SphereComp->GetOverlappingActors(Victims, APawn::StaticClass());

	for (AActor* Victim: Victims)
	{
		if (Victim != GetInstigator())
		{
			 if (Victim->IsA(ASAICharacter::StaticClass()))
			 {
				ASAICharacter* TargetPawn = Cast<ASAICharacter>(Victim);

			 	USkeletalMeshComponent* MeshComp = TargetPawn->GetMesh();
			 	if (MeshComp)
			 	{
					USAttributeComponent* InstAttributeComp = USAttributeComponent::GetAttributes(Victim);
					if (InstAttributeComp->IsAlive())
					{ 
						FVector DefaultLocation = FVector(0.0f, 0.0f, -90.0f);
						FRotator DefaultRotation = FRotator(0.0f, -90.0f, 0.0f);

						UCapsuleComponent* TargetCapsule = TargetPawn->GetCapsuleComponent();

						FVector WorldLocation = MeshComp->GetSocketLocation("pelvis");
					
						FRotator WorldRotation = MeshComp->GetComponentRotation();
						FVector ModifiedRotation = WorldRotation.Vector();
						ModifiedRotation[0] = 0;
						ModifiedRotation[2] = 0;

						WorldRotation = ModifiedRotation.Rotation();

						MeshComp->SetAllBodiesSimulatePhysics(false);
						MeshComp->SetCollisionProfileName("None");
						
						TargetCapsule->SetWorldLocation(WorldLocation, false, nullptr, ETeleportType::TeleportPhysics);
						TargetCapsule->SetWorldRotation(WorldRotation, false, nullptr, ETeleportType::TeleportPhysics);

						MeshComp->AttachToComponent(TargetCapsule, FAttachmentTransformRules::SnapToTargetIncludingScale);
						
						MeshComp->SetRelativeLocation(DefaultLocation);
						MeshComp->SetRelativeRotation(DefaultRotation);
						
						//MeshComp->PlayAnimation(StasisAnim_2, false);
					

						AAIController* AIC = Cast<AAIController>(TargetPawn->GetController());
						if (AIC)
						{
							AIC->GetBrainComponent()->StartLogic();
						}
					//TargetPawn->GetCharacterMovement()->ResetMoveState();	
					}
				}
			
					
					
			 }
		}
	}

	Destroy();
}

void ASSingularity::OnActorEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		if (OtherActor->IsA(ASAICharacter::StaticClass()))
		{
			ASAICharacter* TargetPawn = Cast<ASAICharacter>(OtherActor);

			USkeletalMeshComponent* MeshComp = TargetPawn->GetMesh();
				if (MeshComp)
				{
					USAttributeComponent* InstAttributeComp = USAttributeComponent::GetAttributes(OtherActor);
					if (InstAttributeComp->IsAlive())
					{
						FVector DefaultLocation = FVector(0.0f, 0.0f, -90.0f);
						FRotator DefaultRotation = FRotator(0.0f, -90.0f, 0.0f);

						UCapsuleComponent* TargetCapsule = TargetPawn->GetCapsuleComponent();

						FVector WorldLocation = MeshComp->GetSocketLocation("pelvis");

						FRotator WorldRotation = MeshComp->GetComponentRotation();
						FVector ModifiedRotation = WorldRotation.Vector();
						ModifiedRotation[0] = 0;
						ModifiedRotation[2] = 0;

						WorldRotation = ModifiedRotation.Rotation();

						MeshComp->SetAllBodiesSimulatePhysics(false);
						MeshComp->SetCollisionProfileName("None");

						TargetCapsule->SetWorldLocation(WorldLocation, false, nullptr, ETeleportType::TeleportPhysics);
						TargetCapsule->SetWorldRotation(WorldRotation, false, nullptr, ETeleportType::TeleportPhysics);

						MeshComp->AttachToComponent(TargetCapsule, FAttachmentTransformRules::SnapToTargetIncludingScale);

						MeshComp->SetRelativeLocation(DefaultLocation);
						MeshComp->SetRelativeRotation(DefaultRotation);

						//MeshComp->PlayAnimation(StasisAnim_2, false);


						AAIController* AIC = Cast<AAIController>(TargetPawn->GetController());
						if (AIC)
						{
							AIC->GetBrainComponent()->StartLogic();
						}
						//TargetPawn->GetCharacterMovement()->ResetMoveState();	
					}
				}
			
		}
	}
}

void ASSingularity::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(SingularityDurationTimerHandle, this, &ASSingularity::EndSingularity, SingularityDuration);
	
}

//Not ticking for some reason
void ASSingularity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bUpdatePosition)
	{
		//DrawDebugSphere(GetWorld(), GetActorLocation(), 20, 32, FColor::Green, false, 2.0f);

		TArray<AActor*> Victims;
		SphereComp->GetOverlappingActors(Victims, ASAICharacter::StaticClass());

		for (AActor* Victim : Victims)
		{
			if (Victim != GetInstigator())
			{
				ASAICharacter* TargetPawn = Cast<ASAICharacter>(Victim);

				USkeletalMeshComponent* MeshComp = TargetPawn->GetMesh();
	
				FVector Location = MeshComp->GetSocketLocation("pelvis");
				FRotator Rotation = MeshComp->GetComponentRotation();

				UCapsuleComponent* TargetCapsule = TargetPawn->GetCapsuleComponent();
						
				TargetCapsule->SetWorldLocation(Location, true);
				TargetCapsule->SetWorldRotation(Rotation, true);

				DrawDebugSphere(GetWorld(), TargetCapsule->GetComponentLocation(), 20, 32, FColor::Green, false, 2.0f);

			}
		}
	}
}