// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Paper2DLightsGameMode.h"
#include "Paper2DLightsCharacter.h"

APaper2DLightsGameMode::APaper2DLightsGameMode()
{
	// Set default pawn class to our character
	DefaultPawnClass = APaper2DLightsCharacter::StaticClass();	
}
