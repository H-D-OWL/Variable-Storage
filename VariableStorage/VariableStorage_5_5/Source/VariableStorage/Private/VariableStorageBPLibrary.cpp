// Copyright H.D OWL. All rights reserved.

#include "VariableStorageBPLibrary.h"
#include "Curves/CurveFloat.h"
#include "UObject/TextProperty.h"
#include "VariableStorage.h"

UVariableStorageBPLibrary::UVariableStorageBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

UPROPERTY()
FString TypeSeparator = "@|@";

UPROPERTY()
FString VariableSeparator = "&|&";

FVariableStorage UVariableStorageBPLibrary::SetDataToVariableStorage(int32 Data, FName NameData)
{
	checkNoEntry();
	return  FVariableStorage();
}

FVariableStorage UVariableStorageBPLibrary::SetDataToVariableStorageArray(TArray<int32> Data, FName NameData)
{
	checkNoEntry();
	return FVariableStorage();
}

void UVariableStorageBPLibrary::GetDataToVariableStorage(int32& Data, FVariableStorage VariableStorage, FName NameData)
{
	checkNoEntry();
}

void UVariableStorageBPLibrary::GetDataToVariableStorageArray(TArray<int32>& Data, FVariableStorage VariableStorage, FName NameData)
{
	checkNoEntry();
}

void UVariableStorageBPLibrary::MultiAppendVariableStorage(TArray<FVariableStorage> InputVariableStorages, FVariableStorage& OutputVariableStorage)
{
	for (FVariableStorage ArrayElement : InputVariableStorages)
	{
		OutputVariableStorage.VariableData.Append(ArrayElement.VariableData);

	}
}

DEFINE_FUNCTION(UVariableStorageBPLibrary::execSetDataToVariableStorage)
{
	Stack.MostRecentProperty = nullptr;
	Stack.StepCompiledIn<FProperty>(nullptr);
	void* ValueData = Stack.MostRecentPropertyAddress;
	const FProperty* Data = CastField<FProperty>(Stack.MostRecentProperty);

	if (!Data)
	{
		Stack.bArrayContextFailed = true;
		return;
	}

	PARAM_PASSED_BY_VAL(NameData, FNameProperty, FName);
	Stack.Step(Stack.Object, NULL);

	P_FINISH;
	P_NATIVE_BEGIN;

	if (Data)
	{
		FVariableStorage VariableStorage;
		FString TypeVariable = TypeSeparator;
		FString Variable;
		bool NonWorkingType = false;

		if (NameData.IsNone())
		{
			NonWorkingType = true;
			UE_LOG(LogTemp, Error, TEXT("The name cannot be empty (Variable Storage not created)"));
		}
		else if (CastField<FBoolProperty>(Data))  //Bool
		{
			TypeVariable.Append("Bool");
			Variable = FIntProperty::GetPropertyValue(ValueData) ? TEXT("true") : TEXT("false");
		}
		else if (CastField<FByteProperty>(Data))  //Byte
		{
			TypeVariable.Append("Byte");
			Variable = FString::Printf(TEXT("%d"), FByteProperty::GetPropertyValue(ValueData));
		}
		else if (CastField<FIntProperty>(Data))  //Int32
		{
			TypeVariable.Append("Int32");
			Variable = FString::Printf(TEXT("%d"), FIntProperty::GetPropertyValue(ValueData));
		}
		else if (CastField<FInt64Property>(Data))  //Int64
		{
			TypeVariable.Append("Int64");
			Variable = FString::Printf(TEXT("%lld"), FInt64Property::GetPropertyValue(ValueData));
		}
		else if (CastField<FFloatProperty>(Data))  //Float
		{
			TypeVariable.Append("Float");
			Variable = FString::SanitizeFloat(FFloatProperty::GetPropertyValue(ValueData));
		}
		else if (CastField<FDoubleProperty>(Data))  //Double
		{
			TypeVariable.Append("Double");
			Variable = FString::SanitizeFloat(FDoubleProperty::GetPropertyValue(ValueData));
		}
		else if (CastField<FNameProperty>(Data))  //Name
		{
			TypeVariable.Append("Name");
			Variable = FNameProperty::GetPropertyValue(ValueData).ToString();
		}
		else if (CastField<FStrProperty>(Data))  //String
		{
			TypeVariable.Append("String");
			Variable = FStrProperty::GetPropertyValue(ValueData);
		}
		else if (CastField<FTextProperty>(Data))  //Text
		{
			TypeVariable.Append("Text");
			Variable = FTextProperty::GetPropertyValue(ValueData).ToString();
		}
		else if (const FStructProperty* StructData = CastField<FStructProperty>(Data))
		{
			if (StructData->Struct->GetFName() == FName("Vector"))  //Vector
			{
				TypeVariable.Append("Vector");
				FVector Value = *reinterpret_cast<FVector*>(ValueData);
				Variable = Value.ToString();
			}
			else if (StructData->Struct->GetFName() == FName("Rotator"))  //Rotator
			{
				TypeVariable.Append("Rotator");
				FRotator Value = *reinterpret_cast<FRotator*>(ValueData);
				Variable = Value.ToString();
			}
			else if (StructData->Struct->GetFName() == FName("Transform"))  //Transform
			{
				TypeVariable.Append("Transform");
				FTransform Value = *reinterpret_cast<FTransform*>(ValueData);
				Variable = Value.ToString();
			}
			else if (StructData->Struct->GetFName() == FName("LinearColor"))  //Linear Color
			{
				TypeVariable.Append("LinearColor");
				FLinearColor Value = *reinterpret_cast<FLinearColor*>(ValueData);
				Variable = Value.ToString();
			}
		}
		else if (!NonWorkingType)
		{
			NonWorkingType = true;
			UE_LOG(LogTemp, Error, TEXT("Variable of this type cannot be processed (Variable Storage not created)"));
		}

		if (!NonWorkingType)
		{
			VariableStorage.VariableData.Add(NameData, Variable.Append(TypeVariable));
			*reinterpret_cast<FVariableStorage*>(RESULT_PARAM) = VariableStorage;
		}
	}
}
P_NATIVE_END;


