
#include "Layout2.h"
#include <assert.h>

CCSize Layout2::GetContentSize ( CCNode* n )
{
    CCSize size;
    if ( dynamic_cast<CCScrollView*> ( n ) )
    {
        size = dynamic_cast<CCScrollView*> ( n )->getViewSize();
    }
    else if ( dynamic_cast<CCLabelTTF*> ( n ) )
    {
        CCLabelTTF* t = dynamic_cast<CCLabelTTF*> ( n );
        size = t->getTexture()->getContentSize();
        if ( size.height == 0 )
        {
            size.height = t->getFontSize();
        }
    }
    else
    {
        size = n->getContentSize();
    }

    if ( size.width == 0 )
    {
        size.width = 1;
    }
    if ( size.height == 0 )
    {
        size.height = 1;
    }
    // assert (size.width != 0 && size.height != 0 );
    return size;
}

void  Layout2::GetWorldRect ( CCNode* n, CCPoint& w_left_bottom, CCPoint& w_right_top, CCPoint& w_center )
{
    CCRect  r;
    GetWorldRect ( n, r );
    w_left_bottom = r.origin;
    w_right_top = r.origin + r.size;
    w_center = r.origin + r.size / 2;
}

void Layout2::GetWorldRect ( CCNode* n, CCRect& o_rect/*out*/ )
{
    CCSize size = GetContentSize ( n );
    auto w_matrix = n->nodeToWorldTransform();
    size = CCSizeApplyAffineTransform ( size, w_matrix );
    o_rect.origin = ccp ( w_matrix.tx, w_matrix.ty );
    o_rect.size = size;

}


CCNode* Layout2::Align ( CCNode* target, CCNode* follower, eAlign align, CCPoint follow_anchor_or_movement , CCPoint  target_anchor /*= ccp(0.5, 0.5)*/ )
{
    CCPoint w_target_lb, w_target_rt, w_target_center;
    GetWorldRect ( target, w_target_lb, w_target_rt, w_target_center );
    CCSize followsize = GetContentSize ( follower );

    //get follower world pos
    CCPoint	w_from_pos = follower->convertToWorldSpace ( ccp ( followsize.width * ( follow_anchor_or_movement.x ), followsize.height * ( follow_anchor_or_movement.y ) ) );

#define ALIGN_ACCURACY 2000.0f
    //get scale from cascades,because  scale is accumulated through child hierarchy,move ccp(ALIGN_ACCURACY,ALIGN_ACCURACY) offset,then move back, get the scale from world to local
    follower->setPosition ( ccpAdd ( follower->getPosition(), ccp ( ALIGN_ACCURACY, ALIGN_ACCURACY ) ) );
    CCPoint	w_from_pos_try_move = follower->convertToWorldSpace ( ccp ( followsize.width * ( follow_anchor_or_movement.x ), followsize.height * ( follow_anchor_or_movement.y ) ) );
    CCPoint move_scale = ccp ( ( w_from_pos_try_move.x - w_from_pos.x ) / ALIGN_ACCURACY, ( w_from_pos_try_move.y - w_from_pos.y ) / ALIGN_ACCURACY );
    follower->setPosition ( ccpAdd ( follower->getPosition(), ccp ( -ALIGN_ACCURACY, -ALIGN_ACCURACY ) ) );
#undef ALIGN_ACCURACY

    CCPoint w_offset;
    switch ( align )
    {
    case LEFT:
    {
        w_offset = ccp ( w_target_lb.x - w_from_pos.x, 0 );
    }
    break;
    case RIGHT:
    {
        w_offset = ccp ( w_target_rt.x - w_from_pos.x, 0 );
    }
    break;
    case CENTER:
    {
        w_target_center.x -= ( ( w_target_rt.x - w_target_lb.x ) * ( 0.5 - target_anchor.x ) );
        w_target_center.y -= ( ( w_target_rt.y - w_target_lb.y ) * ( 0.5 - target_anchor.y ) );
        w_offset = ccpSub ( w_target_center, w_from_pos );
    }
    break;
    case TOP:
    {
        w_offset = ccp ( 0, w_target_rt.y - w_from_pos.y );
    }
    break;
    case BUTTOM:
    {
        w_offset = ccp ( 0, w_target_lb.y - w_from_pos.y );
    }
    break;
    case MOVE:
    {
        w_offset = follow_anchor_or_movement;
    }
    break;
    default:
        break;
    }

    follower->setPosition ( ccpAdd ( follower->getPosition(), ccp ( w_offset.x / move_scale.x, w_offset.y / move_scale.y ) ) );
//    DebugDrawLayer::Get()->AddNode (target);
    return follower;
}

