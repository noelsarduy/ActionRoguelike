// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "BrainComponent.h"
#include "SWorldUserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SPlayerState.h"


ASAICharacter::ASAICharacter()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	TimeToHitParamName = "TimeOfHit";

	CreditValue = 5;
	AttributeComp->TeamNumber = 1;
}


void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
	AttributeComp->OnHealthChange.AddDynamic(this, &ASAICharacter::OnHealthChange);
}


void ASAICharacter::OnHealthChange(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{

	if (Delta < 0.0f)
	{
		//Check Instigator's Team
		USAttributeComponent* InstAttributeComp = USAttributeComponent::GetAttributes(InstigatorActor);
		
		if (InstigatorActor != this && InstAttributeComp)
		{
			if (InstAttributeComp->TeamNumber != this->AttributeComp->TeamNumber)
			{
			SetTargetActor(InstigatorActor);
			}

		}

		if (ActiveHealthBar == nullptr)
		{
			ActiveHealthBar = CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if (ActiveHealthBar)
			{
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();
			}
		}

		GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);

		if (NewHealth <= 0.0f /*&& Delta < 0.0f*/)
		{

			// Give Credits to Instigator 
			APawn*  Creditor = Cast<APawn>(InstigatorActor);
			ASPlayerState* InstigatorState = Creditor->GetPlayerState<ASPlayerState>();
			
			if (InstigatorState != nullptr)
			{
				InstigatorState->ApplyCreditsChange(CreditValue);
			}

			// Stop BT
			AAIController* AIC = Cast<AAIController>(GetController());
			if (AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Killed");
			}
		
			// ragdoll
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();
			// Allow BlackHole to consume
			GetMesh()->SetSimulatePhysics(true);

			// set lifespan
			SetLifeSpan(10.0f);

		}
	}

}

void ASAICharacter::SetTargetActor(AActor* NewTarget)
{

	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);
	}
}



void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	SetTargetActor(Pawn);
	DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true);

}