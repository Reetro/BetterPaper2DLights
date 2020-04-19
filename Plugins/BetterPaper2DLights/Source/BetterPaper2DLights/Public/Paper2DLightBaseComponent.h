// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/LocalLightComponent.h"
#include "Paper2DLightBaseComponent.generated.h"

class FLightSceneProxy;

/**
 * The base class for all 2D lights
 */
UCLASS()
class BETTERPAPER2DLIGHTS_API UPaper2DLightBaseComponent : public ULocalLightComponent
{
	GENERATED_BODY()
	

public:

  /**
   * Whether to use physically based inverse squared distance falloff, where AttenuationRadius is only clamping the light's contribution.
   * Disabling inverse squared falloff can be useful when placing fill lights (don't want a super bright spot near the light).
   * When enabled, the light's Intensity is in units of lumens, where 1700 lumens is a 100W lightbulb.
   * When disabled, the light's Intensity is a brightness scale.
   */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Light, AdvancedDisplay)
  uint32 bUseInverseSquaredFalloff : 1;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Light)
  float SourceRadius;

  /**
  * Soft radius of light source shape.
  * Note that light sources shapes which intersect shadow casting geometry can cause shadowing artifacts.
  */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Light)
  float SoftSourceRadius;

  /**
 * Controls the radial falloff of the light when UseInverseSquaredFalloff is disabled.
 * 2 is almost linear and very unrealistic and around 8 it looks reasonable.
 * With large exponents, the light has contribution to only a small area of its influence radius but still costs the same as low exponents.
 */
  UPROPERTY(interp, BlueprintReadOnly, Category = Light, AdvancedDisplay, meta = (UIMin = "2.0", UIMax = "16.0"))
  float LightFalloffExponent;

  /**
 * Length of light source shape.
 * Note that light sources shapes which intersect shadow casting geometry can cause shadowing artifacts.
 */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Light)
  float SourceLength;

  virtual float ComputeLightBrightness() const override;
#if WITH_EDITOR
  virtual void SetLightBrightness(float InBrightness) override;
#endif

  //~ Begin ULightComponent Interface.
  virtual ELightComponentType GetLightType() const override;
  virtual float GetUniformPenumbraSize() const override;
  // TODO make custom scene proxy virtual FLightSceneProxy* CreateSceneProxy() const override;

  //~ Begin UObject Interface
  virtual void Serialize(FArchive& Ar) override;
#if WITH_EDITOR
  virtual bool CanEditChange(const UProperty* InProperty) const override;
  virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR
  //~ End UObject Interface

  /**
   * This is called when property is modified by InterpPropertyTracks
   *
   * @param PropertyThatChanged	Property that changed
   */
  virtual void PostInterpChange(UProperty* PropertyThatChanged) override;

};
