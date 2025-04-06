// Copyright H.D OWL. All rights reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Containers/Map.h"
#include "VariableStorageBPLibrary.generated.h"

USTRUCT(BlueprintType, meta = (HiddenByDefault))
struct FVariableStorage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "VariableStorage")
	TMap<FName, FString> VariableData;
	FVariableStorage()
	{
	}
};

UCLASS()
class UVariableStorageBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintPure, CustomThunk, Category = "Variable Storage", DisplayName = "Set Data To Variable Storage", meta = (ToolTip = "Set Data To Variable Storage \n 1. Use the function only with these variable types: Bool, Byte, Int32, Int64, Float, Name, String, Text, Vector, Rotator, Transform, Linear Color. \n 2. Do not use these character combinations in text variables : '@|@', '&|&'. \n 3. Do not leave 'Name' blank. \n Failure to follow these rules will result in incorrect results or an error.", ReturnDisplayName = "Variable Storage", Keywords = "Set Data To Variable Storage", CustomStructureParam = "Data"))
	static FVariableStorage SetDataToVariableStorage(int32 Data, FName NameData);

	UFUNCTION(BlueprintPure, CustomThunk, Category = "Variable Storage", DisplayName = "Set Data To Variable Storage (Array)", meta = (ToolTip = "Set Data To Variable Storage (Array) \n 1. Use the function only with these variable types: Bool, Byte, Int32, Int64, Float, Name, String, Text, Vector, Rotator, Transform, Linear Color. \n 2. Do not use these character combinations in text variables : '@|@', '&|&'. \n 3. Do not leave 'Name' blank. \n Failure to follow these rules will result in incorrect results or an error.", ReturnDisplayName = "Variable Storage", Keywords = "Set Data To Variable Storage Array", ArrayParm = "Data"))
	static FVariableStorage SetDataToVariableStorageArray(TArray<int32> Data, FName NameData);

	UFUNCTION(BlueprintPure, CustomThunk, Category = "Variable Storage", DisplayName = "Get Data To Variable Storage", meta = (ToolTip = "Get Data To Variable Storage \n 1. Use the function only with these variable types: Bool, Byte, Int32, Int64, Float, Name, String, Text, Vector, Rotator, Transform, Linear Color. \n 2. Do not use these character combinations in text variables : '@|@', '&|&'. \n 3. Do not leave 'Name' blank. \n Failure to follow these rules will result in incorrect results or an error.", Keywords = "Get Data To Variable Storage", CustomStructureParam = "Data"))
	static void GetDataToVariableStorage(int32& Data, FVariableStorage VariableStorage, FName NameData);

	UFUNCTION(BlueprintPure, CustomThunk, Category = "Variable Storage", DisplayName = "Get Data To Variable Storage (Array)", meta = (ToolTip = "Get Data To Variable Storage (Array) \n 1. Use the function only with these variable types: Bool, Byte, Int32, Int64, Float, Name, String, Text, Vector, Rotator, Transform, Linear Color. \n 2. Do not use these character combinations in text variables : '@|@', '&|&'. \n 3. Do not leave 'Name' blank. \n Failure to follow these rules will result in incorrect results or an error.", Keywords = "Get Data To Variable Storage Array", ArrayParm = "Data"))
	static void GetDataToVariableStorageArray(TArray<int32>& Data, FVariableStorage VariableStorage, FName NameData);

	UFUNCTION(BlueprintPure, Category = "Variable Storage", DisplayName = "Multi Append Variable Storage", meta = (ToolTip = "Multi Append Variable Storage \n Note: Variable Storage cannot contain 'Data' with the same 'Name'. \n 'Data' with the same 'Name' will replace each other.", Keywords = "Multi Append Variable Storage VariableStorage"))
	static void MultiAppendVariableStorage(TArray<FVariableStorage> InputVariableStorages, FVariableStorage& OutputVariableStorage);

private:
	DECLARE_FUNCTION(execSetDataToVariableStorage);
	DECLARE_FUNCTION(execSetDataToVariableStorageArray);
	DECLARE_FUNCTION(execGetDataToVariableStorage);
	DECLARE_FUNCTION(execGetDataToVariableStorageArray);
};
