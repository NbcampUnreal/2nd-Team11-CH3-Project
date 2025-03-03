// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Attachment/FireRateParts.h"
#include "Item/GunBase.h"

void UFireRateParts::ApplyAttachmentEffect(UGunBase* InGun)
{
	InGun->FireRate += EffectAmount;
}

void UFireRateParts::RemoveAttachmentEffect(UGunBase* InGun)
{
	InGun->FireRate -= EffectAmount;
}
