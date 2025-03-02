// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemBase.h"
#include "GunBase.generated.h"

class APlayerCharacter;
class UAttachmentBase;

UCLASS()
class SPARTADIVERS_API UGunBase : public UItemBase
{
	GENERATED_BODY()

//private:

public:
	UGunBase();

	APlayerCharacter* PlayerCharacter;
	FTimerHandle FireCooldownTimer;
	bool bHitHead;
	bool bCanFire;
	bool bOnInfiniteBullet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun|Stats")
	float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun|Stats")
	float FireRate;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun|Stats")
	//int32 Penetration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun|Stats")
	int32 CurAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun|Stats")
	int32 MaxAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun|Stats")
	float CurRecoil;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun|Stats")
	float RecoilGap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun|Stats")
	float MaxRecoil;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun|Stats")
	float ReloadTime;

	UPROPERTY()
	UAttachmentBase* FirstAttachment;
	UPROPERTY()
	UAttachmentBase* SecondAttachment;
	UPROPERTY()
	UAttachmentBase* ThirdAttachment;

	UFUNCTION(BlueprintCallable,Category = "Gun|Actions")
	virtual void Fire();
	UFUNCTION(BlueprintCallable,Category = "Gun|Actions")
	virtual void ResetFireCooldown();
	UFUNCTION(BlueprintCallable,Category = "Gun|Actions")
	virtual void Reload();
	UFUNCTION(BlueprintCallable,Category = "Gun|Actions")
	virtual void ApplyRecoil();

	FVector GetCameraLocation() const;
	FRotator GetCameraRotation() const;
	float GetSpringArmLength() const;

protected:
	virtual FVector GetFireStartLocation() const;
	virtual FVector GetFireEndLocation() const;

};
