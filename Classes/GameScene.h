//
//  GameScene.hpp
//  Cocos2dxNyangameTutorial
//
//  Created by Hata on 2018/01/21.
//

#ifndef __GAMESCENE__H__
#define __GAMESCENE__H__

#include "cocos2d.h"

# define PNG_BACKGROUND "background.png"

class GameScene : public cocos2d::CCLayer
{
protected:
    enum kTag
    {
        kTagBackground = 1,
    };
    
    enum kZOrder
    {
        kZOrderBackground,
    };
    
    cocos2d::CCSprite* m_background;
    void showBackGround();
    
public:
    virtual bool init();
    static cocos2d::CCScene* scene();
    CREATE_FUNC(GameScene);
};

#endif
