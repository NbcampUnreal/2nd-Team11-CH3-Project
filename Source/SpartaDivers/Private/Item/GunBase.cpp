// Copyright Epic Games, Inc. All Rights Reserved.

#include "Item/GunBase.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/SpringArmComponent.h"
#include "Item/AttachmentBase.h"


UGunBase::UGunBase()
{
    ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (Character)
    {
        PlayerCharacter = Cast<APlayerCharacter>(Character);
    }

    Damage = 0.0f;
    FireRate = 0.0f;
    MaxAmmo = 0;
    CurAmmo = MaxAmmo;
    ReloadTime = 0.0f;

    CurRecoil = 0.0f;
    RecoilGap = 0.0f;
    MaxRecoil = 0.0f;

    bHitHead = false;
    bCanFire = true;
    bOnInfiniteBullet = false;
}

void UGunBase::Fire()
{
    if (!bOnInfiniteBullet && bCanFire && CurAmmo > 0)
    {
        CurAmmo--;
        bCanFire = false;

        GetWorld()->GetTimerManager().SetTimer(
            FireCooldownTimer,
            this,
            &UGunBase::ResetFireCooldown,
            FireRate,
            false);

        UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, PlayerCharacter->GetActorLocation());
    }
}

void UGunBase::ResetFireCooldown()
{
    bCanFire = true;
}

void UGunBase::Reload()
{
    CurAmmo = MaxAmmo;
    bCanFire = true;
}

void UGunBase::ApplyRecoil()
{
    FRotator GunRecoil = FRotator(-CurRecoil, 0.f, 0.f);
    PlayerCharacter->AddControllerPitchInput(GunRecoil.Pitch);
}

FVector UGunBase::GetCameraLocation() const
{
    FVector CameraLocation;
    FRotator CameraRotation;
    PlayerCharacter->GetController()->GetPlayerViewPoint(CameraLocation, CameraRotation);
    return CameraLocation;
}

FRotator UGunBase::GetCameraRotation() const
{
    FVector CameraLocation;
    FRotator CameraRotation;
    PlayerCharacter->GetController()->GetPlayerViewPoint(CameraLocation, CameraRotation);
    return CameraRotation;
}

float UGunBase::GetSpringArmLength() const
{
    USpringArmComponent* SpringArm = PlayerCharacter->FindComponentByClass<USpringArmComponent>();
    return SpringArm ? SpringArm->TargetArmLength : 300.0f; // Default Length
}

void UGunBase::SetAttachment(int32 InAttachmentIndex, UAttachmentBase* InNewAttachment)
{
    switch (InAttachmentIndex)
    {
    case 0:
        if (InNewAttachment == nullptr)
        {
            if (FirstAttachment != nullptr)
            {
                FirstAttachment->RemoveAttachmentEffect(this);
                FirstAttachment = nullptr;
            }
        }
        else
        {
            if (FirstAttachment == nullptr)
            {
                InNewAttachment->ApplyAttachmentEffect(this);
                FirstAttachment = InNewAttachment;
            }
        }
        break;
    case 1:
        if (InNewAttachment == nullptr)
        {
            if (SecondAttachment != nullptr)
            {
                SecondAttachment->RemoveAttachmentEffect(this);
                SecondAttachment = nullptr;
            }
        }
        else
        {
            if (SecondAttachment == nullptr)
            {
                InNewAttachment->ApplyAttachmentEffect(this);
                SecondAttachment = InNewAttachment;
            }
        }
        break;
    case 2:
        if (InNewAttachment == nullptr)
        {
            if (ThirdAttachment != nullptr)
            {
                ThirdAttachment->RemoveAttachmentEffect(this);
                ThirdAttachment = nullptr;
            }
        }
        else
        {
            if (ThirdAttachment == nullptr)
            {
                InNewAttachment->ApplyAttachmentEffect(this);
                ThirdAttachment = InNewAttachment;
            }
        }
        break;
    default:
        break;
    }
}

USoundBase* UGunBase::GetReloadSound()
{
    return ReloadSound;
}

FVector UGunBase::GetFireStartLocation() const
{
    FVector CameraLocation = GetCameraLocation();
    FRotator CameraRotation = GetCameraRotation();
    float SpringArmLength = GetSpringArmLength();
    FVector ForwardVector = CameraRotation.Vector();
    FVector MuzzleOffset = ForwardVector * SpringArmLength;

    return CameraLocation + MuzzleOffset;
}

FVector UGunBase::GetFireEndLocation() const
{
    FVector CameraLocation = GetCameraLocation();
    FRotator CameraRotation = GetCameraRotation();

    float SpringArmLength = GetSpringArmLength();
    FVector ForwardVector = CameraRotation.Vector();
    FVector MuzzleOffset = ForwardVector * SpringArmLength;

    FVector FireStartLocation = GetFireStartLocation();
    // Adjust the end location by a more realistic distance
    return FireStartLocation + ForwardVector * 10000.0f;  
}
