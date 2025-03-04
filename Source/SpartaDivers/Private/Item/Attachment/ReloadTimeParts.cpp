// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Attachment/ReloadTimeParts.h"
#include "Item/GunBase.h"

void UReloadTimeParts::ApplyAttachmentEffect(UGunBase* InGun)
{
	InGun->ReloadTime += EffectAmount;
}

void UReloadTimeParts::RemoveAttachmentEffect(UGunBase* InGun)
{
	InGun->ReloadTime -= EffectAmount;
}
