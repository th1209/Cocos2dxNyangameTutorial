//
//  BlockSprite.hpp
//  Cocos2dxNyangameTutorial
//
//  Created by Hata on 2018/01/21.
//

#ifndef __BLOCK_SPRITE__H__
#define __BLOCK_SPRITE__H__

#include "cocos2d.h"
#include "Config.h"

class BlockSprite : public cocos2d::CCSprite
{
protected:
    const char* getBlockImageFileName(kBlock blockType);
    virtual bool initWithBlockType(kBlock blockTyoe);

public:
    CC_SYNTHESIZE_READONLY(kBlock, m_blockType, BlockType);
    CC_SYNTHESIZE_READONLY(int, m_nextPosX, NextPosX);
    CC_SYNTHESIZE_READONLY(int, m_nextPosY, NextPosY);
    
    void initNextPos();
    void setNextPos(int nextPosX, int nextPosY);

    BlockSprite();
    virtual ~BlockSprite();
    static BlockSprite* createWithBlockType(kBlock blockType);
};

#endif
