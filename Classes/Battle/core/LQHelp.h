//
//  LQHelp.h
//  game
//
//  Created by lequdev1 on 14-7-26.
//
//

#ifndef __game__LQHelp__
#define __game__LQHelp__

#include <iostream>

#include "cocos2d.h"
#include "extensions/cocos-ext.h"


USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

#include "LQStringExtend.h"

static Scale9Sprite* createItemWithFileName(std::string& filename, int quality, int number, float scale = 1,float fontSize = 30, Color3B fontcolor = Color3B::YELLOW)
{
    
    std::string filePath = FileUtils::getInstance()->fullPathForFilename(filename.c_str());
    Sprite* iconsp = Sprite::create(filePath.c_str());
    iconsp->setScale(scale);
    
    std::string qualityStr;
    stringf(qualityStr, "qualitybg_%d.png",quality);
    Scale9Sprite* bg = Scale9Sprite::createWithSpriteFrameName(qualityStr);
    bg->setPreferredSize(iconsp->getContentSize() * scale*1.2);
    
    iconsp->setPosition(Point(bg->getContentSize()*0.5));
    
    bg->addChild(iconsp);
    
    std::string numberStr;
    stringf(numberStr, "X%d",number);
    Label* numberLabel = Label::createWithSystemFont(numberStr,"Helvetica", fontSize);
    numberLabel->setPosition(Point(iconsp->getContentSize().width+10,0));
    numberLabel->enableOutline(Color4B(0, 0, 0, 255),2);
    numberLabel->setAnchorPoint(Point(1,0));
    
    bg->addChild(numberLabel);
    numberLabel->setColor(fontcolor);
    numberLabel->setVisible(number > 0);
   
    return bg;
}

#endif /* defined(__game__LQHelp__) */
