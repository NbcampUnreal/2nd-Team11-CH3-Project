// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Attachment/AmmoParts.h"
#include "Item/GunBase.h"

void UAmmoParts::ApplyAttachmentEffect(UGunBase* InGun)
{
	InGun->MaxAmmo += EffectAmount;
}

void UAmmoParts::RemoveAttachmentEffect(UGunBase* InGun)
{
	InGun->MaxAmmo -= EffectAmount;
}