DEFINE_FUNCTION(UVariableStorageBPLibrary::execSetDataToVariableStorageArray)
{

	Stack.MostRecentProperty = nullptr;
	Stack.StepCompiledIn<FArrayProperty>(nullptr);
	void* ValueData = Stack.MostRecentPropertyAddress;
	const FArrayProperty* Data = CastField<FArrayProperty>(Stack.MostRecentProperty);

	if (!Data)
	{
		Stack.bArrayContextFailed = true;
		return;
	}


	PARAM_PASSED_BY_VAL(NameData, FNameProperty, FName);
	Stack.Step(Stack.Object, NULL);

	P_FINISH;
	P_NATIVE_BEGIN;

	if (Data)
	{
		FVariableStorage VariableStorage;
		FString Variable;
		FString ArrayPart;
		FString TypeVariable = TypeSeparator;
		bool HasDoneIt = false;
		bool NonWorkingType = false;
		const FProperty* InnerProp = Data->Inner;

		if (NameData.IsNone())
		{
			NonWorkingType = true;
			UE_LOG(LogTemp, Error, TEXT("The name cannot be empty (Variable Storage not created)"));
		}
		else if (InnerProp)
		{
			FScriptArrayHelper ArrayHelper(Data, ValueData);

			for (int32 i = 0; i < ArrayHelper.Num(); ++i)
			{
				void* ElementAddress = ArrayHelper.GetRawPtr(i);

				if (CastField<FBoolProperty>(InnerProp))  //Bool
				{
					if (!HasDoneIt)
					{
						TypeVariable.Append("Bool");
						HasDoneIt = true;
					}

					const FBoolProperty* VariableProp = CastField<FBoolProperty>(InnerProp);
					ArrayPart = VariableProp->GetPropertyValue(ElementAddress) ? TEXT("true") : TEXT("false");
				}
				else if (CastField<FByteProperty>(InnerProp))  //Byte
				{
					if (!HasDoneIt)
					{
						TypeVariable.Append("Byte");
						HasDoneIt = true;
					}

					const FByteProperty* VariableProp = CastField<FByteProperty>(InnerProp);
					ArrayPart = (FString::Printf(TEXT("%d"), VariableProp->GetPropertyValue(ElementAddress)));
				}
				else if (CastField<FIntProperty>(InnerProp))  //Int32
				{
					if (!HasDoneIt)
					{
						TypeVariable.Append("Int32");
						HasDoneIt = true;
					}

					const FIntProperty* VariableProp = CastField<FIntProperty>(InnerProp);
					ArrayPart = (FString::Printf(TEXT("%d"), VariableProp->GetPropertyValue(ElementAddress)));
				}
				else if (CastField<FInt64Property>(InnerProp))  //Int64
				{
					if (!HasDoneIt)
					{
						TypeVariable.Append("Int64");
						HasDoneIt = true;
					}

					const FInt64Property* VariableProp = CastField<FInt64Property>(InnerProp);
					ArrayPart = (FString::Printf(TEXT("%lld"), VariableProp->GetPropertyValue(ElementAddress)));
				}
				else if (CastField<FFloatProperty>(InnerProp))  //Float
				{
					if (!HasDoneIt)
					{
						TypeVariable.Append("Float");
						HasDoneIt = true;
					}

					const FFloatProperty* VariableProp = CastField<FFloatProperty>(InnerProp);
					ArrayPart = FString::SanitizeFloat(VariableProp->GetPropertyValue(ElementAddress));
				}
				else if (CastField<FDoubleProperty>(InnerProp))  //Double
				{
					if (!HasDoneIt)
					{
						TypeVariable.Append("Double");
						HasDoneIt = true;
					}

					const FDoubleProperty* VariableProp = CastField<FDoubleProperty>(InnerProp);
					ArrayPart = FString::SanitizeFloat(VariableProp->GetPropertyValue(ElementAddress));
				}
				else if (CastField<FNameProperty>(InnerProp))  //Name
				{
					if (!HasDoneIt)
					{
						TypeVariable.Append("Name");
						HasDoneIt = true;
					}

					const FNameProperty* VariableProp = CastField<FNameProperty>(InnerProp);
					ArrayPart = VariableProp->GetPropertyValue(ElementAddress).ToString();
				}
				else if (CastField<FStrProperty>(InnerProp))  //String
				{
					if (!HasDoneIt)
					{
						TypeVariable.Append("String");
						HasDoneIt = true;
					}

					const FStrProperty* VariableProp = CastField<FStrProperty>(InnerProp);
					ArrayPart = VariableProp->GetPropertyValue(ElementAddress);
				}
				else if (CastField<FTextProperty>(InnerProp))  //Text
				{
					if (!HasDoneIt)
					{
						TypeVariable.Append("Text");
						HasDoneIt = true;
					}

					const FTextProperty* VariableProp = CastField<FTextProperty>(InnerProp);
					ArrayPart = VariableProp->GetPropertyValue(ElementAddress).ToString();
				}
				else if (const FStructProperty* StructData = CastField<FStructProperty>(InnerProp))
				{
					if (StructData->Struct->GetFName() == FName("Vector"))  //Vector
					{
						if (!HasDoneIt)
						{
							TypeVariable.Append("Vector");
							HasDoneIt = true;
						}

						FVector Value = *reinterpret_cast<FVector*>(ElementAddress);
						ArrayPart = Value.ToString();
					}
					else if (StructData->Struct->GetFName() == FName("Rotator"))  //Rotator
					{
						if (!HasDoneIt)
						{
							TypeVariable.Append("Rotator");
							HasDoneIt = true;
						}

						FRotator Value = *reinterpret_cast<FRotator*>(ElementAddress);
						ArrayPart = Value.ToString();
					}
					else if (StructData->Struct->GetFName() == FName("Transform"))  //Transform
					{
						if (!HasDoneIt)
						{
							TypeVariable.Append("Transform");
							HasDoneIt = true;
						}

						FTransform Value = *reinterpret_cast<FTransform*>(ElementAddress);
						ArrayPart = Value.ToString();
					}
					else if (StructData->Struct->GetFName() == FName("LinearColor"))  //Color
					{
						if (!HasDoneIt)
						{
							TypeVariable.Append("LinearColor");
							HasDoneIt = true;
						}

						FLinearColor Value = *reinterpret_cast<FLinearColor*>(ElementAddress);
						ArrayPart = Value.ToString();
					}
				}
				else if (!NonWorkingType)
				{
					NonWorkingType = true;
					UE_LOG(LogTemp, Error, TEXT("Variable of this type cannot be processed (Variable Storage not created)"));
				}

				if (Variable.IsEmpty())
				{
					Variable = ArrayPart;
				}
				else
				{
					Variable.Append(VariableSeparator).Append(ArrayPart);
				}
			}

			if (!NonWorkingType)
			{
				TypeVariable = TypeVariable.Append("|Array");
				VariableStorage.VariableData.Add(NameData, Variable.Append(TypeVariable));
				*reinterpret_cast<FVariableStorage*>(RESULT_PARAM) = VariableStorage;
			}
		}
	}
}
P_NATIVE_END;

