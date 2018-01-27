//
//  GameScene.cpp
//  Cocos2dxNyangameTutorial
//
//  Created by Hata on 2018/01/21.
//

#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "BlockSprite.h"
#include "CCPlaySE.h"

using namespace cocos2d;
using namespace CocosDenshion;
using namespace std;

CCScene* GameScene::scene()
{
    CCScene* scene = CCScene::create();
    GameScene* layer = GameScene::create();
    scene->addChild(layer);
    return scene;
}

bool GameScene::init()
{
    if (!CCLayer::init())
    {
        return false;
    }

    // 以下コードで、タップイベントが有効になる.
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);

    initForVariables();

    showBackGround();
    showBlock();

    showLabel();

    // 効果音の事前読み込み.
    SimpleAudioEngine::sharedEngine()->preloadEffect(MP3_REMOVE_BLOCK);

    return true;
}

void GameScene::showBackGround()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    m_background = CCSprite::create(PNG_BACKGROUND);
    m_background->setPosition(ccp(winSize.width / 2, winSize.height / 2));
    addChild(m_background, kZOrderBackground, kTagBackground);
}

void GameScene::initForVariables()
{
    // 乱数の初期化。
    srand((unsigned)time(NULL));

    BlockSprite* pBlock = BlockSprite::createWithBlockType(kBlockRed);
    m_blockSize = pBlock->getContentSize().height;

    blockTypes.push_back(kBlockRed);
    blockTypes.push_back(kBlockBlue);
    blockTypes.push_back(kBlockYellow);
    blockTypes.push_back(kBlockGreen);
    blockTypes.push_back(kBlockGray);

    m_animating = false;
    m_score = 0;
}

CCPoint GameScene::getPosition(int posIndexX, int posIndexY)
{
    float offsetX = m_background->getContentSize().width * 0.168;
    float offsetY = m_background->getContentSize().height * 0.029;
    return CCPoint(
        (posIndexX + 0.5) * m_blockSize + offsetX,
        (posIndexY + 0.5) * m_blockSize + offsetY);
}

int GameScene::getTag(int posIndexX, int posIndexY)
{
    return kTagBaseBlock + posIndexX * 100 + posIndexY;
}

void GameScene::showBlock()
{
    for (int x = 0; x < MAX_BLOCK_X; x++)
    {
        for (int y = 0; y < MAX_BLOCK_Y; y++)
        {
            kBlock blockType = (kBlock)(rand() % kBlockCount);

            int tag = getTag(x, y);
            m_blockTags[blockType].push_back(tag);

            BlockSprite* pBlock = BlockSprite::createWithBlockType(blockType);
            pBlock->setPosition(getPosition(x, y));
            m_background->addChild(pBlock, kZOrderBlock, tag);
        }
    }
}

bool GameScene::ccTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent)
{
    // ccTouchBeganでtrueを返すと、他のタップイベント系メソッドが有効になる。
    return !m_animating;
}

void GameScene::ccTouchEnded(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent)
{
    CCPoint touchPoint = m_background->convertTouchToNodeSpace(pTouch);
    
    int tag = 0;
    kBlock blockType;
    CCLog("x:%f, y:%f", touchPoint.x, touchPoint.y);

    getTouchBlockTag(touchPoint, tag, blockType);
    if (tag != 0)
    {
        list<int> sameColorBlockTags = getSameColorBlockTags(tag, blockType);
        if (sameColorBlockTags.size() > 1)
        {
            m_score += pow(sameColorBlockTags.size() - 2, 2);

            m_animating = true;

            // 隣接するコマの削除.
            removeBlock(sameColorBlockTags, blockType);

            // 他のブロックの値の更新 & アニメーション諸々.
            movingBlockAnimation1(sameColorBlockTags);
        }
    }
}

