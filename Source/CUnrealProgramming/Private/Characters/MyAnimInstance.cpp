// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MyAnimInstance.h"
#include "Characters/MyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
void UMyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	MyCharacter = Cast<AMyCharacter>(TryGetPawnOwner());

	if (MyCharacter)
	{
		CharacterMovement = MyCharacter->GetCharacterMovement();
	}
}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (CharacterMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(CharacterMovement->Velocity);
		IsFalling = CharacterMovement->IsFalling();
		CharacterState = MyCharacter->GetCharacterState();
	}

}
