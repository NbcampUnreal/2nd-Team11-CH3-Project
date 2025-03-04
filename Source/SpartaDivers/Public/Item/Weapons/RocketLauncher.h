// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Item/GunBase.h"
#include "RocketLauncher.generated.h"

class APlayerProjectile;

UCLASS()
class SPARTADIVERS_API URocketLauncher : public UGunBase
{
	GENERATED_BODY()

public:
	URocketLauncher();

	virtual void Fire() override;
	virtual void LaunchProjectile();
	FRotator GetLaunchRotation();
	virtual void ResetFireCooldown() override;
	virtual void Reload() override;

	UPROPERTY(EditDefaultsOnly)
	float VelocityOfProjectile = 100.f;
	UPROPERTY(EditDefaultsOnly)
	FName FireSocketName;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APlayerProjectile> ProjectileClass;
};
