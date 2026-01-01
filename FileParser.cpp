#include "FileParser.h"

FString UFileParser::Parse_Implementation(const FString& /*FilePath*/)
{
    return FString();
}

FString UFileParser::GetFileType_Implementation() const
{
    return TEXT("Unknown File Type");
}

