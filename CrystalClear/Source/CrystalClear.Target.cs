// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class CrystalClearTarget : TargetRules
{
	public CrystalClearTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "CrystalClear" } );
		WindowsPlatform.PCHMemoryAllocationFactor = 2000;
	}
}
