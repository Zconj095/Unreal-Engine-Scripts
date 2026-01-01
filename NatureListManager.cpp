#include "NatureListManager.h"

ANatureListManager::ANatureListManager()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ANatureListManager::BeginPlay()
{
    Super::BeginPlay();

    // Initialize lists with example data
    InitializeFlowers();
    InitializeClouds();
    InitializeBerries();

    // Display lists
    DisplayList(TEXT("Flowers"));
    DisplayList(TEXT("Clouds"));
    DisplayList(TEXT("Berries"));
}

void ANatureListManager::InitializeFlowers()
{
    ListOfFlowers.Add(TEXT("Rose"));
    ListOfFlowers.Add(TEXT("Tulip"));
    ListOfFlowers.Add(TEXT("Daisy"));
    ListOfFlowers.Add(TEXT("Sunflower"));
    ListOfFlowers.Add(TEXT("Orchid"));
}

void ANatureListManager::InitializeClouds()
{
    ListOfClouds.Add(TEXT("Cumulus"));
    ListOfClouds.Add(TEXT("Stratus"));
    ListOfClouds.Add(TEXT("Cirrus"));
    ListOfClouds.Add(TEXT("Nimbus"));
    ListOfClouds.Add(TEXT("Altostratus"));
}

void ANatureListManager::InitializeBerries()
{
    ListOfBerries.Add(TEXT("Strawberry"));
    ListOfBerries.Add(TEXT("Blueberry"));
    ListOfBerries.Add(TEXT("Raspberry"));
    ListOfBerries.Add(TEXT("Blackberry"));
    ListOfBerries.Add(TEXT("Goji Berry"));
}

void ANatureListManager::DisplayListInternal(const FString& Category, const TArray<FString>& Items) const
{
    UE_LOG(LogTemp, Log, TEXT("--- %s ---"), *Category);
    for (const FString& Item : Items)
    {
        UE_LOG(LogTemp, Log, TEXT("%s"), *Item);
    }
}

void ANatureListManager::DisplayList(const FString& Category)
{
    const TArray<FString>* TargetList = GetListByCategoryConst(Category);
    if (TargetList)
    {
        DisplayListInternal(Category, *TargetList);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Category '%s' not recognized."), *Category);
    }
}

bool ANatureListManager::SearchInList(const FString& Category, const FString& Item) const
{
    const TArray<FString>* TargetList = GetListByCategoryConst(Category);
    if (TargetList && TargetList->Contains(Item))
    {
        UE_LOG(LogTemp, Log, TEXT("%s found in %s!"), *Item, *Category);
        return true;
    }
    UE_LOG(LogTemp, Log, TEXT("%s not found in %s."), *Item, *Category);
    return false;
}

bool ANatureListManager::AddToList(const FString& Category, const FString& Item)
{
    TArray<FString>* TargetList = GetListByCategoryMutable(Category);
    if (TargetList && !TargetList->Contains(Item))
    {
        TargetList->Add(Item);
        UE_LOG(LogTemp, Log, TEXT("%s added to %s."), *Item, *Category);
        return true;
    }
    UE_LOG(LogTemp, Log, TEXT("%s already exists in %s or category is invalid."), *Item, *Category);
    return false;
}

bool ANatureListManager::RemoveFromList(const FString& Category, const FString& Item)
{
    TArray<FString>* TargetList = GetListByCategoryMutable(Category);
    if (TargetList && TargetList->Contains(Item))
    {
        TargetList->Remove(Item);
        UE_LOG(LogTemp, Log, TEXT("%s removed from %s."), *Item, *Category);
        return true;
    }
    UE_LOG(LogTemp, Log, TEXT("%s not found in %s or category is invalid."), *Item, *Category);
    return false;
}

TArray<FString>* ANatureListManager::GetListByCategoryMutable(const FString& Category)
{
    FString Lower = Category.ToLower();
    if (Lower.Equals(TEXT("flowers")))
    {
        return &ListOfFlowers;
    }
    if (Lower.Equals(TEXT("clouds")))
    {
        return &ListOfClouds;
    }
    if (Lower.Equals(TEXT("berries")))
    {
        return &ListOfBerries;
    }
    UE_LOG(LogTemp, Warning, TEXT("Category '%s' not recognized."), *Category);
    return nullptr;
}

const TArray<FString>* ANatureListManager::GetListByCategoryConst(const FString& Category) const
{
    FString Lower = Category.ToLower();
    if (Lower.Equals(TEXT("flowers")))
    {
        return &ListOfFlowers;
    }
    if (Lower.Equals(TEXT("clouds")))
    {
        return &ListOfClouds;
    }
    if (Lower.Equals(TEXT("berries")))
    {
        return &ListOfBerries;
    }
    UE_LOG(LogTemp, Warning, TEXT("Category '%s' not recognized."), *Category);
    return nullptr;
}

