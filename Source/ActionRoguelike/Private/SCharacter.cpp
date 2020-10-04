// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "SInteractionComponent.h"
#include "Camera/PlayerCameraManager.h"
#include <DrawDebugHelpers.h>
#include <Kismet/KismetMathLibrary.h>

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

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;

	// Random Jump Height
    float RandomVelocity = FMath::RandRange(400.0f, 1000.0f);
	GetCharacterMovement()->JumpZVelocity = RandomVelocity;
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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


void ASCharacter::StartJump()
{
	bPressedJump = true;
	
}

void ASCharacter::StopJump()
{
	bPressedJump = false;
}

FVector ASCharacter::ProjectileDirectionCalc() 
{
	//Get Camera Manager

	APlayerCameraManager* CurrentCamera = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	//Get Camera information for raycast & set up collision channel
	FHitResult Hit;
	ECollisionChannel TraceChannel;
	TraceChannel = ECC_Camera;
	
	FVector CamLoc = CurrentCamera->GetCameraLocation();
	FRotator CamRot = CurrentCamera->GetCameraRotation();
	//Raycast Length
	FVector End = CamLoc + (CamRot.Vector() * 10000);
	FVector HitEnd;
	//Raycast
	bool bBlockingHit = GetWorld()->LineTraceSingleByChannel(Hit, CamLoc, End, TraceChannel);
	//If hit get vector to hit location
	if (bBlockingHit) {
		HitEnd = Hit.Location;
	}
	//Else use raycast length
	else 
	{
		HitEnd = End;
	}
	//Debug Line
	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;
	DrawDebugLine(GetWorld(), CamLoc, End, LineColor, false, 10.0f, 0, 10.0f);
	return HitEnd;

	/*FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

	FRotator FinalRot = UKismetMathLibrary::FindLookAtRotation(HandLocation, HitEnd);

	FTransform SpawnTM = FTransform(FinalRot, HandLocation);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Instigator);*/
}


void ASCharacter::PrimaryAttack()
{
	
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, 0.18f);
	
	//GetWorldTimerManager().ClearTimer(TimerHandle_PrimaryAttack)
}



void ASCharacter::PrimaryAttack_TimeElapsed()
{	
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

	FVector HitEnd = ProjectileDirectionCalc();
	
	FRotator FinalRot = UKismetMathLibrary::FindLookAtRotation(HandLocation, HitEnd);
	
	FTransform SpawnTM = FTransform(FinalRot, HandLocation);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;

	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);

}

void ASCharacter::BlackHoleAbility() 
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_BlackHoleAbility, this, &ASCharacter::BlackHoleAbility_TimeElapsed, 0.18f);
}

void ASCharacter::BlackHoleAbility_TimeElapsed()
{
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

	FVector HitEnd = ProjectileDirectionCalc();

	FRotator FinalRot = UKismetMathLibrary::FindLookAtRotation(HandLocation, HitEnd);

	FTransform SpawnTM = FTransform(FinalRot, HandLocation);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;

	GetWorld()->SpawnActor<AActor>(BlackHoleProjectileClass, SpawnTM, SpawnParams);

}

void ASCharacter::Dash()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_Dash, this, &ASCharacter::Dash_TimeElapsed, 0.18f);
}

void ASCharacter::Dash_TimeElapsed()
{
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

	FVector HitEnd = ProjectileDirectionCalc();

	FRotator FinalRot = UKismetMathLibrary::FindLookAtRotation(HandLocation, HitEnd);

	FTransform SpawnTM = FTransform(FinalRot, HandLocation);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;

	GetWorld()->SpawnActor<AActor>(DashProjectileClass, SpawnTM, SpawnParams);

}

void ASCharacter::PrimaryInteract()
{
	if(InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
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
}

