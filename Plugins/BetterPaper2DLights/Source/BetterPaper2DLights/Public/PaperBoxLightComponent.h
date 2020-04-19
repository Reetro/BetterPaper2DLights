// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BetterPaper2DLights/Public/Paper2DLightBaseComponent.h"
#include "PaperBoxLightComponent.generated.h"

class UBoxComponent;

/**
 *  A light component which emits light from a single point equally in all directions but constrained to a 2D plane
 */
UCLASS(Blueprintable, ClassGroup = (Lights, Common), hidecategories = (Object, LightShafts), editinlinenew, meta = (BlueprintSpawnableComponent))
class BETTERPAPER2DLIGHTS_API UPaperBoxLightComponent : public UPaper2DLightBaseComponent
{
	GENERATED_BODY()

public:

  UPaperBoxLightComponent();

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
  UBoxComponent* BoxLightingBounds;

};
