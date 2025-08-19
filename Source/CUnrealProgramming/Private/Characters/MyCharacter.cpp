// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MyCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "Camera/CameraComponent.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

		if (Subsystem)
		{
			Subsystem->AddMappingContext(CharacterContext, 0);

		}
	}


}

void AMyCharacter::Move(const FInputActionValue& Value)
{
	if (ActionState != EActionState::EAS_Unoccupied) return;
	const FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementVector.Y);

	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.X);

	if (MovementVector.X < -0.1f) // Moving left
	{
		MovingLeft = true;
		MovingRight = false;
	}
	else if (MovementVector.X > 0.1f) // Moving right
	{
		MovingRight = true;
		MovingLeft = false;
	}
	else // Moving forward (or stopping)
	{
		MovingLeft = false;
		MovingRight = false;
	}

	FVector DesiredMovementDirection = (ForwardDirection * MovementVector.Y) + (RightDirection * MovementVector.X);

	if (!DesiredMovementDirection.IsNearlyZero())
	{
		FRotator DesiredRotation = DesiredMovementDirection.Rotation();
		SetActorRotation(FRotator(0.f, DesiredRotation.Yaw, 0.f));
	}
}

void AMyCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerPitchInput(LookAxisVector.Y);
	AddControllerYawInput(LookAxisVector.X);

}

void AMyCharacter::Equip(const FInputActionValue& Value)
{
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);

	if (OverlappingItem)
	{
		OverlappingWeapon->EquipWeapon(GetMesh(), FName("Fox_-HeadSocket"));
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
		OverlappingItem = nullptr;
		EquipWeapon = OverlappingWeapon;
	}
	else
	{
		if (canDisarm())
		{
			PlayEquipMontage(FName("Unequip"));
			CharacterState = ECharacterState::ECS_Unequipped;
			ActionState = EActionState::EAS_EquippingWeapon;
		}

		else if (canArm())
		{
			PlayEquipMontage(FName("Equip"));
			CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
			ActionState = EActionState::EAS_EquippingWeapon;
		}
	}
}

void AMyCharacter::Attack(const FInputActionValue& Value)
{
	
	if (canAttack())
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
}

bool AMyCharacter::canAttack()
{
	return ActionState == EActionState::EAS_Unoccupied && CharacterState != ECharacterState::ECS_Unequipped;
}



bool AMyCharacter::canDisarm()
{
	return ActionState == EActionState::EAS_Unoccupied && CharacterState != ECharacterState::ECS_Unequipped;
}

bool AMyCharacter::canArm()
{
	return ActionState == EActionState::EAS_Unoccupied && CharacterState == ECharacterState::ECS_Unequipped && EquipWeapon;
}

void AMyCharacter::Disarm()
{
	if (EquipWeapon)
	{
		EquipWeapon->AttachMeshToSocket(GetMesh(), FName("Fox_-Spine1Socket"));
	}
}

void AMyCharacter::Arm()
{
	if (EquipWeapon)
	{
		EquipWeapon->AttachMeshToSocket(GetMesh(), FName("Fox_-HeadSocket"));
	}
}

void AMyCharacter::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void AMyCharacter::PlayEquipMontage(FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}

}
void AMyCharacter::PlayAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
		const int32 Selection = FMath::RandRange(0, 1);
		FName SectionName = FName();
		switch (Selection)
		{
		case 0:

			SectionName = FName("Attack1");
			break;

		case 1:

			SectionName = FName("Attack2");
			break;
		default:
			break;
		}
		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
	}
}

void AMyCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &AMyCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMyCharacter::Jump);
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Triggered, this, &AMyCharacter::Equip);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AMyCharacter::Attack);

	}
}

void AMyCharacter::Jump()
{

    Super::Jump();

}