// タッチされた場所に対応するBlockを見つける。
// tagとblockTypeを参照渡しで更新する。
void GameScene::getTouchBlockTag(CCPoint touchPoint, int& tag, kBlock& blockType)
{
    for (int x = 0; x < MAX_BLOCK_X; x++)
    {
        for (int y = 0; y < MAX_BLOCK_Y; y++)
        {
            int currentTag = getTag(x, y);
            CCNode* node = m_background->getChildByTag(currentTag);
            if (node && node->boundingBox().containsPoint(touchPoint))
            {
                tag = currentTag;
                // ↓getBlockTypeの定義は子にしかないので、キャストする必要がある.
                blockType = ((BlockSprite*)node)->getBlockType();

                return;
            }
        }
    }
}

// 第二引数で指定したタグがlistに含まれるか確かめる。
bool GameScene::hasSameColorBlock(std::list<int> blockTagList, int searchBlockTag)
{
    list<int>::iterator it;
    for (it = blockTagList.begin(); it != blockTagList.end(); ++it)
    {
        if (*it == searchBlockTag)
        {
            return true;
        }
    }

    return false;
}

// 同じ色のブロックのリストを返す(引数に渡したブロックも含まれる)。
list<int> GameScene::getSameColorBlockTags(int baseTag, kBlock blockType)
{
    list<int> sameColorBlockTags;
    sameColorBlockTags.push_back(baseTag);
    
    list<int>::iterator it = sameColorBlockTags.begin();
    while (it != sameColorBlockTags.end())
    {
        int tags[] = {
            *it + 100,
            *it - 100,
            *it + 1,
            *it - 1,
        };
        
        for (int i = 0; i < sizeof(tags) / sizeof(tags[0]); i++)
        {
            if (hasSameColorBlock(sameColorBlockTags, tags[i]))
            {
                // 既にリストに含まれている場合.
                continue;
            }

            if (hasSameColorBlock(m_blockTags[blockType], tags[i]))
            {
                sameColorBlockTags.push_back(tags[i]);
            }
        }

        it++;
    }

    return sameColorBlockTags;
}

void GameScene::removeBlock(list<int> blockTags, kBlock blockType)
{
    bool first = true;

    list<int>::iterator it = blockTags.begin();
    while(it != blockTags.end())
    {
        // タグ配列から削除.
        m_blockTags[blockType].remove(*it);

        CCNode* block = m_background->getChildByTag(*it);
        if (block)
        {
            // ブロックの縮小アニメーション、実際のブロック削除アクションを作成.
            CCScaleTo* scale = CCScaleTo::create(REMOVING_TIME, 0);
            CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(GameScene::removingBlock));

            // 各アニメーションを連結(順次再生).
            CCFiniteTimeAction* sequence = CCSequence::create(scale, func, NULL);

            CCFiniteTimeAction* action;
            if (first)
            {
                // SE再生アニメーションを連結(並列再生).
                CCPlaySE* playSe = CCPlaySE::create(MP3_REMOVE_BLOCK);
                action = CCSpawn::create(sequence, playSe, NULL);

                first = false;
            }
            else
            {
                action = sequence;
            }

            block->runAction(action);
        }

        it++;
    }

    SimpleAudioEngine::sharedEngine()->playEffect(MP3_REMOVE_BLOCK);
}

void GameScene::removingBlock(CCNode* block)
{
    block->removeFromParentAndCleanup(true);
}

GameScene::PositionIndex GameScene::getPositionIndex(int tag)
{
    int pos1_x = (tag - kTagBaseBlock) / 100;
    int pos1_y = (tag - kTagBaseBlock) % 100;
    return PositionIndex(pos1_x, pos1_y);
}

// y方向にブロックの位置をデクリメントする.
// tag: 移動対象のブロックのタグ
// posIndex: 移動対象のブロックの位置(移動前)
void GameScene::setNewPosition1(int tag, PositionIndex posIndex)
{
    BlockSprite* blockSprite = (BlockSprite*)m_background->getChildByTag(tag);
    int nextPosY = blockSprite->getNextPosY();
    if (nextPosY == -1)
    {
        nextPosY = posIndex.y;
    }

    blockSprite->setNextPos(posIndex.x, --nextPosY);
}

