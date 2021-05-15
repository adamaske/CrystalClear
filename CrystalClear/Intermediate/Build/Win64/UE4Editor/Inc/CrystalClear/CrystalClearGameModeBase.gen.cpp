// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "CrystalClear/CrystalClearGameModeBase.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeCrystalClearGameModeBase() {}
// Cross Module References
	CRYSTALCLEAR_API UClass* Z_Construct_UClass_ACrystalClearGameModeBase_NoRegister();
	CRYSTALCLEAR_API UClass* Z_Construct_UClass_ACrystalClearGameModeBase();
	ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
	UPackage* Z_Construct_UPackage__Script_CrystalClear();
// End Cross Module References
	void ACrystalClearGameModeBase::StaticRegisterNativesACrystalClearGameModeBase()
	{
	}
	UClass* Z_Construct_UClass_ACrystalClearGameModeBase_NoRegister()
	{
		return ACrystalClearGameModeBase::StaticClass();
	}
	struct Z_Construct_UClass_ACrystalClearGameModeBase_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ACrystalClearGameModeBase_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AGameModeBase,
		(UObject* (*)())Z_Construct_UPackage__Script_CrystalClear,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACrystalClearGameModeBase_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering Utilities|Transformation" },
		{ "IncludePath", "CrystalClearGameModeBase.h" },
		{ "ModuleRelativePath", "CrystalClearGameModeBase.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_ACrystalClearGameModeBase_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ACrystalClearGameModeBase>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_ACrystalClearGameModeBase_Statics::ClassParams = {
		&ACrystalClearGameModeBase::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x009002ACu,
		METADATA_PARAMS(Z_Construct_UClass_ACrystalClearGameModeBase_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_ACrystalClearGameModeBase_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_ACrystalClearGameModeBase()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_ACrystalClearGameModeBase_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(ACrystalClearGameModeBase, 1531506444);
	template<> CRYSTALCLEAR_API UClass* StaticClass<ACrystalClearGameModeBase>()
	{
		return ACrystalClearGameModeBase::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_ACrystalClearGameModeBase(Z_Construct_UClass_ACrystalClearGameModeBase, &ACrystalClearGameModeBase::StaticClass, TEXT("/Script/CrystalClear"), TEXT("ACrystalClearGameModeBase"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(ACrystalClearGameModeBase);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