CCNode* Layout2::__Left ( float anchorX, CCNode* target, CCNode* follow )
{
    return Align ( target, follow, LEFT, ccp ( anchorX, 0 ) );
}

CCNode* Layout2::__Right ( float anchorX, CCNode* target, CCNode* follow )
{
    return Align ( target, follow, RIGHT, ccp ( anchorX, 0 ) );
}

CCNode* Layout2::__Top ( float anchorY, CCNode* target, CCNode* follow )
{
    return Align ( target, follow, TOP, ccp ( 0, anchorY ) );
}

CCNode* Layout2::__Buttom ( float anchorY, CCNode* target, CCNode* follow )
{
    return Align ( target, follow, BUTTOM, ccp ( 0, anchorY ) );
}

CCNode* Layout2::__Move ( CCPoint movement, CCNode* target )
{
    return Align ( target, target, MOVE, movement );
}

CCNode* Layout2::__Center ( CCNode* target, CCNode* follow, CCPoint follow_anchor /*= ccp (0.5, 0.5) */, CCPoint  target_anchor /*= ccp(0.5, 0.5)*/ )
{
    return Align ( target, follow, CENTER, follow_anchor, target_anchor );
}

void drawLine ( const CCPoint start, const CCPoint end, int width = 1 )
{
    for ( int i = 0; i < width; ++i )
    {
        ccDrawLine ( ccpAdd ( start, ccp ( -i, -i ) ), ccpAdd ( end, ccp ( -i, -i ) ) );
    }
    ccDrawLine ( start, end );
    for ( int i = 0; i < width; ++i )
    {
        ccDrawLine ( ccpAdd ( start, ccp ( i, i ) ), ccpAdd ( end, ccp ( i, i ) ) );
    }
}



void Layout2::__DebugLayout ( CCNode* n )
{
}

CCPoint Layout2::__Ruler ( CCNode* to, CCNode* from, CCPoint from_anchor /*= ccp (0.5, 0.5)*/, CCPoint to_anchor /*= ccp (0.5, 0.5) */ )
{
    CCRect rect_world_to;
    GetWorldRect ( to, rect_world_to );

    CCRect rect_world_from;
    GetWorldRect ( from, rect_world_from );
    return ccp ( rect_world_to.origin.x + to_anchor.x * rect_world_to.size.width - rect_world_from.origin.x - from_anchor.x * rect_world_from.size.width,
                 rect_world_to.origin.y + to_anchor.y * rect_world_to.size.height - rect_world_from.origin.y - from_anchor.y * rect_world_from.size.height
               );
}

CCNode* Layout2::__Middle ( CCNode* target1, CCNode* target2, CCNode* follow )
{
    CCPoint w_target_lb1, w_target_rt1, w_target_center1;
    GetWorldRect ( target1, w_target_lb1, w_target_rt1, w_target_center1 );

    CCPoint w_target_lb2, w_target_rt2, w_target_center2;
    GetWorldRect ( target2, w_target_lb2, w_target_rt2, w_target_center2 );

    CCPoint w_follow_will_go_to = w_target_center1 + ( w_target_center2 - w_target_center1 ) / 2;
    if ( follow->getParent() )
    {
        follow->setPosition ( follow->getParent()->convertToNodeSpace ( w_follow_will_go_to ) );
    }
    else
    {
        //world coordinate
        follow->setPosition ( w_follow_will_go_to );
    }

    return follow;
}



DebugDrawLayer* DebugDrawLayer::s_drawLayer = NULL;
int DebugDrawLayer::s_id = 1;
#define STAY_SEC 10
bool DebugDrawLayer::init()
{
    if ( !CCLayer::init() ) return false;
#ifdef  COCOS2D
    CC_SCHEDULE ( schedule_selector ( DebugDrawLayer::Update ), this, 1.0 / 60.0f, false );
#endif
    return true;
}

