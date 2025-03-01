#include "HW_CheatManager.h"
#include "MyFunctionLibrary.h"

UHW_CheatManager::UHW_CheatManager()
{
}

UHW_CheatManager::~UHW_CheatManager()
{
}

void UHW_CheatManager::StartFadeOut()
{
	UMyFunctionLibrary::StartFadeOut(this); // 인자로 UObjcet를 넘겨줘야함 앞에 U가 붙은 아무 객체.
}
