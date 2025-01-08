// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;
//using EpicGames.Horde.Users;
//using Microsoft.CodeAnalysis.Diagnostics;

public class LastPotionerEditorTarget : TargetRules
{
	public LastPotionerEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_5;
        CppStandard = CppStandardVersion.Cpp20;



        ExtraModuleNames.AddRange( new string[] { "LastPotioner" } );
	}
}
