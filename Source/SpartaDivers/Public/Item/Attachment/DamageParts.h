// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/AttachmentBase.h"
#include "DamageParts.generated.h"

/**
 * 
 */
UCLASS()
class SPARTADIVERS_API UDamageParts : public UAttachmentBase
{
	GENERATED_BODY()

public:
	virtual void ApplyAttachmentEffect(UGunBase* InGun) override;
	virtual void RemoveAttachmentEffect(UGunBase* InGun) override;
};
