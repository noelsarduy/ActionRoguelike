// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_Regenerate.h"
#include "SAttributeComponent.h"
#include "AIController.h"
#include "TimerManager.h"


EBTNodeResult::Type USBTTaskRegenerate::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* MyPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (MyPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(MyPawn);
	if (ensure(AttributeComp))
	{
		AttributeComp->ApplyHealthChange(MyPawn, 10.0f);
		//float HealthGain = 10.0f;
		//float RegenDelaay = 1.0f;
			
		/*FTimerHandle HealingTimer;
		while ((AttributeComp->GetHealth()) < AttributeComp->GetHealthMax())
			{
				MyPawn->GetWorldTimerManager().SetTimer(HealingTimer, this, (AttributeComp->ApplyHealthChange(MyPawn, HealthGain)), RegenDelaay);
				MyPawn->GetWorldTimerManager().ClearTimer(HealingTimer);
			}*/

	}

	return EBTNodeResult::Succeeded;
}
