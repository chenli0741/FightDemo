//
//  maskScene.h
//  layerColor
//
//  Created by HelloWorld on 13-12-12.
//
//

#ifndef __layerColor__maskScene__
#define __layerColor__maskScene__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;
using namespace std;
class MaskLayer:public Layer{
public:
//    static Scene * scene();
    MaskLayer();
    ~MaskLayer();
    static MaskLayer* createMaskLayer();
    bool initMask();
    
    void setRectPoint(Point pt);
    void setRectScale(float scale);
    void setStencilSprite(string filename);
    void onEnter();
    void onExit();
    
private:
    ClippingNode* m_clipperNode;
    LayerColor* m_colorLayer;
    Sprite* m_touchNode;
};
#endif /* defined(__layerColor__maskScene__) */
