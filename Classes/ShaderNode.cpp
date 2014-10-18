#include "ShaderNode.h"

enum
{
    SIZE_X = 480,
    SIZE_Y = 320,
};

ShaderNode::ShaderNode()
    :m_center (vertex2 (0.0f, 0.0f))
    , m_resolution (vertex2 (0.0f, 0.0f))
    , m_time (0.0f)
    , m_uniformCenter (0)
    , m_uniformResolution (0)
    , m_uniformTime (0)
{
}

ShaderNode::~ShaderNode()
{

}

ShaderNode* ShaderNode::shaderNodeWithVertex (const char *vert, const char *frag)
{
    ShaderNode *node = new ShaderNode();
    node->initWithVertex (vert, frag);
    node->autorelease();

    return node;
}

bool ShaderNode::initWithVertex (const char *vert, const char *frag)
{

    loadShaderVertex (vert, frag);

    m_time = 0;
    m_resolution = vertex2 (SIZE_X, SIZE_Y);

    scheduleUpdate();

    setContentSize (CCSizeMake (SIZE_X, SIZE_Y));
    setAnchorPoint (ccp (0.5f, 0.5f));

    m_vertFileName = vert;
    m_fragFileName = frag;

    return true;
}

void ShaderNode::listenBackToForeground (CCObject *obj)
{
    this->setShaderProgram (NULL);
    loadShaderVertex (m_vertFileName.c_str(), m_fragFileName.c_str());
}

void ShaderNode::loadShaderVertex (const char *vert, const char *frag)
{
    CCGLProgram *shader = new CCGLProgram();
    shader->initWithVertexShaderFilename (vert, frag);

    shader->addAttribute ("aVertex", kCCVertexAttrib_Position);
    shader->link();

    shader->updateUniforms();

    m_uniformCenter = glGetUniformLocation (shader->getProgram(), "center");
    m_uniformResolution = glGetUniformLocation (shader->getProgram(), "resolution");
    m_uniformTime = glGetUniformLocation (shader->getProgram(), "time");

    this->setShaderProgram (shader);

    shader->release();
}

void ShaderNode::update (float dt)
{
    m_time += dt;
}

void ShaderNode::setPosition (const CCPoint &newPosition)
{
    CCNode::setPosition (newPosition);
    CCPoint position = getPosition();
    m_center = vertex2 (position.x * CC_CONTENT_SCALE_FACTOR(), position.y * CC_CONTENT_SCALE_FACTOR());
}

void ShaderNode::draw()
{
    CC_NODE_DRAW_SETUP();

    float w = SIZE_X, h = SIZE_Y;
    GLfloat vertices[12] = { 0, 0, w, 0, w, h, 0, 0, 0, h, w, h };

    //
    // Uniforms
    //
    getShaderProgram()->setUniformLocationWith2f (m_uniformCenter, m_center.x, m_center.y);
    getShaderProgram()->setUniformLocationWith2f (m_uniformResolution, m_resolution.x, m_resolution.y);

    // time changes all the time, so it is Ok to call OpenGL directly, and not the "cached" version
    glUniform1f (m_uniformTime, m_time);

    ccGLEnableVertexAttribs (kCCVertexAttribFlag_Position);

    glVertexAttribPointer (kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, vertices);

    glDrawArrays (GL_TRIANGLES, 0, 6);

    CC_INCREMENT_GL_DRAWS (1);
}

