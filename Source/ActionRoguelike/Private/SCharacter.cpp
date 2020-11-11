// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "SInteractionComponent.h"
#include "Camera/PlayerCameraManager.h"
#include <DrawDebugHelpers.h>
#include <Kismet/KismetMathLibrary.h>
#include "SAttributeComponent.h"
#include <GameFramework/PlayerController.h>
#include <GameFramework/Character.h>
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SActionComponent.h"


// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;

	// Random Jump Height
    float RandomVelocity = FMath::RandRange(400.0f, 1000.0f);
	GetCharacterMovement()->JumpZVelocity = RandomVelocity;

	TimeToHitParamName = "TimeToHit";

}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChange);
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

FVector ASCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
}

void ASCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;
	
	AddMovementInput(ControlRot.Vector(), Value);
}

void ASCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	// X = Forward (Red)
	// Y = Right (Green)
	// Z = Up (Blue)

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, Value);
}


void ASCharacter::SprintStart()
{
	ActionComp->StartActionByName(this, "Sprint");
}


void ASCharacter::SprintStop()
{
	ActionComp->StopActionByName(this, "Sprint");
}

// void ASCharacter::StartJump()
// {
// 	bPressedJump = true;
// }
// 
// void ASCharacter::StopJump()
// {
// 	bPressedJump = false;
// }

// void ASCharacter::ProjectileDirectionCalc(TSubclassOf<AActor>Projectile) 
// {
// 	//Get Camera Manager
// 	APlayerCameraManager* CurrentCamera = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
// 	//Get Camera information for raycast & set up collision channel
// 	FHitResult Hit;
// 	
// 	FVector CamLoc = CurrentCamera->GetCameraLocation();
// 	FRotator CamRot = CurrentCamera->GetCameraRotation();
// 	
// 	//Raycast Length
// 	FVector End = CamLoc + (CamRot.Vector() * 10000);
// 	FVector HitEnd;
// 	
// 	FCollisionShape Shape;
// 	Shape.SetSphere(10.0f);
// 
// 	//Raycast must ignore actor
// 	FCollisionQueryParams Params;
// 	Params.AddIgnoredActor(this);
// 
// 	FCollisionObjectQueryParams ObjParams;
// 	ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
// 	ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
// 	ObjParams.AddObjectTypesToQuery(ECC_Pawn);
// 
// 	bool bBlockingHit = GetWorld()->SweepSingleByObjectType(Hit, CamLoc, End, FQuat::Identity, ObjParams, Shape, Params);
// 	
// 	//If hit get vector to hit location
// 	if (bBlockingHit) 
// 	{
// 		End = Hit.Location;
// 	}
// 
// 	//Set up Spawn Location and Rotation for Projectile
// 	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
// 	FRotator FinalRot = UKismetMathLibrary::FindLookAtRotation(HandLocation, End);
// 	FTransform SpawnTM = FTransform(FinalRot, HandLocation);
// 	
// 	//Spawn Projectile
// 	FActorSpawnParameters SpawnParams;
// 	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
// 	SpawnParams.Instigator = this;
// 
// 	GetWorld()->SpawnActor<AActor>(Projectile, SpawnTM, SpawnParams);
// 
// }


void ASCharacter::PrimaryAttack()
{
	ActionComp->StartActionByName(this, "PrimaryAttack");
}


void ASCharacter::BlackHoleAbility() 
{
	ActionComp->StartActionByName(this, "BlackHoleAbility");
}


void ASCharacter::Dash()
{
	ActionComp->StartActionByName(this, "Dash");
}


void ASCharacter::PrimaryInteract()
{
	if(InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}

void ASCharacter::OnHealthChange(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
		//FVector InputColor = 1,1,1;
		//MeshComp->SetVectorParameterValueOnMaterials("Color", );
		
		// Rage added equal to damage received (Abs to turn into positive rage number)
		float RageDelta = FMath::Abs(Delta);
		AttributeComp->ApplyRage(InstigatorActor, RageDelta);
	}

	if (NewHealth <= 0.0f && Delta < 0.0f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);
		GetWorldTimerManager().SetTimer(TimerHandle_Death, this, &ASCharacter::Death_TimeElapsed, 2.85f);
	}
}

void ASCharacter::Death_TimeElapsed()
{
	//GetMesh()->SetSimulatePhysics(true);
	//GetMesh()->SetCollisionProfileName("Ragdoll");
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//GetCharacterMovement()->DisableMovement();

}



/*void ASCharacter::Sprint(*/

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Player movement input controls
	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	// Player camera controls
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Pitch", this, &APawn::AddControllerPitchInput);

	// Jump Input Movement
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::Jump);

	// Player Primary
	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);
	// Player Ability
	PlayerInputComponent->BindAction("BlackHoleAbility", IE_Pressed, this, &ASCharacter::BlackHoleAbility);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ASCharacter::Dash);
	//Sprint Input Component
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASCharacter::SprintStop);
}

void ASCharacter::HealSelf(float Amount /* = 100 */)
{
	AttributeComp->ApplyHealthChange(this, Amount);
}

