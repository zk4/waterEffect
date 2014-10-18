#pragma once

#include "cocos2d.h"
#include "cocos-ext.h"
#include <vector>

using namespace std;
using namespace cocos2d;
using namespace cocos2d::extension;

//core macro
#define    _LEFT Layout2::__Left
#define    _RIGHT Layout2::__Right
#define    _TOP Layout2::__Top
#define    _BOTTOM Layout2::__Buttom
#define    _CENTER Layout2::__Center
#define    _MOVE Layout2::__Move
#define    _MIDDLE Layout2::__Middle
//helper macro
#define    _MLT(x,y,z,b)  _MOVE(ccp(x,y),_TOP(1,z,_LEFT(0,z,b)))
#define    _MRT(x,y,z,b)  _MOVE(ccp(x,y),_TOP(1,z,_RIGHT(1,z,b)))
#define    _MRB(x,y,z,b)  _MOVE(ccp(x,y),_BOTTOM(0,z,_RIGHT(1,z,b)))
#define    _MLC(x,y,z,b)  _MOVE(ccp(x,y),_LEFT(0,z,_CENTER(z,b)))
#define    _MRC(x,y,z,b)  _MOVE(ccp(x,y),_RIGHT(1,z,_CENTER(z,b)))
#define    _MBC(x,y,z,b)  _MOVE(ccp(x,y),_BOTTOM(0,z,_CENTER(z,b)))

#define    _DEBUG_LAYOUT(x)  DebugDrawLayer::Get()->AddNode(x);
#define    _DEBUG_LAYOUT2(x)  DebugDrawLayer::Get()->AddRect(x);
class DebugDrawLayer : public CCLayer
{
    static DebugDrawLayer* s_drawLayer;
public:
    static int s_id;

    struct _AdvancedRect
    {

        int stack_count;
        CCRect r;
        float  line_base_width;
        float  remain_secs;
        ccColor4F  color;

    };
    bool init();
    CREATE_FUNC ( DebugDrawLayer );
    static void LazyInit();
    static DebugDrawLayer*  Get();
    void draw();

    void AddRect ( CCRect  r, ccColor4F color = ccc4f ( 1, 1, 1, 1 ) );
    void AddNode ( CCNode* n, ccColor4F color = ccc4f ( 1, 1, 1, 1 ) );

    void ClearAllRects();
    void Update ( float d );

    vector<_AdvancedRect>   _needs_draw;

private:
    void DrawRect ( CCRect r, int width, ccColor4F c );
};
class Layout2
{
public:
    enum eAlign
    {
        LEFT = 1,
        RIGHT = 1 << 1,
        CENTER = 1 << 2,
        TOP = 1 << 3,
        BUTTOM = 1 << 4,

        MOVE = 1 << 5
    };



    static CCNode* Align ( CCNode* target1, CCNode* follow, eAlign align, CCPoint  follow_anchor_or_movement, CCPoint  target_anchor = ccp ( 0.5, 0.5 ) );
    static CCNode* __Left ( float anchorX, CCNode* target, CCNode* follow );
    static CCNode* __Right ( float anchorX,  CCNode* target, CCNode* follow );
    static CCNode* __Top ( float anchorY,    CCNode* target, CCNode* follow );
    static CCNode* __Buttom ( float anchorY,   CCNode* target, CCNode* follow );
    static CCNode* __Center ( CCNode* target, CCNode* follow, CCPoint  follow_anchor = ccp ( 0.5, 0.5 ), CCPoint  target_anchor = ccp ( 0.5, 0.5 ) );
    static CCNode* __Middle ( CCNode* target1, CCNode* target2, CCNode* follow );
    static CCNode* __Move ( CCPoint  movement , CCNode* target );

    static CCPoint  __Ruler ( CCNode* to, CCNode* from, CCPoint  from_anchor = ccp ( 0.5, 0.5 ), CCPoint  to_anchor = ccp ( 0.5, 0.5 ) );
    static void GetWorldRect ( CCNode* n, CCPoint& w_left_bottom, CCPoint& w_right_top, CCPoint& w_center );
    static void GetWorldRect ( CCNode* n, CCRect& o_rect/*out*/ );
    static void __DebugLayout ( CCNode* n );
    static CCSize GetContentSize ( CCNode* n );

};

