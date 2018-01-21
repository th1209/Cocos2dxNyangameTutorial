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

public:
    CC_SYNTHESIZE_READONLY(kBlock, m_blockType, BlockType);

    BlockSprite();
    virtual ~BlockSprite();
    virtual bool initWithBlockType(kBlock blockTyoe);
    static BlockSprite* createWithBlockType(kBlock blockType);
};

#endif
