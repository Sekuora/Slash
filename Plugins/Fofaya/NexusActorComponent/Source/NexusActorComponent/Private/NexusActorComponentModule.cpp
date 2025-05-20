// Copyright Epic Games, Inc. All Rights Reserved.

#include "NexusActorComponentModule.h"

#include "NexusActorDetailsPanel.h"
#include "NexusActorComponent.h"

#define LOCTEXT_NAMESPACE "NexusActorComponentModule"

void FNexusActorComponentModule::StartupModule()
{

	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module


	/*FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyEditorModule.RegisterCustomClassLayout("NexusActorComponent", FOnGetDetailCustomizationInstance::CreateStatic(&NexusActorDetailsPanel::MakeInstance));*/

	
}

void FNexusActorComponentModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.



	//if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	//{
	//	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	//	PropertyEditorModule.UnregisterCustomClassLayout("NexusActorComponent");
	//}

	
}

#undef LOCTEXT_NAMESPACE

	IMPLEMENT_MODULE(FNexusActorComponentModule, NexusActorComponentEditorMode)