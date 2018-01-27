//
//  CCPlaySE.cpp
//  Cocos2dxNyangameTutorial
//
//  Created by Hata on 2018/01/22.
//

#include "CCPlaySE.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCPlaySE::CCPlaySE(std::string sound)
{
    m_sound = sound;
}

CCPlaySE* CCPlaySE::create(std::string sound)
{
    CCPlaySE* pRet = new CCPlaySE(sound);
    if (pRet)
    {
        pRet->autorelease();
    }

    return pRet;
}

// 毎フレーム呼ばれるメソッド.
void CCPlaySE::update(float time)
{
    // voidにキャストしているのは、コンパイラのwarning回避.
    CC_UNUSED_PARAM(time);
    SimpleAudioEngine::sharedEngine()->playEffect(m_sound.c_str());
}

// 逆のアニメーションを実装して返す.
CCFiniteTimeAction* CCPlaySE::reverse()
{
    // 今回は簡単のため、同じSEを生成して返す.
    return (CCFiniteTimeAction*)(CCPlaySE::create(m_sound));
}

// CCObject::copyを呼んだ際の内部実装として使われるメソッド.
// インスタンスのコピーのためには、このメソッドをオーバーライドする.
CCObject* CCPlaySE::copyWithZone(cocos2d::CCZone *pZone)
{
    CCZone* pNewZone = NULL;
    CCPlaySE* pRet = NULL;
    if (pZone && pZone->m_pCopyObject)
    {
        pRet = (CCPlaySE*) (pZone->m_pCopyObject);
    }
    else
    {
        pRet = new CCPlaySE(m_sound);
        pZone = pNewZone = new CCZone(pRet);
    }

    CCActionInstant::copyWithZone(pZone);
    CC_SAFE_DELETE(pNewZone);
    return pRet;
}
