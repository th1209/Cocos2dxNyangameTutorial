//
//  GameScene.hpp
//  Cocos2dxNyangameTutorial
//
//  Created by Hata on 2018/01/21.
//

#ifndef __GAMESCENE__H__
#define __GAMESCENE__H__

#include "cocos2d.h"
#include "Config.h"

# define PNG_BACKGROUND "background.png"
# define MP3_REMOVE_BLOCK "removeBlock.mp3"

# define MAX_BLOCK_X 8
# define MAX_BLOCK_Y 8

class GameScene : public cocos2d::CCLayer
{
protected:
    enum kTag
    {
        kTagBackground = 1,
        kTagBaseBlock = 10000,
    };
    
    enum kZOrder
    {
        kZOrderBackground,
        kZOrderBlock,
    };

    // 背景に関するメンバ.
    cocos2d::CCSprite* m_background;
    void showBackGround();

    // ブロックに関するメンバ.
    float m_blockSize;
    std::map<kBlock, std::list<int> > m_blockTags;
    void initForVariables();
    void showBlock();
    cocos2d::CCPoint getPosition(int posIndexX, int posIndexY);
    int getTag(int posIndexX, int posIndexY);

    // タッチイベントに関するメンバ.
    void getTouchBlockTag(cocos2d::CCPoint touchPoint, int &tag, kBlock &blockType);
    std::list<int> getSameColorBlockTags(int baseTag, kBlock blockType);
    void removeBlock(std::list<int> blockTags, kBlock blockType);
    bool hasSameColorBlock(std::list<int> blockTagList, int searchBlockTag);

public:
    virtual bool init();
    static cocos2d::CCScene* scene();
    CREATE_FUNC(GameScene);

    virtual bool ccTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
    virtual void ccTouchEnded(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
};

#endif
