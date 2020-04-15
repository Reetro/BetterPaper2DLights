// Fill out your copyright notice in the Description page of Project Settings.


#include "PaperBoxLightComponent.h"
#include "Components/BoxComponent.h"


UPaperBoxLightComponent::UPaperBoxLightComponent()
{
  BoxLightingBounds = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxLightingBounds"));
  BoxLightingBounds->SetupAttachment(GetAttachmentRoot());

  BoxLightingBounds->SetBoxExtent(FVector(32.0f, 0, 32.0f));
}
