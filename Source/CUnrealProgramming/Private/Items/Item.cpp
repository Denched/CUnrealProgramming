// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "CUnrealProgramming/DebugMacro.h"


// Sets default values
AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMesh;
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
}

float AItem::TransformedSin()
{
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

float AItem::TransformedCosin()
{
	return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RunningTime += DeltaTime;
	FRotator Rotation = GetActorRotation();
	Rotation.Yaw += DeltaTime * 50.f;
	AddActorWorldRotation(Rotation);


}

