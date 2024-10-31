// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

class USoundBase;
/**
 * 
 */
UCLASS()
class CUNREALPROGRAMMING_API AWeapon : public AItem
{
	GENERATED_BODY()
public:
	void EquipWeapon(USceneComponent* InParent, FName InSocketName);

	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);
	
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

private:

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* EquipSound;
};
