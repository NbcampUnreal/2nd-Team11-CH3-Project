// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/AnimNotifyState_JumpToPlayer.h"
#include "Kismet/GameplayStatics.h"

void UAnimNotifyState_JumpToPlayer::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(MeshComp->GetWorld(), 0);
    if (PlayerPawn)
    {
        TargetLocation = PlayerPawn->GetActorLocation() - MeshComp->GetOwner()->GetActorForwardVector() * Radius;
        float Distance = FVector::Distance(TargetLocation, MeshComp->GetOwner()->GetActorLocation());
        Velocity = Distance / TotalDuration;
    }
}

void UAnimNotifyState_JumpToPlayer::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
    AActor* Owner = MeshComp->GetOwner();
    if (Owner)
    {
        FVector Direction = (TargetLocation - Owner->GetActorLocation()).GetSafeNormal();
        FVector NewLocation = Owner->GetActorLocation() + Direction * Velocity * FrameDeltaTime;

        Owner->SetActorLocation(NewLocation);
    }
}
