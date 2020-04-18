// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Engine/Light.h"
#include "PaperBoxLight.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Lights, PointLights), ComponentWrapperClass, meta = (ChildCanTick))
class BETTERPAPER2DLIGHTS_API APaperBoxLight : public ALight
{
	GENERATED_BODY()

public:

  APaperBoxLight();

  UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Light", meta = (ExposeFunctionCategories = "PointLight,Rendering|Lighting"))
  class UPaper2DLightBaseComponent* BoxLightComponent;

};