DEFINE_FUNCTION(UVariableStorageBPLibrary::execGetDataToVariableStorage)
{
	Stack.MostRecentProperty = nullptr;
	Stack.StepCompiledIn<FProperty>(nullptr);
	void* ValueData = Stack.MostRecentPropertyAddress;
	const FProperty* Data = CastField<FProperty>(Stack.MostRecentProperty);

	if (!Data)
	{
		Stack.bArrayContextFailed = true;
		return;
	}

	Stack.MostRecentProperty = nullptr;
	Stack.StepCompiledIn<FStructProperty>(nullptr);
	FVariableStorage VariableStorage = *reinterpret_cast<FVariableStorage*>(Stack.MostRecentPropertyAddress);

	PARAM_PASSED_BY_VAL(NameData, FNameProperty, FName);
	Stack.Step(Stack.Object, NULL);

	P_FINISH;
	P_NATIVE_BEGIN;

	if (Data)
	{
		FString Variable;
		FString TypeVariable;
		bool NonWorkingType = false;


		if (NameData.IsNone())
		{
			//NonWorkingType = true;
			UE_LOG(LogTemp, Error, TEXT("The name cannot be empty (No data received)"));
		}
		else if (VariableStorage.VariableData.Contains(NameData))
		{
			Variable = *VariableStorage.VariableData.Find(NameData);

			if (Variable.Contains(TypeSeparator))
			{
				Variable.Split(TypeSeparator, &Variable, &TypeVariable);

				if (!TypeVariable.Contains("|Array"))
				{
					if (CastField<FBoolProperty>(Data))  //Bool
					{
						if (TypeVariable == "Bool")
						{
							CastField<FBoolProperty>(Data)->SetPropertyValue(ValueData, FCString::ToBool(*Variable));
						}
						else { NonWorkingType = true; }

					}
					else if (CastField<FByteProperty>(Data))  //Byte
					{
						if (TypeVariable == "Byte")
						{
							CastField<FByteProperty>(Data)->SetPropertyValue(ValueData, FCString::Atoi(*Variable));
						}
						else { NonWorkingType = true; }

					}
					else if (CastField<FIntProperty>(Data))  //Int32
					{
						if (TypeVariable == "Int32")
						{
							CastField<FIntProperty>(Data)->SetPropertyValue(ValueData, FCString::Atoi(*Variable));
						}
						else { NonWorkingType = true; }

					}
					else if (CastField<FInt64Property>(Data))  //Int64
					{
						if (TypeVariable == "Int64")
						{
							CastField<FInt64Property>(Data)->SetPropertyValue(ValueData, FCString::Atoi64(*Variable));
						}
						else { NonWorkingType = true; }

					}
					else if (CastField<FFloatProperty>(Data))  //Float
					{
						if (TypeVariable == "Float")
						{
							CastField<FFloatProperty>(Data)->SetPropertyValue(ValueData, FCString::Atof(*Variable));
						}
						else { NonWorkingType = true; }

					}
					else if (CastField<FDoubleProperty>(Data))  //Double
					{
						if (TypeVariable == "Double")
						{
							CastField<FDoubleProperty>(Data)->SetPropertyValue(ValueData, FCString::Atod(*Variable));
						}
						else { NonWorkingType = true; }

					}
					else if (CastField<FNameProperty>(Data))  //Name
					{
						if (TypeVariable == "Name")
						{
							CastField<FNameProperty>(Data)->SetPropertyValue(ValueData, FName(Variable));
						}
						else { NonWorkingType = true; }

					}
					else if (CastField<FStrProperty>(Data))  //String
					{
						if (TypeVariable == "String")
						{
							CastField<FStrProperty>(Data)->SetPropertyValue(ValueData, Variable);
						}
						else { NonWorkingType = true; }

					}
					else if (CastField<FTextProperty>(Data))  //Text
					{
						if (TypeVariable == "Text")
						{
							CastField<FTextProperty>(Data)->SetPropertyValue(ValueData, FText::AsCultureInvariant(Variable));
						}
						else { NonWorkingType = true; }
					}
					else if (const FStructProperty* StructData = CastField<FStructProperty>(Data))
					{
						if (StructData->Struct->GetFName() == FName("Vector"))  //Vector
						{
							if (TypeVariable == "Vector")
							{
								FVector Value;
								Value.InitFromString(Variable);
								*reinterpret_cast<FVector*>(ValueData) = Value;
							}
							else { NonWorkingType = true; }
						}
						else if (StructData->Struct->GetFName() == FName("Rotator"))  //Rotator
						{
							if (TypeVariable == "Rotator")
							{
								FRotator Value;
								Value.InitFromString(Variable);
								*reinterpret_cast<FRotator*>(ValueData) = Value;
							}
							else { NonWorkingType = true; }
						}
						else if (StructData->Struct->GetFName() == FName("Transform"))  //Transform
						{
							if (TypeVariable == "Transform")
							{
								FTransform Value;
								FVector VectorValue;
								FRotator RotatorValue;
								FQuat QuatValue;
								FString RemainingPartValue;
								FString FStringValue1;
								FString FStringValue2;
								FString FStringValue3;

								Variable.Split(TEXT("|"), &Variable, &RemainingPartValue);

								FStringValue1 = Variable;
								FStringValue1.Split(TEXT(","), &FStringValue1, &FStringValue2);
								FStringValue2.Split(TEXT(","), &FStringValue2, &FStringValue3);
								VectorValue.Set(FCString::Atod(*FStringValue1), FCString::Atod(*FStringValue2), FCString::Atod(*FStringValue3));
								Value.SetLocation(VectorValue);

								RemainingPartValue.Split(TEXT("|"), &Variable, &RemainingPartValue);

								FStringValue1 = Variable;
								FStringValue1.Split(TEXT(","), &FStringValue1, &FStringValue2);
								FStringValue2.Split(TEXT(","), &FStringValue2, &FStringValue3);
								RotatorValue.Add(FCString::Atod(*FStringValue1), FCString::Atod(*FStringValue2), FCString::Atod(*FStringValue3));
								QuatValue = RotatorValue.Quaternion();
								Value.SetRotation(QuatValue);

								FStringValue1 = RemainingPartValue;
								FStringValue1.Split(TEXT(","), &FStringValue1, &FStringValue2);
								FStringValue2.Split(TEXT(","), &FStringValue2, &FStringValue3);
								VectorValue.Set(FCString::Atod(*FStringValue1), FCString::Atod(*FStringValue2), FCString::Atod(*FStringValue3));
								Value.SetScale3D(VectorValue);

								*reinterpret_cast<FTransform*>(ValueData) = Value;
							}
							else { NonWorkingType = true; }
						}
						else if (StructData->Struct->GetFName() == FName("LinearColor"))  //Color
						{
							if (TypeVariable == "LinearColor")
							{
								FLinearColor Value;
								Value.InitFromString(Variable);
								*reinterpret_cast<FLinearColor*>(ValueData) = Value;
							}
							else { NonWorkingType = true; }
						}
					}
					else if (!NonWorkingType)
					{
						UE_LOG(LogTemp, Error, TEXT("Variable of this type cannot be processed (No data received)"));
					}

				}
				else
				{
					//TypeVariable = TypeVariable.Mid(0, TypeVariable.Len() - 6);
					UE_LOG(LogTemp, Error, TEXT("The data you are trying to get is an array (No data received)"));
				}
			}
		}
		if (NonWorkingType)
		{
			UE_LOG(LogTemp, Error, TEXT("The type of data you are trying to retrieve does not match the type of data from Variable Storage (No data received)"));
		}
	}
}
P_NATIVE_END;