void DebugDrawLayer::LazyInit()
{
    if ( DebugDrawLayer::s_drawLayer == NULL )
    {
        s_drawLayer = DebugDrawLayer::create();
        s_drawLayer->setTag ( ( int ) s_drawLayer );
        if (!CCDirector::sharedDirector()->getNotificationNode())
            CCDirector::sharedDirector()->setNotificationNode (CCNode::create());
        CCDirector::sharedDirector()->getNotificationNode()->addChild ( s_drawLayer );
    }
}
void DebugDrawLayer::DrawRect ( CCRect r, int width, ccColor4F c )
{
    CCPoint left_bottom = r.origin;
    CCPoint left_top = ccp ( r.origin.x, r.origin.y + r.size.height );
    CCPoint right_top = ccp ( r.origin.x + r.size.width, r.origin.y + r.size.height );
    CCPoint right_bottom = ccp ( right_top.x, left_bottom.y );
    ccDrawSolidRect ( ccpAdd ( left_bottom, ccp ( -width / 2, -width / 2 ) ), ccpAdd ( left_top, ccp ( width / 2, width / 2 ) ), c );
    ccDrawSolidRect ( ccpAdd ( left_top, ccp ( -width / 2, -width / 2 ) ), ccpAdd ( right_top, ccp ( width / 2, width / 2 ) ), c );
    ccDrawSolidRect ( ccpAdd ( right_bottom, ccp ( -width / 2, -width / 2 ) ), ccpAdd ( right_top, ccp ( width / 2, width / 2 ) ), c );
    ccDrawSolidRect ( ccpAdd ( left_bottom, ccp ( -width / 2, -width / 2 ) ), ccpAdd ( right_bottom, ccp ( width / 2, width / 2 ) ), c );
}

void DebugDrawLayer::draw()
{
    CCLayer::draw();
    for ( auto &d : _needs_draw )
    {
        int base_color = 256;
        int step = base_color / d.stack_count;
        for ( int i = d.stack_count; i > 0; i-- )
        {
            ccColor4F  c = { 1, 1, 1, 1 };
            c.g = step * i / 256.0f;
            c.b = step * i / 256.0f;
            DrawRect ( d.r,   2, c );
        }
    }
}


void DebugDrawLayer::AddNode ( CCNode* n, ccColor4F color /*= ccc4f (1, 1, 1, 1)*/ )
{
    CCPoint w_target_lb, w_target_rt, w_target_center;
    Layout2::GetWorldRect ( n, w_target_lb, w_target_rt, w_target_center );
    AddRect ( CCRectMake ( w_target_lb.x, w_target_lb.y, w_target_rt.x - w_target_lb.x, w_target_rt.y - w_target_lb.y ), color );
}

void DebugDrawLayer::AddRect ( CCRect r, ccColor4F color /*= ccc4f (1,1,1,1)*/ )
{
    DebugDrawLayer::LazyInit();
    assert ( s_id != INT_MAX && "you are awsome!" );
    DebugDrawLayer::_AdvancedRect ar = { 1, r, 1, STAY_SEC, color };
    for ( auto & a : _needs_draw )
    {
        if ( a.r.equals ( ar.r ) )   //the same size
        {
            a.stack_count++;
            return;
        }
    }
    _needs_draw.push_back ( ar );
}




void DebugDrawLayer::ClearAllRects()
{
    _needs_draw.clear();
}





DebugDrawLayer* DebugDrawLayer::Get()
{
    DebugDrawLayer::LazyInit();
    return s_drawLayer;
}

void DebugDrawLayer::Update ( float d )
{
    for ( int i = _needs_draw.size() - 1; i >= 0; --i )
    {
        _needs_draw[i].remain_secs -= d;
        if ( _needs_draw[i].remain_secs <= 0 )
        {
            if ( _needs_draw[i].stack_count > 1 )
            {
                _needs_draw[i].stack_count--;
                _needs_draw[i].remain_secs = STAY_SEC;
            }

        }
    }
}



