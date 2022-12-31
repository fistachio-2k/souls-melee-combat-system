// Fill out your copyright notice in the Description page of Project Settings.


#include "FremenCharacter.h"
#include "VoiceProject/Items/BaseWeapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "VoiceProject/Items/Interactable.h"

// Sets default values
AFremenCharacter::AFremenCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Rotation is determent by the camera and not by the controller.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

}

// Called when the game starts or when spawned
void AFremenCharacter::BeginPlay()
{
	Super::BeginPlay();		
	
	// Spawn main weapon.
	if (WeaponClass != nullptr)
	{
		if (UWorld* World = GetWorld())
		{
			FActorSpawnParameters SpawnParameters;
			
			SpawnParameters.Owner = this;
			SpawnParameters.Instigator = this;

			if (ABaseWeapon* Weapon = World->SpawnActor<ABaseWeapon>(WeaponClass, GetActorTransform(), SpawnParameters))
			{
				SetMainWeapon(Weapon);
			}
		}
	}
}

// Called every frame
void AFremenCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFremenCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AFremenCharacter::MoveForward);
    PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AFremenCharacter::MoveRight);
    PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AFremenCharacter::LookUp);
    PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &AFremenCharacter::LookRight);

	PlayerInputComponent->BindAction(TEXT("ToggleWeapon"), IE_Pressed, this, &AFremenCharacter::ToggleWeapon);
	PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &AFremenCharacter::Interact);
}

ABaseWeapon& AFremenCharacter::GetMainWeapon() const
{
	return *MainWeapon;
}

void AFremenCharacter::SetMainWeapon(ABaseWeapon* Weapon)
{
	if (Weapon)
	{
		if (MainWeapon)
		{
			MainWeapon->OnUnequipped();
		}
		
		MainWeapon = Weapon;
		Weapon->SetOwner(this);
		MainWeapon->OnEquipped();
	}
}

void AFremenCharacter::MoveForward(float AxisValue)
{
	if ((Controller != nullptr) && (AxisValue != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, AxisValue);
	}
}

void AFremenCharacter::MoveRight(float AxisValue)
{
	if ( (Controller != nullptr) && (AxisValue != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, AxisValue);
	}
}

void AFremenCharacter::LookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AFremenCharacter::LookRight(float AxisValue)
{
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AFremenCharacter::ToggleWeapon()
{
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Action: ToggleWeapon")));
	
	if (MainWeapon)
	{
		if (UAnimMontage* Montage = MainWeapon->IsWeaponInHande() ? SheatheWeaponMontage : DrawWeaponMontage)
		{
			PlayAnimMontage(Montage);
		}
	}
}

void AFremenCharacter::Interact()
{
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Action: Interact")));

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
	ObjectTypesArray.Init(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1), 1);

	TArray<AActor*> OutArray;
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Init(MainWeapon, 1);
	
	if (UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), 500.f, ObjectTypesArray, nullptr, IgnoreActors, OutArray))
	{
		if (IInteractable* Item = Cast<IInteractable>(OutArray[0]))
		{
			Item->Interact(this);
			if(GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Interact with %s"), *OutArray[0]->GetName()));
		}
	}
}