DEFINE_FUNCTION(UVariableStorageBPLibrary::execGetDataToVariableStorageArray)
{
	Stack.MostRecentProperty = nullptr;
	Stack.StepCompiledIn<FArrayProperty>(nullptr);
	void* ValueData = Stack.MostRecentPropertyAddress;
	const FArrayProperty* Data = CastField<FArrayProperty>(Stack.MostRecentProperty);

	if (!Data)
	{
		Stack.bArrayContextFailed = true;
		return;
	}

	Stack.MostRecentProperty = nullptr;
	Stack.StepCompiledIn<FStructProperty>(nullptr);
	FVariableStorage VariableStorage = *reinterpret_cast<FVariableStorage*>(Stack.MostRecentPropertyAddress);

	PARAM_PASSED_BY_VAL(NameData, FNameProperty, FName);
	Stack.Step(Stack.Object, NULL);

	P_FINISH;
	P_NATIVE_BEGIN;

	if (Data)
	{
		FString Variable;
		FString TypeVariable;
		int32 ArrayIndex = 0;
		bool NonWorkingType = false;
		const FProperty* InnerProp = Data->Inner;
		FScriptArrayHelper ArrayHelper(Data, ValueData);
		void* ElementAddress;

		if (NameData.IsNone())
		{
			UE_LOG(LogTemp, Error, TEXT("The name cannot be empty (No data received)"));
		}
		else if (VariableStorage.VariableData.Contains(NameData))
		{
			Variable = *VariableStorage.VariableData.Find(NameData);

			if (Variable.Contains(TypeSeparator))
			{
				Variable.Split(TypeSeparator, &Variable, &TypeVariable);

				if (TypeVariable.Contains("|Array"))
				{
					FString RemainingPart;
					TypeVariable = TypeVariable.Mid(0, TypeVariable.Len() - 6);

					while (!Variable.IsEmpty() and NonWorkingType == false)
					{
						if (CastField<FBoolProperty>(InnerProp))  //Bool
						{
							if (TypeVariable == "Bool")
							{
								ArrayHelper.AddValue();
								ElementAddress = ArrayHelper.GetRawPtr(ArrayIndex);

								if (Variable.Contains(VariableSeparator))
								{
									Variable.Split(VariableSeparator, &Variable, &RemainingPart);
									const FBoolProperty* VariableProp = CastField<FBoolProperty>(InnerProp);
									VariableProp->SetPropertyValue(ElementAddress, FCString::ToBool(*Variable));
									Variable = RemainingPart;
									ArrayIndex++;
								}
								else
								{
									const FBoolProperty* VariableProp = CastField<FBoolProperty>(InnerProp);
									VariableProp->SetPropertyValue(ElementAddress, FCString::ToBool(*Variable));
									Variable = "";
								}
							}
							else { NonWorkingType = true; }
						}
						else if (CastField<FByteProperty>(InnerProp))  //Byte
						{
							if (TypeVariable == "Byte")
							{
								ArrayHelper.AddValue();
								ElementAddress = ArrayHelper.GetRawPtr(ArrayIndex);

								if (Variable.Contains(VariableSeparator))
								{
									Variable.Split(VariableSeparator, &Variable, &RemainingPart);
									const FByteProperty* VariableProp = CastField<FByteProperty>(InnerProp);
									VariableProp->SetPropertyValue(ElementAddress, FCString::Atoi(*Variable));
									Variable = RemainingPart;
									ArrayIndex++;
								}
								else
								{
									const FByteProperty* VariableProp = CastField<FByteProperty>(InnerProp);
									VariableProp->SetPropertyValue(ElementAddress, FCString::Atoi(*Variable));
									Variable = "";
								}
							}
							else { NonWorkingType = true; }

						}
						else if (CastField<FIntProperty>(InnerProp))  //Int32
						{
							if (TypeVariable == "Int32")
							{
								ArrayHelper.AddValue();
								ElementAddress = ArrayHelper.GetRawPtr(ArrayIndex);

								if (Variable.Contains(VariableSeparator))
								{
									Variable.Split(VariableSeparator, &Variable, &RemainingPart);
									const FIntProperty* VariableProp = CastField<FIntProperty>(InnerProp);
									VariableProp->SetPropertyValue(ElementAddress, FCString::Atoi(*Variable));
									Variable = RemainingPart;
									ArrayIndex++;
								}
								else
								{
									const FIntProperty* VariableProp = CastField<FIntProperty>(InnerProp);
									VariableProp->SetPropertyValue(ElementAddress, FCString::Atoi(*Variable));
									Variable = "";
								}
							}
							else { NonWorkingType = true; }

						}
						else if (CastField<FInt64Property>(InnerProp))  //Int64
						{
							if (TypeVariable == "Int64")
							{
								ArrayHelper.AddValue();
								ElementAddress = ArrayHelper.GetRawPtr(ArrayIndex);

								if (Variable.Contains(VariableSeparator))
								{
									Variable.Split(VariableSeparator, &Variable, &RemainingPart);
									const FInt64Property* VariableProp = CastField<FInt64Property>(InnerProp);
									VariableProp->SetPropertyValue(ElementAddress, FCString::Atoi64(*Variable));
									Variable = RemainingPart;
									ArrayIndex++;
								}
								else
								{
									const FInt64Property* VariableProp = CastField<FInt64Property>(InnerProp);
									VariableProp->SetPropertyValue(ElementAddress, FCString::Atoi64(*Variable));
									Variable = "";
								}
							}
							else { NonWorkingType = true; }

						}
						else if (CastField<FFloatProperty>(InnerProp))  //Float
						{
							if (TypeVariable == "Float")
							{
								ArrayHelper.AddValue();
								ElementAddress = ArrayHelper.GetRawPtr(ArrayIndex);

								if (Variable.Contains(VariableSeparator))
								{
									Variable.Split(VariableSeparator, &Variable, &RemainingPart);
									const FFloatProperty* VariableProp = CastField<FFloatProperty>(InnerProp);
									VariableProp->SetPropertyValue(ElementAddress, FCString::Atof(*Variable));
									Variable = RemainingPart;
									ArrayIndex++;
								}
								else
								{
									const FFloatProperty* VariableProp = CastField<FFloatProperty>(InnerProp);
									VariableProp->SetPropertyValue(ElementAddress, FCString::Atof(*Variable));
									Variable = "";
								}
							}
							else { NonWorkingType = true; }

						}
						else if (CastField<FDoubleProperty>(InnerProp))  //Double
						{
							if (TypeVariable == "Double")
							{
								ArrayHelper.AddValue();
								ElementAddress = ArrayHelper.GetRawPtr(ArrayIndex);

								if (Variable.Contains(VariableSeparator))
								{
									Variable.Split(VariableSeparator, &Variable, &RemainingPart);
									const FDoubleProperty* VariableProp = CastField<FDoubleProperty>(InnerProp);
									VariableProp->SetPropertyValue(ElementAddress, FCString::Atod(*Variable));
									Variable = RemainingPart;
									ArrayIndex++;
								}
								else
								{
									const FDoubleProperty* VariableProp = CastField<FDoubleProperty>(InnerProp);
									VariableProp->SetPropertyValue(ElementAddress, FCString::Atod(*Variable));
									Variable = "";
								}
							}
							else { NonWorkingType = true; }

						}
						else if (CastField<FNameProperty>(InnerProp))  //Name
						{
							if (TypeVariable == "Name")
							{
								ArrayHelper.AddValue();
								ElementAddress = ArrayHelper.GetRawPtr(ArrayIndex);

								if (Variable.Contains(VariableSeparator))
								{
									Variable.Split(VariableSeparator, &Variable, &RemainingPart);
									const FNameProperty* VariableProp = CastField<FNameProperty>(InnerProp);
									VariableProp->SetPropertyValue(ElementAddress, FName(*Variable));
									Variable = RemainingPart;
									ArrayIndex++;
								}
								else
								{
									const FNameProperty* VariableProp = CastField<FNameProperty>(InnerProp);
									VariableProp->SetPropertyValue(ElementAddress, FName(*Variable));
									Variable = "";
								}
							}
							else { NonWorkingType = true; }

						}
						else if (CastField<FStrProperty>(InnerProp))  //String
						{
							if (TypeVariable == "String")
							{
								ArrayHelper.AddValue();
								ElementAddress = ArrayHelper.GetRawPtr(ArrayIndex);

								if (Variable.Contains(VariableSeparator))
								{
									Variable.Split(VariableSeparator, &Variable, &RemainingPart);
									const FStrProperty* VariableProp = CastField<FStrProperty>(InnerProp);
									VariableProp->SetPropertyValue(ElementAddress, *Variable);
									Variable = RemainingPart;
									ArrayIndex++;
								}
								else
								{
									const FStrProperty* VariableProp = CastField<FStrProperty>(InnerProp);
									VariableProp->SetPropertyValue(ElementAddress, *Variable);
									Variable = "";
								}
							}
							else { NonWorkingType = true; }

						}
						else if (CastField<FTextProperty>(InnerProp))  //Text
						{
							if (TypeVariable == "Text")
							{
								ArrayHelper.AddValue();
								ElementAddress = ArrayHelper.GetRawPtr(ArrayIndex);

								if (Variable.Contains(VariableSeparator))
								{
									Variable.Split(VariableSeparator, &Variable, &RemainingPart);
									const FTextProperty* VariableProp = CastField<FTextProperty>(InnerProp);
									VariableProp->SetPropertyValue(ElementAddress, FText::AsCultureInvariant(*Variable));
									Variable = RemainingPart;
									ArrayIndex++;
								}
								else
								{
									const FTextProperty* VariableProp = CastField<FTextProperty>(InnerProp);
									VariableProp->SetPropertyValue(ElementAddress, FText::AsCultureInvariant(*Variable));
									Variable = "";
								}
							}
							else { NonWorkingType = true; }
						}
						else if (const FStructProperty* StructData = CastField<FStructProperty>(InnerProp))
						{
							if (StructData->Struct->GetFName() == FName("Vector"))  //Vector
							{
								if (TypeVariable == "Vector")
								{
									ArrayHelper.AddValue();
									ElementAddress = ArrayHelper.GetRawPtr(ArrayIndex);
									FVector Value;

									if (Variable.Contains(VariableSeparator))
									{
										Variable.Split(VariableSeparator, &Variable, &RemainingPart);
										Value.InitFromString(Variable);
										*reinterpret_cast<FVector*>(ElementAddress) = Value;
										Variable = RemainingPart;
										ArrayIndex++;
									}
									else
									{
										Value.InitFromString(Variable);
										*reinterpret_cast<FVector*>(ElementAddress) = Value;
										Variable = "";
									}
								}
								else { NonWorkingType = true; }
							}
							else if (StructData->Struct->GetFName() == FName("Rotator"))  //Rotator
							{
								if (TypeVariable == "Rotator")
								{
									ArrayHelper.AddValue();
									ElementAddress = ArrayHelper.GetRawPtr(ArrayIndex);
									FRotator Value;

									if (Variable.Contains(VariableSeparator))
									{
										Variable.Split(VariableSeparator, &Variable, &RemainingPart);
										Value.InitFromString(Variable);
										*reinterpret_cast<FRotator*>(ElementAddress) = Value;
										Variable = RemainingPart;
										ArrayIndex++;
									}
									else
									{
										Value.InitFromString(Variable);
										*reinterpret_cast<FRotator*>(ElementAddress) = Value;
										Variable = "";
									}
								}
								else { NonWorkingType = true; }
							}
							else if (StructData->Struct->GetFName() == FName("Transform"))  //Transform
							{
								if (TypeVariable == "Transform")
								{
									ArrayHelper.AddValue();
									ElementAddress = ArrayHelper.GetRawPtr(ArrayIndex);
									FTransform Value;
									FVector VectorValue;
									FRotator RotatorValue;
									FQuat QuatValue;
									FString RemainingPartValue;
									FString FStringValue1;
									FString FStringValue2;
									FString FStringValue3;

									if (Variable.Contains(VariableSeparator))
									{
										Variable.Split(VariableSeparator, &Variable, &RemainingPart);

										Variable.Split(TEXT("|"), &Variable, &RemainingPartValue);

										FStringValue1 = Variable;
										FStringValue1.Split(TEXT(","), &FStringValue1, &FStringValue2);
										FStringValue2.Split(TEXT(","), &FStringValue2, &FStringValue3);
										VectorValue.Set(FCString::Atod(*FStringValue1), FCString::Atod(*FStringValue2), FCString::Atod(*FStringValue3));
										Value.SetLocation(VectorValue);

										RemainingPartValue.Split(TEXT("|"), &Variable, &RemainingPartValue);

										FStringValue1 = Variable;
										FStringValue1.Split(TEXT(","), &FStringValue1, &FStringValue2);
										FStringValue2.Split(TEXT(","), &FStringValue2, &FStringValue3);
										RotatorValue.Add(FCString::Atod(*FStringValue1), FCString::Atod(*FStringValue2), FCString::Atod(*FStringValue3));
										QuatValue = RotatorValue.Quaternion();
										Value.SetRotation(QuatValue);

										FStringValue1 = RemainingPartValue;
										FStringValue1.Split(TEXT(","), &FStringValue1, &FStringValue2);
										FStringValue2.Split(TEXT(","), &FStringValue2, &FStringValue3);
										VectorValue.Set(FCString::Atod(*FStringValue1), FCString::Atod(*FStringValue2), FCString::Atod(*FStringValue3));
										Value.SetScale3D(VectorValue);

										*reinterpret_cast<FTransform*>(ElementAddress) = Value;
										Variable = RemainingPart;
										ArrayIndex++;
									}
									else
									{
										Variable.Split(TEXT("|"), &Variable, &RemainingPartValue);

										FStringValue1 = Variable;
										FStringValue1.Split(TEXT(","), &FStringValue1, &FStringValue2);
										FStringValue2.Split(TEXT(","), &FStringValue2, &FStringValue3);
										VectorValue.Set(FCString::Atod(*FStringValue1), FCString::Atod(*FStringValue2), FCString::Atod(*FStringValue3));
										Value.SetLocation(VectorValue);

										RemainingPartValue.Split(TEXT("|"), &Variable, &RemainingPartValue);

										FStringValue1 = Variable;
										FStringValue1.Split(TEXT(","), &FStringValue1, &FStringValue2);
										FStringValue2.Split(TEXT(","), &FStringValue2, &FStringValue3);
										RotatorValue.Add(FCString::Atod(*FStringValue1), FCString::Atod(*FStringValue2), FCString::Atod(*FStringValue3));
										QuatValue = RotatorValue.Quaternion();
										Value.SetRotation(QuatValue);

										FStringValue1 = RemainingPartValue;
										FStringValue1.Split(TEXT(","), &FStringValue1, &FStringValue2);
										FStringValue2.Split(TEXT(","), &FStringValue2, &FStringValue3);
										VectorValue.Set(FCString::Atod(*FStringValue1), FCString::Atod(*FStringValue2), FCString::Atod(*FStringValue3));
										Value.SetScale3D(VectorValue);

										*reinterpret_cast<FTransform*>(ElementAddress) = Value;
										Variable = "";
									}
								}
								else { NonWorkingType = true; }
							}
							else if (StructData->Struct->GetFName() == FName("LinearColor"))  //Color
							{
								if (TypeVariable == "LinearColor")
								{
									ArrayHelper.AddValue();
									ElementAddress = ArrayHelper.GetRawPtr(ArrayIndex);
									FLinearColor Value;

									if (Variable.Contains(VariableSeparator))
									{
										Variable.Split(VariableSeparator, &Variable, &RemainingPart);
										Value.InitFromString(Variable);
										*reinterpret_cast<FLinearColor*>(ElementAddress) = Value;
										Variable = RemainingPart;
										ArrayIndex++;
									}
									else
									{
										Value.InitFromString(Variable);
										*reinterpret_cast<FLinearColor*>(ElementAddress) = Value;
										Variable = "";
									}
								}
								else { NonWorkingType = true; }
							}
						}
						else if (!NonWorkingType)
						{
							Variable = "";
							UE_LOG(LogTemp, Error, TEXT("Variable of this type cannot be processed (No data received)"));
						}
					}
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("The data you are trying to get is an array (No data received)"));
				}
			}
		}
		if (NonWorkingType)
		{
			UE_LOG(LogTemp, Error, TEXT("The type of data you are trying to retrieve does not match the type of data from Variable Storage (No data received)"));
		}
	}
}
P_NATIVE_END;