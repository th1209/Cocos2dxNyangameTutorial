//
//  GameScene.cpp
//  Cocos2dxNyangameTutorial
//
//  Created by Hata on 2018/01/21.
//

#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "BlockSprite.h"

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
    return true;
}

void GameScene::ccTouchEnded(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent)
{
    CCPoint touchPoint = m_background->convertTouchToNodeSpace(pTouch);
    
    int tag = 0;
    kBlock blockType;

    getTouchBlockTag(touchPoint, tag, blockType);
    if (tag != 0)
    {
        list<int> sameColorBlockTags = getSameColorBlockTags(tag, blockType);
        if (sameColorBlockTags.size() > 1)
        {
            removeBlock(sameColorBlockTags, blockType);
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
    list<int>::iterator it = blockTags.begin();
    while(it != blockTags.end())
    {
        m_blockTags[blockType].remove(*it);
        
        CCNode* block = m_background->getChildByTag(*it);
        if (block)
        {
            block->removeFromParentAndCleanup(true);
        }

        it++;
    }

    SimpleAudioEngine::sharedEngine()->playEffect(MP3_REMOVE_BLOCK);
}
