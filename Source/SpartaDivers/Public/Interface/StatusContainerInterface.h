// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "StatusContainerInterface.generated.h"

class UStatusContainerComponent;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UStatusContainerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SPARTADIVERS_API IStatusContainerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual UStatusContainerComponent* GetStatusContainerComponent() const = 0;
};
