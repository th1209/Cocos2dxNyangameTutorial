//
//  CCPlaySE.h
//  Cocos2dxNyangameTutorial
//
//  Created by Hata on 2018/01/22.
//

#ifndef __CCPLAYSE_H__
#define __CCPLAYSE_H__

#include "cocos2d.h"

class CCPlaySE : public cocos2d::CCActionInstant
{
public:
    CCPlaySE(std::string sound);
    virtual ~CCPlaySE() {}

    virtual void update(float time);
    virtual CCFiniteTimeAction* reverse(void);
    virtual CCObject* copyWithZone(cocos2d::CCZone *pZone);

public:
    static CCPlaySE* create(std::string sound);

protected:
    std::string m_sound;
};

#endif /* CCPlaySE_h */
