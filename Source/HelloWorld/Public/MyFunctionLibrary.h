// 블루프린트에서 실험용으로 쓸 함수 노드를 불러오기 위해 만든 라이브러리입니다.
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyFunctionLibrary.generated.h"

UCLASS()
class HELLOWORLD_API UMyFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static float GetFadeDuration(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static void StartFadeOut(const UObject* WorldContextObject);
};
