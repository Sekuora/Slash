// Fill out your copyright notice in the Description page of Project Settings.


#include "NexusActorDetailsPanel.h"
//#include "DetailLayoutBuilder.h"
//#include "DetailCategoryBuilder.h"
//#include "IDetailCustomization.h"
//#include "PropertyHandle.h"
//#include "IDetailGroup.h"
//#include "NexusActorComponent.h"

NexusActorDetailsPanel::NexusActorDetailsPanel()
{
}

NexusActorDetailsPanel::~NexusActorDetailsPanel()
{
}


//TSharedRef<IDetailCustomization> NexusActorDetailsPanel::MakeInstance()
//{
//	//return MakeShareable(new NexusActorDetailsPanel);
//
//}
//
//void NexusActorDetailsPanel::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
//{
//	// Optional Initially Excluded Categories
//	TArray<FName> InitialExcludedNames = {"Sockets", "Events", "Variable" };
//	ExcludedDefaultCategoryNames.Append(InitialExcludedNames);
//
//
//	// Singleton Method to get details panel excluded categories names
//	NexusActorReference = UNexusActorComponent::GetInstace();
//	TArray NexusActorExcludedCategories = NexusActorReference->GetDetailsPanelExcludedCategories();
//	ExcludedDefaultCategoryNames.Append(NexusActorExcludedCategories);
//
//
//	// Get all default category names into an array
//	DetailBuilder.GetCategoryNames(DefaultCategoryNames);
//
//
//	//
//	// Create links category in the details builder
//	//
//	
//	// Create new category name
//	FName LinkedCategoryName = ("Linked Components Settings");
//
//	// Add to exlude names list
//
//	ExcludedDefaultCategoryNames.AddUnique(LinkedCategoryName);
//
//	IDetailCategoryBuilder& LinkedComponentsCategory = DetailBuilder.EditCategory(LinkedCategoryName);
//
//	IDetailGroup& LinkManagmentGroup = LinkedComponentsCategory.AddGroup(FName("Link Details"), FText::FromString("Link Details"));
//
//	//
//	// Create new group for default categories
//	//
//
//	IDetailCategoryBuilder& DefaultParentCategory = DetailBuilder.EditCategory("Main Component Settings");
//
//	DefaultParentCategory.SetShowAdvanced(true);
//
//	for (auto CategoryName : DefaultCategoryNames)
//	{
//
//		// Skip Names inside exclude category names list
//
//		if (ExcludedDefaultCategoryNames.Contains(CategoryName))
//		{
//			continue;
//		}
//
//		// Assign Detail Category Builder for each default category
//
//		IDetailCategoryBuilder& DefaultCategory = DetailBuilder.EditCategory(CategoryName);
//
//		// Store property handles of default categories in array
//
//		TArray<TSharedRef<IPropertyHandle>> Properties;
//		DefaultCategory.GetDefaultProperties(Properties);
//
//
//		// Create a group under the parent category with the default category's name for each default category
//		IDetailGroup& DefaultCategoryGroup = DefaultParentCategory.AddGroup(FName(*CategoryName.ToString()), FText::FromString(CategoryName.ToString()));
//
//
//		// keep all the default group names as rows under the parent 
//
//		for (TSharedRef<IPropertyHandle> PropertyHandle : Properties)
//		{
//
//			if (ExcludedDefaultCategoryNames.Contains(PropertyHandle.Get().GetDefaultCategoryName()))
//			{
//				LinkManagmentGroup.AddPropertyRow(PropertyHandle);
//			}
//
//			else
//			{
//				DefaultCategoryGroup.AddPropertyRow(PropertyHandle);
//				DefaultCategoryGroup.SetDisplayMode(EDetailGroupDisplayMode::Category);
//			}
//		}
//
//		if (!ExcludedDefaultCategoryNames.Contains(CategoryName))
//		{
//			DetailBuilder.HideCategory(CategoryName);
//		}
//	}
//}

