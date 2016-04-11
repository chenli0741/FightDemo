//
//  maskScene.cpp
//  layerColor
//
//  Created by HelloWorld on 13-12-12.
//
//

#include "MaskLayer.h"

#define KLQStencilNodeTagNumber 100
#define KLQMaskLayerColor 0
#define KLQMaskSceneInterfaceTagNumber 1000
#define LayerColorOpacity  100
MaskLayer::MaskLayer():m_touchNode(NULL),m_clipperNode(NULL),m_colorLayer(NULL)
{

}
MaskLayer::~MaskLayer()
{

}
MaskLayer* MaskLayer::createMaskLayer()
{
    MaskLayer* layer = new MaskLayer();
    if (layer && layer->initMask()) {
        layer->autorelease();
    }else{
        CC_SAFE_DELETE(layer);
    }
    
    return layer;
}
bool MaskLayer::initMask()
{
    Size size =Director::getInstance()->getVisibleSize();
    
    m_clipperNode = ClippingNode::create();
    m_clipperNode->setInverted(true);
    m_clipperNode->setAlphaThreshold(0.0f);
    addChild(m_clipperNode);
    
    m_colorLayer = LayerColor::create(Color4B(KLQMaskLayerColor, KLQMaskLayerColor, KLQMaskLayerColor, LayerColorOpacity), size.width, size.height);
    m_clipperNode->addChild(m_colorLayer,1);

    return true;
}
void MaskLayer::setStencilSprite(string filename)
{
    if (filename == "") {
        return;
    }
    Sprite* stencil = Sprite::create(filename);
    m_clipperNode->setStencil(stencil);
    
    if (m_touchNode) {
        m_touchNode->removeFromParent();
    }
    m_touchNode = Sprite::create(filename);
    this->addChild(m_touchNode);
    m_touchNode->setOpacity(LayerColorOpacity);
}
void MaskLayer::setRectPoint(Point pt)
{
    if (!m_clipperNode->getStencil()) {
        return;
    }
    m_clipperNode->getStencil()->setPosition(pt);
    if (m_touchNode) {
        m_touchNode->setPosition(pt);
    }
}
void MaskLayer::setRectScale(float scale)
{
    if (!m_clipperNode->getStencil()) {
        return;
    }
    m_clipperNode->getStencil()->setScale(scale);
    if (m_touchNode) {
        m_touchNode->setScale(scale);
    }
}
void MaskLayer::onEnter()
{
    Layer::onEnter();
}
void MaskLayer::onExit()
{
    Layer::onExit();
}