// ブロックの削除時に、各ブロックのy方向の位置を更新する.
// blocks: 削除対象のブロック.
void GameScene::searchNewPosition1(std::list<int> blocks)
{
    list<int>::iterator it1 = blocks.begin();
    while(it1 != blocks.end())
    {
        PositionIndex posIndex1 = getPositionIndex(*it1);

        vector<kBlock>::iterator it2 = blockTypes.begin();
        while(it2 != blockTypes.end())
        {
            // 色ごとに、全てのブロックを走査する.
            list<int>::iterator it3 = m_blockTags[*it2].begin();
            while(it3 != m_blockTags[*it2].end())
            {
                PositionIndex posIndex2 = getPositionIndex(*it3);
                if (posIndex1.x == posIndex2.x && posIndex1.y < posIndex2.y)
                {
                    setNewPosition1(*it3, posIndex2);
                }
                it3++;
            }
            it2++;
        }
        it1++;
    }
}

// 以下を行う.
//  1. m_blockTagsの値の更新
//  2. 各ブロックのタグの更新、NextPosの最初期化
//  3. 各ブロックの移動アニメーション
void GameScene::moveBlock()
{
    vector<kBlock>::iterator it1 = blockTypes.begin();
    while (it1 != blockTypes.end())
    {
        list<int>::iterator it2 = m_blockTags[*it1].begin();
        while (it2 != m_blockTags[*it1].end())
        {
            BlockSprite* blockSprite = (BlockSprite*) m_background->getChildByTag(*it2);

            int nextPosX = blockSprite->getNextPosX();
            int nextPosY = blockSprite->getNextPosY();

            if (nextPosX != -1 || nextPosY != -1)
            {
                // ブロックの情報を更新.
                // (いい加減 変更に脆いコードなので何とかしたい...)
                int newTag = getTag(nextPosX, nextPosY);
                blockSprite->initNextPos();
                blockSprite->setTag(newTag);

                // タグ配列の要素を更新.
                *it2 = newTag;

                // 移動アニメーション.
                CCMoveTo* move = CCMoveTo::create(MOVING_TIME, getPosition(nextPosX, nextPosY));
                blockSprite->runAction(move);
            }
            it2++;
        }
        it1++;
    }
}

void GameScene::movingBlockAnimation1(std::list<int> blocks)
{
    searchNewPosition1(blocks);
    moveBlock();
    scheduleOnce(schedule_selector(GameScene::movingBlockAnimation2), MOVING_TIME);
}

void GameScene::movedBlocks()
{
    // ラベルの更新.
    showLabel();

    m_animating = false;
    
    if (! existsSameBlock())
    {
        // ゲームオーバー表示.
        CCSize bgSize = m_background->getContentSize();
        CCSprite* gameOver = CCSprite::create(PNG_GAMEOVER);
        gameOver->setPosition(ccp(bgSize.width / 2, bgSize.height * 0.8));
        m_background->addChild(gameOver, kZOrderGameOver, kTagGameOver);
        setTouchEnabled(false);
    }
}

// x方向にブロックの位置をデクリメントする.
// tag: 移動対象のブロックのタグ
// posIndex: 移動対象のブロックの位置(移動前)
void GameScene::setNewPosition2(int tag, PositionIndex posIndex)
{
    BlockSprite* blockSprite = (BlockSprite*)m_background->getChildByTag(tag);
    int nextPosX = blockSprite->getNextPosX();
    if (nextPosX == -1)
    {
        nextPosX = posIndex.x;
    }

    blockSprite->setNextPos(--nextPosX, posIndex.y);
}

// keyに列番号、valueにブロックが1つでも存在するかを表すmapを返す.
std::map<int, bool> GameScene::getExistsBlockColumn()
{
    // mapの初期化.
    map<int, bool> xBlock;
    for (int i = 0; i < MAX_BLOCK_X; i++) {
        xBlock[i] = false;
    }

    vector<kBlock>::iterator itBlockType = blockTypes.begin();
    while (itBlockType != blockTypes.end())
    {
        list<int>::iterator itTag = m_blockTags[*itBlockType].begin();
        while (itTag != m_blockTags[*itBlockType].end())
        {
            // ブロックが1つでも存在する列はtrueにする.
            xBlock[getPositionIndex(*itTag).x] = true;
            itTag++;
        }
        itBlockType++;
    }

    return xBlock;
}

