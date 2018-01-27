//
//  Config.h
//  Cocos2dxNyangameTutorial
//
//  Created by Hata on 2018/01/21.
//

#ifndef __CONFIG_H__
#define __CONFIG_H__

enum kBlock
{
    kBlockRed,
    kBlockBlue,
    kBlockYellow,
    kBlockGreen,
    kBlockGray,
    // ↓enumが0始まりである仕様を利用したイディオム.
    kBlockCount,
};

// 可変引数マクロ.
#define ccsf(...) CCString::createWithFormat(__VA_ARGS__)->getCString();

#endif
