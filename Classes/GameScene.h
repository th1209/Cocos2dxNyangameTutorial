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

# define FONT_RED "redFont.fnt"
# define FONT_BLUE "blueFont.fnt"
# define FONT_YELLOW "yellowFont.fnt"
# define FONT_GREEN "greenFont.fnt"
# define FONT_GRAY "grayFont.fnt"
# define FONT_WHITE "whiteFont.fnt"

# define MAX_BLOCK_X 8
# define MAX_BLOCK_Y 8

# define REMOVING_TIME 0.1f
# define MOVING_TIME 0.2f


class GameScene : public cocos2d::CCLayer
{
protected:
    enum kTag
    {
        kTagBackground = 1,
        kTagRedLabel,
        kTagBlueLabel,
        kTagYellowLabel,
        kTagGreenLabel,
        kTagGrayLabel,
        kTagScoreLabel,
        kTagBaseBlock = 10000,
    };
    
    enum kZOrder
    {
        kZOrderBackground,
        kZOrderLabel,
        kZOrderBlock,
    };
    
    struct PositionIndex
    {
        PositionIndex(int x1, int y1)
        {
            x = x1;
            y = y1;
        }

        int x;
        int y;
    };

    // 背景に関するメンバ変数.
    cocos2d::CCSprite* m_background;

    // ブロックに関するメンバ変数.
    float m_blockSize;
    std::map<kBlock, std::list<int> > m_blockTags;
    std::vector<kBlock> blockTypes;

    bool m_animating;

    int m_score;

    // 初期化に関するメンバ関数.
    void initForVariables();
    void showBackGround();
    void showBlock();

    // ブロックのタッチ・削除に関するメンバ関数.
    cocos2d::CCPoint getPosition(int posIndexX, int posIndexY);
    int getTag(int posIndexX, int posIndexY);
    void getTouchBlockTag(cocos2d::CCPoint touchPoint, int &tag, kBlock &blockType);
    std::list<int> getSameColorBlockTags(int baseTag, kBlock blockType);
    void removeBlock(std::list<int> blockTags, kBlock blockType);
    bool hasSameColorBlock(std::list<int> blockTagList, int searchBlockTag);
    void removingBlock(cocos2d::CCNode* block);

    // ブロックの移動に関するメンバ関数
    PositionIndex getPositionIndex(int tag);
    void setNewPosition1(int tag, PositionIndex posIndex);
    void searchNewPosition1(std::list<int> blocks);
    void moveBlock();
    void movingBlockAnimation1(std::list<int> blocks);
    void movedBlocks();
    std::map<int, bool> getExistsBlockColumn();
    void setNewPosition2(int tag, PositionIndex posIndex);
    void searchNewPosition2();
    void movingBlockAnimation2();

    void showLabel();


public:
    virtual bool init();
    static cocos2d::CCScene* scene();
    CREATE_FUNC(GameScene);

    virtual bool ccTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
    virtual void ccTouchEnded(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
};

#endif
