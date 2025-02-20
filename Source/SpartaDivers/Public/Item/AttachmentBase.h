// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemBase.h"
#include "AttachmentBase.generated.h"

class UGunBase;

/**
 * 
 */
UCLASS()
class SPARTADIVERS_API UAttachmentBase : public UItemBase
{
	GENERATED_BODY()
	
public:
	UAttachmentBase();

	virtual void ApplyAttachmentEffect(UGunBase* InGun);
};
