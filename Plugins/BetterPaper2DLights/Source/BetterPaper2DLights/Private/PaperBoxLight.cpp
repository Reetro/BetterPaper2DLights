// Fill out your copyright notice in the Description page of Project Settings.


#include "PaperBoxLight.h"
#include "PaperBoxLightComponent.h"

APaperBoxLight::APaperBoxLight()
{
  BoxLightComponent = CreateDefaultSubobject<UPaperBoxLightComponent>(TEXT("BoxLight"));
  RootComponent = BoxLightComponent;
}
