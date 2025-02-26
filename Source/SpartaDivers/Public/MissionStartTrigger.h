// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MissionStartTrigger.generated.h"

class UBoxComponent;
class AMissionManager;
UCLASS()
class SPARTADIVERS_API AMissionStartTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	AMissionStartTrigger();

    UFUNCTION()
    void ActivateTrigger();

    UFUNCTION(BlueprintCallable)
    void DeactivateTrigger();

    UFUNCTION()
    void OnInteracted();
    
    UFUNCTION(BlueprintImplementableEvent)
    void OnInteracted_BP();


protected:
    virtual void BeginPlay() override;

private:
    bool bIsActive;

    FTimerHandle MissionStartTimerHandle;

    UPROPERTY(EditAnywhere)
    AMissionManager* MissionManager;

    UPROPERTY(VisibleAnywhere)
    class UBoxComponent* TriggerBox;

    UPROPERTY(VisibleAnywhere)
    class UUserWidget* InteractionWidget;

    UFUNCTION()
    void OnOverlapBegin(
        UPrimitiveComponent* OverlappedComponent, 
        AActor* OtherActor, 
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex, 
        bool bFromSweep,
        const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp, 
        int32 OtherBodyIndex);
};
