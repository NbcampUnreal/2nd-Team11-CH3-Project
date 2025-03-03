// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Attachment/DamageParts.h"
#include "Item/GunBase.h"

void UDamageParts::ApplyAttachmentEffect(UGunBase* InGun)
{
	InGun->Damage += EffectAmount;
}

void UDamageParts::RemoveAttachmentEffect(UGunBase* InGun)
{
	InGun->Damage -= EffectAmount;
}
