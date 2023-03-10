// Fill out your copyright notice in the Description page of Project Settings.


#include "FremenCharacter.h"

#include "VoiceProject/Items/BaseWeapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "VoiceProject/Components/CombatComponent.h"
#include "VoiceProject/Items/Interactable.h"
#include "VoiceProject/Utils/Logger.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/RagdollComponent.h"

// Sets default values
AFremenCharacter::AFremenCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	// Rotation is determent by the camera and not by the controller.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	AddOwnedComponent(CombatComponent);

	RagdollComponent = CreateDefaultSubobject<URagdollComponent>(TEXT("RagdollComponent"));
	AddOwnedComponent(RagdollComponent);
}

// Called when the game starts or when spawned
void AFremenCharacter::BeginPlay()
{
	Super::BeginPlay();		
	OnTakePointDamage.AddUniqueDynamic(this, &AFremenCharacter::OnReceivePointDamage);
	TrySpawnMainCharacter();
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
	PlayerInputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, &AFremenCharacter::Attack);
	PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &AFremenCharacter::Interact);
	PlayerInputComponent->BindAction(TEXT("Dodge"), IE_Pressed, this, &AFremenCharacter::Dodge);
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
	Logger::Log(ELogLevel::INFO, __FUNCTION__);
	if (bIsDodging)
	{
		return;
	}
	
	if (const ABaseWeapon* MainWeapon = CombatComponent->GetMainWeapon())
	{
		if (UAnimMontage* Montage = CombatComponent->IsCombatEnabled() ? MainWeapon->SheatheWeaponMontage : MainWeapon->DrawWeaponMontage)
		{
			Logger::Log(ELogLevel::INFO, FString::Printf(TEXT("play %s"), *Montage->GetName()));

			PlayAnimMontage(Montage);
		}
	}
}

void AFremenCharacter::Interact()
{
	Logger::Log(ELogLevel::INFO, __FUNCTION__);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
	ObjectTypesArray.Init(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1), 1);

	TArray<AActor*> OutArray;
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Init(this, 1);
	
	if (UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), 500.f, ObjectTypesArray, nullptr, IgnoreActors, OutArray))
	{
		if (IInteractable* Item = Cast<IInteractable>(OutArray[0]))
		{
			Item->Interact(this);
			Logger::Log(ELogLevel::INFO, FString::Printf(TEXT("Interact with %s"), *OutArray[0]->GetName()));
		}
	}
}

void AFremenCharacter::Dodge()
{
	Logger::Log(ELogLevel::INFO, __FUNCTION__);
	
	if (bIsDisabled || bIsDodging || (CombatComponent && CombatComponent->bIsAttacking))
	{
		return;
	}

	PerformDodge();
}

void AFremenCharacter::Attack()
{
	Logger::Log(ELogLevel::INFO, __FUNCTION__);
	if (bIsDisabled || bIsDodging || (CombatComponent && !CombatComponent->IsCombatEnabled()))
	{
		return;
	}
	
	if (CombatComponent->bIsAttacking)
	{
		CombatComponent->bIsAttackSaved = true;
	}
	else
	{
		PerformAttack(CombatComponent->AttackCount, false);
	}
}

void AFremenCharacter::AttackContinue()
{
	if (!CombatComponent)
	{
		return;
	}

	CombatComponent->bIsAttacking = false;
	if (CombatComponent->bIsAttackSaved)
	{
		CombatComponent->bIsAttackSaved = false;
		if (CombatComponent->IsCombatEnabled())
		{
			PerformAttack(CombatComponent->AttackCount);
		}
	}
}

void AFremenCharacter::ResetMovementState()
{
	CombatComponent->ResetCombat();
	bIsDodging = false;
	bIsDisabled = false;
}

FRotator AFremenCharacter::GetSignificantInputRotation(float Threshold)
{
	const FVector LastInput = GetLastMovementInputVector();
	if (LastInput.Length() >= Threshold)
	{
		return FRotationMatrix::MakeFromX(LastInput).Rotator();
	}

	return GetActorRotation();
}

void AFremenCharacter::PerformAttack(unsigned int AttackIndex, bool IsRandom)
{
	TArray<UAnimMontage*>& MontagesArray = CombatComponent->GetMainWeapon()->AttackMontages;
	int Index = IsRandom ? FMath::RandRange(0, MontagesArray.Num()) : AttackIndex;

	if (Index < MontagesArray.Num())
	{
		UAnimMontage* Montage = MontagesArray[Index];
		CombatComponent->bIsAttacking = true;
		CombatComponent->AttackCount ++;
		CombatComponent->AttackCount %= MontagesArray.Num();
		PlayAnimMontage(Montage);
	}
}

void AFremenCharacter::PerformDodge()
{
	bIsDodging = true;
	PlayAnimMontage(DodgeMontage);
}

void AFremenCharacter::PerformDeath()
{
	Logger::Log(ELogLevel::DEBUG, __FUNCTION__);

	bIsDead = true;
	RagdollComponent->EnableRagdoll();
	
	float InitialSpeed = 2000.f;
	FVector HitVelocity = GetActorForwardVector() * InitialSpeed * -1;
	GetMesh()->SetPhysicsLinearVelocity(HitVelocity, false, TEXT("Pelvis"));

	if (ABaseWeapon* MainWeapon = CombatComponent->GetMainWeapon())
	{
		MainWeapon->GetItemMesh()->SetCollisionProfileName(TEXT("PhysicsActor"), true);
		MainWeapon->GetItemMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		MainWeapon->GetItemMesh()->SetSimulatePhysics(true);
	}

	FTimerHandle DeathTimer;
	GetWorldTimerManager().SetTimer(DeathTimer, this, &AFremenCharacter::DestroyCharacter, 5.f, false);
}

void AFremenCharacter::DestroyCharacter()
{
	if (ABaseWeapon* MainWeapon = CombatComponent->GetMainWeapon())
	{
		MainWeapon->Destroy();
	}
	
	this->Destroy();
}

void AFremenCharacter::OnReceivePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy,
                                            FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection,
                                            const UDamageType* DamageType, AActor* DamageCauser)
{
	Logger::Log(ELogLevel::DEBUG, __FUNCTION__);

	UGameplayStatics::PlaySoundAtLocation(GetWorld(),HitCue, HitLocation);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodEmitter, HitLocation, ShotFromDirection.Rotation());
	PlayAnimMontage(HitMontage);
	bIsDisabled = true;
	Health -= Damage;

	if (Health <= 0)
	{
		PerformDeath();
	}
}

void AFremenCharacter::TrySpawnMainCharacter()
{
	if (WeaponClass != nullptr)
	{
		if (UWorld* World = GetWorld())
		{
			FActorSpawnParameters SpawnParameters;
			
			SpawnParameters.Owner = this;
			SpawnParameters.Instigator = this;

			if (ABaseWeapon* Weapon = World->SpawnActor<ABaseWeapon>(WeaponClass, GetActorTransform(), SpawnParameters))
			{
				CombatComponent->SetMainWeapon(Weapon);
			}
		}
	}
}

