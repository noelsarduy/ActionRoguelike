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
#include "SActionComponent.h"
#include "SAffinityFactionComponent.h"


ASAICharacter::ASAICharacter()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

	// Ensures we receive a controlled when spawned in the level by our gamemode
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");

	FactionComp = CreateDefaultSubobject<USAffinityFactionComponent>("FactionComp");

	// Disabled on capsule to let projectiles pass through capsule and hit mesh instead
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	// Enabled on mesh to react to incoming projectiles
	GetMesh()->SetGenerateOverlapEvents(true);

	TimeToHitParamName = "TimeOfHit";
	TargetActorKey = "TargetActor";
	
	CreditValue = 5;
	AttributeComp->TeamNumber = 1;
}


void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChange);
}


void ASAICharacter::OnHealthChange(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{

	if (Delta < 0.0f)
	{
		//Check Instigator's Team
		USAffinityFactionComponent* InstFactionComp = USAffinityFactionComponent::GetFactionComponents(InstigatorActor);
		if (InstigatorActor != this && InstFactionComp)
		{
			FactionComp->AddAggresion(InstigatorActor, Delta);
			
			if (FactionComp->IsEnemy(InstigatorActor) || FactionComp->GetAggro(InstigatorActor) <= FactionComp->IndividualEnemyThreshold)
			{
				
				if (GetTargetActor())
				{
					if (FactionComp->GetAggro(InstigatorActor) < FactionComp->GetAggro(GetTargetActor()))
					{
						SetTargetActor(InstigatorActor);
					}
				}

				else
				{
					SetTargetActor(InstigatorActor);
				}	
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
			if (Creditor != nullptr)
			{
				ASPlayerState* InstigatorState = Creditor->GetPlayerState<ASPlayerState>(); 
				
				if (InstigatorState != nullptr)
				{
					InstigatorState->ApplyCreditsChange(CreditValue);
				}
			}
			

			// Stop BT
			AAIController* AIC = Cast<AAIController>(GetController());
			if (AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Killed");
			}
		
			// Ragdoll
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
		AIC->GetBlackboardComponent()->SetValueAsObject(TargetActorKey, NewTarget);
	}
}

AActor* ASAICharacter::GetTargetActor() const
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		return Cast<AActor>(AIC->GetBlackboardComponent()->GetValueAsObject(TargetActorKey));
	}

	return nullptr;
}

void ASAICharacter::OnPawnSeen(APawn* Pawn)
{	
	USAffinityFactionComponent* OtherFactionComponent = USAffinityFactionComponent::GetFactionComponents(Pawn);
	// Ignore if target already set
	if (GetTargetActor() != Pawn && FactionComp->IsEnemy(Pawn))
	{
		DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 0.5f, true);
		
		AActor* PrevTargetActor = GetTargetActor();
		
		SetTargetActor(Pawn);
		if (FactionComp->GetAggro(Pawn) >= FactionComp->GetAggro(GetTargetActor()))
		{
			SetTargetActor(PrevTargetActor);
		}

		

		USWorldUserWidget* NewWidget = CreateWidget<USWorldUserWidget>(GetWorld(), SpottedWidgetClass);
		if (NewWidget)
		{
			NewWidget->AttachedActor = this;
			// Index of 10 (or anything higher than default of 0) places this on top of any other widget.
			// May end up behind the minion health bar otherwise.
			NewWidget->AddToViewport(10);
		}
	}
	
	DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::Red, 0.5f, true);
}