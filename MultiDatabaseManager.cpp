#include "MultiDatabaseManager.h"

#include "VectorDatabaseBase.h"

void UMultiDatabaseManager::AddDatabase(UVectorDatabaseBase* Db)
{
    if (Db)
    {
        Databases.Add(Db);
    }
}

void UMultiDatabaseManager::GetAllDatabases(TArray<UVectorDatabaseBase*>& OutDatabases) const
{
    OutDatabases.Reset();
    OutDatabases.Reserve(Databases.Num());
    for (UVectorDatabaseBase* Db : Databases)
    {
        OutDatabases.Add(Db);
    }
}