void GameScene::searchNewPosition2()
{
    map<int, bool> xBlock = getExistsBlockColumn();
    
    bool first = true;
    
    for (int i = MAX_BLOCK_X - 1 ; i >= 0; i--)
    {
        // 右の列から走査する.

        if (xBlock[i])
        {
            // ブロックが存在する列.
            
            first = false;
            continue;
        }
        else
        {
            // ブロックが存在しない列.

            if (first)
            {
                // 1番右の列なら何もしない.
                continue;
            }
            else
            {
                vector<kBlock>::iterator itBlockType = blockTypes.begin();
                while (itBlockType != blockTypes.end())
                {
                    list<int>::iterator itTag = m_blockTags[*itBlockType].begin();
                    while (itTag != m_blockTags[*itBlockType].end())
                    {
                        PositionIndex posIndex = getPositionIndex(*itTag);
                        if (i < posIndex.x)
                        {
                            // この列より右にあるブロックなら位置を更新.
                            setNewPosition2(*itTag, posIndex);
                        }

                        itTag++;
                    }
                    itBlockType++;
                }
            }
        }
    }
}

void GameScene::movingBlockAnimation2()
{
    searchNewPosition2();
    moveBlock();
    scheduleOnce(schedule_selector(GameScene::movedBlocks), MOVING_TIME);
}

void GameScene::showLabel()
{
    // ブロック個数ラベル.
    CCSize bgSize = m_background->getContentSize();
    
    int tagsForLabel[] = {
        kTagRedLabel,
        kTagBlueLabel,
        kTagYellowLabel,
        kTagGreenLabel,
        kTagGrayLabel,
    };
    const char* fontNames[] = {
        FONT_RED,
        FONT_BLUE,
        FONT_YELLOW,
        FONT_GREEN,
        FONT_GRAY,
    };
    float heightRate[] = {0.61, 0.51, 0.41, 0.31, 0.21};
    
    vector<kBlock>::iterator it = blockTypes.begin();
    while (it != blockTypes.end())
    {
        int count = m_blockTags[*it].size();
        const char* countStr = ccsf("%02d", count);
        CCLabelBMFont* label = (CCLabelBMFont*)m_background->getChildByTag(tagsForLabel[*it]);

        if (!label)
        {
            // ラベルの新規生成.
            label = CCLabelBMFont::create(countStr, fontNames[*it]);
            label->setPosition(ccp(bgSize.width * 0.78, bgSize.height * heightRate[*it]));
            m_background->addChild(label, kZOrderLabel, tagsForLabel[*it]);
        }
        else
        {
            // ラベルの更新.
            label->setString(countStr);
        }
        it++;
    }

    // スコアラベル.
    const char* scoreStr = ccsf("%d", m_score);
    CCLabelBMFont* scoreLabel = (CCLabelBMFont*)m_background->getChildByTag(kTagScoreLabel);
    if (!scoreLabel)
    {
        scoreLabel = CCLabelBMFont::create(scoreStr, FONT_WHITE);
        scoreLabel->setPosition(ccp(bgSize.width * 0.78, bgSize.height * 0.75));
        m_background->addChild(scoreLabel, kZOrderLabel, kTagScoreLabel);
    }
    else
    {
        scoreLabel->setString(scoreStr);
    }
}

// 残りブロック数の判定.
bool GameScene::existsSameBlock()
{
    // 全ブロックについて走査
    vector<kBlock>::iterator it1 = blockTypes.begin();
    while (it1 != blockTypes.end())
    {
        list<int>::iterator it2 = m_blockTags[*it1].begin();
        while (it2 != m_blockTags[*it1].end())
        {
            if (getSameColorBlockTags(*it2, *it1).size() > 1)
            {
                // 隣り合うコマが見つかった時点でtrueを返す.
                return true;
            }
            it2++;
        }
        it1++;
    }
    
    return false;
}
