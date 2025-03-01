#include "PatternLibrary.h"
#include "BossCharacter.h"
//#include "Attack1.h"

UPatternLibrary::UPatternLibrary()
{
    // 생성할 때, Attack1 클래스 가져와서 할당해주면될듯
}


void UPatternLibrary::BeginPlay()
{
    Super::BeginPlay();

    //// 예시코드임다 - 공격 객체들 생성 (UObject로 가정한다고 하네요 이부분은 지피티형님이 알려줌)
    //if (!Attack1)
    //{
    //    Attack1 = NewObject<UBossAttack1>(this, UBossAttack1::StaticClass());
    //    Attack1->InitializeAttack(this);
    //}
}

//void UPatternLibrary::PerformAttack1()
//{
//    // 예시 코드
//    if (attack1)
//    {
//        abosscharacter* bossowner = cast<abosscharacter>(getowner());
//        attack1->executeattack(bossowner);
//    }
//}

