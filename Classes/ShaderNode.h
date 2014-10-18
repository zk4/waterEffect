#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;
using namespace cocos2d;

class ShaderNode : public CCNode
{
public:
    ShaderNode();
    ~ShaderNode();

    bool initWithVertex (const char *vert, const char *frag);
    void loadShaderVertex (const char *vert, const char *frag);
    void listenBackToForeground (CCObject *obj);

    virtual void update (float dt);
    virtual void setPosition (const CCPoint &newPosition);
    virtual void draw();

    static ShaderNode* shaderNodeWithVertex (const char *vert, const char *frag);

private:

    ccVertex2F m_center;
    ccVertex2F m_resolution;
    float      m_time;
    GLuint     m_uniformCenter, m_uniformResolution, m_uniformTime;
    std::string m_vertFileName;
    std::string m_fragFileName;
};