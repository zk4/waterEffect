#include "HelloWorldScene.h"
#include "Layout2.h"

#include "ShaderNode.h"
USING_NS_CC;

#define		RESOLUTION 64

static GLuint	texture;



CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild (layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{

    if (!CCLayerColor::initWithColor ({255,255,255,255}))
    {
        return false;
    }

    auto water = CCSprite::create ("noise.jpg");
    addChild (water);

    _CENTER (this, water);

    auto shader = ShaderNode::shaderNodeWithVertex ("Shaders/water.vsh", "Shaders/water.fsh");
    addChild (shader);
    _CENTER (this, shader);

    scheduleUpdate();
    return true;
}

