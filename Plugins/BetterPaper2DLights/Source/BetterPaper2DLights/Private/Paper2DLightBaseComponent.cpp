// Fill out your copyright notice in the Description page of Project Settings.


#include "Paper2DLightBaseComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "RenderingThread.h"
#include "Engine/Texture2D.h"
#include "SceneManagement.h"

int32 GAllow2DLightCubemapShadows = 1;
static FAutoConsoleVariableRef CVarAllow2DLightCubemapShadows(
  TEXT("r.AllowPointLightCubemapShadows"),
  GAllow2DLightCubemapShadows,
  TEXT("When 0, will prevent 2D light cube map shadows from being used and the light will be unshadowed.")
);

#if WITH_EDITOR
void UPaper2DLightBaseComponent::SetLightBrightness(float InBrightness)
{
  if (bUseInverseSquaredFalloff)
  {
    if (IntensityUnits == ELightUnits::Candelas)
    {
      Super::SetLightBrightness(InBrightness / (100.f * 100.f)); // Conversion from cm2 to m2
    }
    else if (IntensityUnits == ELightUnits::Lumens)
    {
      Super::SetLightBrightness(InBrightness / (100.f * 100.f / 4 / PI)); // Conversion from cm2 to m2 and 4PI from the sphere area in the 1/r2 attenuation
    }
    else
    {
      Super::SetLightBrightness(InBrightness / 16); // Legacy scale of 16
    }
  }
  else
  {
    Super::SetLightBrightness(InBrightness);
  }
}
#endif // WITH_EDITOR

/**
* @return ELightComponentType for the light component class
*/
ELightComponentType UPaper2DLightBaseComponent::GetLightType() const
{
  return LightType_Point;
}

float UPaper2DLightBaseComponent::GetUniformPenumbraSize() const
{
  if (LightmassSettings.bUseAreaShadowsForStationaryLight)
  {
    // Interpret distance as shadow factor directly
    return 1.0f;
  }
  else
  {
    // Heuristic to derive uniform penumbra size from light source radius
    return FMath::Clamp(SourceRadius == 0 ? .05f : SourceRadius * .005f, .0001f, 1.0f);
  }
}

static bool Is2DLightSupported(const UPaper2DLightBaseComponent* InLight)
{
  if (GMaxRHIFeatureLevel <= ERHIFeatureLevel::ES3_1 && InLight->IsMovable())
  {
    // if project does not support dynamic point lights on mobile do not add them to the renderer 
    static auto* CVarPointLights = IConsoleManager::Get().FindTConsoleVariableDataInt(TEXT("r.MobileNumDynamicPointLights"));
    const bool bPointLights = CVarPointLights->GetValueOnAnyThread() > 0;
    return bPointLights;
  }
  return true;
}

void UPaper2DLightBaseComponent::Serialize(FArchive& Ar)
{
  Super::Serialize(Ar);

  if (Ar.UE4Ver() < VER_UE4_INVERSE_SQUARED_LIGHTS_DEFAULT)
  {
    bUseInverseSquaredFalloff = InverseSquaredFalloff_DEPRECATED;
  }
  // Reorient old light tubes that didn't use an IES profile
  else if (Ar.UE4Ver() < VER_UE4_POINTLIGHT_SOURCE_ORIENTATION && SourceLength > KINDA_SMALL_NUMBER&& IESTexture == nullptr)
  {
    AddLocalRotation(FRotator(-90.f, 0.f, 0.f));
  }

  if (Ar.IsLoading() && !bUseInverseSquaredFalloff)
  {
    IntensityUnits = ELightUnits::Unitless;
  }
}

#if WITH_EDITOR

bool UPaper2DLightBaseComponent::CanEditChange(const UProperty* InProperty) const
{
  if (InProperty)
  {
    FString PropertyName = InProperty->GetName();
    if (PropertyName == GET_MEMBER_NAME_STRING_CHECKED(UPaper2DLightBaseComponent, LightFalloffExponent))
    {
      return !bUseInverseSquaredFalloff;
    }
    if (PropertyName == GET_MEMBER_NAME_STRING_CHECKED(ULocalLightComponent, IntensityUnits))
    {
      return bUseInverseSquaredFalloff;
    }
  }

  return Super::CanEditChange(InProperty);
}

/**
 * Called after property has changed via e.g. property window or set command.
 *
 * @param	PropertyThatChanged	UProperty that has been changed, NULL if unknown
 */
void UPaper2DLightBaseComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
  // Make sure exponent is > 0.
  LightFalloffExponent = FMath::Max((float)KINDA_SMALL_NUMBER, LightFalloffExponent);
  SourceRadius = FMath::Max(0.0f, SourceRadius);
  SoftSourceRadius = FMath::Max(0.0f, SoftSourceRadius);
  SourceLength = FMath::Max(0.0f, SourceLength);

  if (!bUseInverseSquaredFalloff)
  {
    IntensityUnits = ELightUnits::Unitless;
  }

  Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif // WITH_EDITOR

void UPaper2DLightBaseComponent::PostInterpChange(UProperty* PropertyThatChanged)
{
  static FName LightFalloffExponentName(TEXT("LightFalloffExponent"));
  FName PropertyName = PropertyThatChanged->GetFName();

  if (PropertyName == LightFalloffExponentName)
  {
    MarkRenderStateDirty();
  }
  else
  {
    Super::PostInterpChange(PropertyThatChanged);
  }
}

float UPaper2DLightBaseComponent::ComputeLightBrightness() const
{
  float LightBrightness = Super::ComputeLightBrightness();

  if (bUseInverseSquaredFalloff)
  {
    if (IntensityUnits == ELightUnits::Candelas)
    {
      LightBrightness *= (100.f * 100.f); // Conversion from cm2 to m2
    }
    else if (IntensityUnits == ELightUnits::Lumens)
    {
      LightBrightness *= (100.f * 100.f / 4 / PI); // Conversion from cm2 to m2 and 4PI from the sphere area in the 1/r2 attenuation
    }
    else
    {
      LightBrightness *= 16; // Legacy scale of 16
    }
  }
  return LightBrightness;
}

