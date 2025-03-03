// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Attachment/RecoilParts.h"
#include "Item/GunBase.h"

void URecoilParts::ApplyAttachmentEffect(UGunBase* InGun)
{
	InGun->MaxRecoil += EffectAmount;
}

void URecoilParts::RemoveAttachmentEffect(UGunBase* InGun)
{
	InGun->MaxRecoil -= EffectAmount;
}
