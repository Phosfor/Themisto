/*
   Copyright 2010-2011 Tyslenko Max, Bogatirev Pavel.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "World/Objects/Player.hpp"

#include "Physic/Body.hpp"
#include "Physic/Impact.hpp"



Player::Player()
{
    mName = worldManager().generateUniqueID();
    mInputManager = &inputManager();
    mType = PhysicBodyObject;
    mGC = appManager().getGraphic();
    CL_InputDevice keyb = inputManager().getKeyboard();
    mKeyDownSlot = keyb.sig_key_down().connect(this, &Player::keyDown);
    mKeyUpSlot = keyb.sig_key_up().connect(this, &Player::keyUp);
    mJumpVelocity = sqrt(20*PlayerHeight/2);
}

b2ContactEdge* getFirstTouchingContact(boost::shared_ptr<Body> body)
{
    for(b2ContactEdge* ce = body->getBody()->GetContactList(); ce != NULL; ce=ce->next)
    {
        if(ce->contact->IsTouching())
        {
            return ce;
        }
    }
    return NULL;
}

void Player::keyDown(const CL_InputEvent& ev, const CL_InputState& state)
{
    if(inputManager().keyPressed( CL_KEY_D) || inputManager().keyPressed( CL_KEY_A) )
    {
        mCircleBody1->getBody()->SetFixedRotation(false);
        mCircleBody2->getBody()->SetFixedRotation(false);
        mCircleBody3->getBody()->SetFixedRotation(false);
        mCircleBody4->getBody()->SetFixedRotation(false);
    }
    if( ev.id == CL_KEY_W || ev.id == CL_KEY_SPACE )
    {
        b2ContactEdge* ce = getFirstTouchingContact(mCircleBody1);
        if(ce == NULL) ce = getFirstTouchingContact(mCircleBody2);
        if(ce == NULL) ce = getFirstTouchingContact(mCircleBody3);
        if(ce == NULL) ce = getFirstTouchingContact(mCircleBody4);
        if(ce != NULL)
        {
            b2WorldManifold m;
            ce->contact->GetWorldManifold(&m);
            b2Vec2 jumpVelocity = mJumpVelocity* m.normal;
            float sin = 0.5;
            float cos = 0.866;
            bool rotated = false;
            if(inputManager().keyPressed( CL_KEY_D))
            {
                // Rotate velocity vector to pi/3 angle
                jumpVelocity.Set(jumpVelocity.x *cos- jumpVelocity.y *sin, jumpVelocity.x *sin + jumpVelocity.y *cos);
                rotated = true;
            }
            else if(inputManager().keyPressed( CL_KEY_A))
            {
                // Rotate velocity vector to -pi/3 angle
                jumpVelocity.Set(jumpVelocity.x *cos + jumpVelocity.y *sin, -jumpVelocity.x *sin + jumpVelocity.y *cos);
                rotated = true;
            }
            // Jumping up
            if( (jumpVelocity.x * m.normal.x < 0) && rotated )
            {
                float koef = m.normal.y*m.normal.y*m.normal.y*m.normal.y;
                jumpVelocity = koef * jumpVelocity;
            }
            jumpVelocity += ce->other->GetLinearVelocity();
            mTopBoxBody->getBody()->SetLinearVelocity(jumpVelocity);
            mRollBaseBody->getBody()->SetLinearVelocity(jumpVelocity);
            mCircleBody1->getBody()->SetLinearVelocity(jumpVelocity);
            mCircleBody2->getBody()->SetLinearVelocity(jumpVelocity);
            mCircleBody3->getBody()->SetLinearVelocity(jumpVelocity);
            mCircleBody4->getBody()->SetLinearVelocity(jumpVelocity);
            ce->other->ApplyLinearImpulse((-1*PlayerMass)*jumpVelocity, b2Vec2(0,0));
        }
    }
}

void Player::keyUp(const CL_InputEvent& ev, const CL_InputState& state)
{
    if(!inputManager().keyPressed( CL_KEY_D) && !inputManager().keyPressed( CL_KEY_A) )
    {
        mCircleBody1->getBody()->SetAngularVelocity(0);
        mCircleBody1->getBody()->SetFixedRotation(true);
        mCircleBody2->getBody()->SetAngularVelocity(0);
        mCircleBody2->getBody()->SetFixedRotation(true);
        mCircleBody3->getBody()->SetAngularVelocity(0);
        mCircleBody3->getBody()->SetFixedRotation(true);
        mCircleBody4->getBody()->SetAngularVelocity(0);
        mCircleBody4->getBody()->SetFixedRotation(true);
    }

}

void Player::updateVisual(float newX, float newY)
{
}

void Player::step(float32 elapsed)
{
    // A - left, D - right
    float speed = 0;
    if(inputManager().keyPressed( CL_KEY_D))
    {
        speed = mRollAngularVelocity;
    }
    else if(inputManager().keyPressed( CL_KEY_A))
    {
        speed = -mRollAngularVelocity;
    }
    if(speed != 0)
    {
        mCircleBody1->getBody()->SetAngularVelocity(speed);
        mCircleBody2->getBody()->SetAngularVelocity(speed);
        mCircleBody3->getBody()->SetAngularVelocity(speed);
        mCircleBody4->getBody()->SetAngularVelocity(speed);
    }
}


void Player::setPosition(CL_Pointf newPos)
{
    throw CL_Exception("Can't move body, after it created.");
}

CL_Pointf Player::getPosition()
{
    b2Vec2 position = mTopBoxBody->getBody()->GetPosition();
    return CL_Pointf(Meters2Pixels(position.x), Meters2Pixels(position.y));
}

CL_Rectf Player::getRectangle()
{
    b2Vec2 pos = mTopBoxBody->getBody()->GetPosition();
    return CL_Rectf(pos.x, pos.y, pos.x + PlayerWidth, pos.y + PlayerHeight);
}

void Player::update(float elapsed)
{
    step(elapsed);
}

void Player::setPhysic(boost::shared_ptr<Body> bodyTopBox,
                       boost::shared_ptr<Body> bodyRollBase,
                       boost::shared_ptr<Body> bodyCircle1,
                       boost::shared_ptr<Body> bodyCircle2,
                       boost::shared_ptr<Body> bodyCircle3,
                       boost::shared_ptr<Body> bodyCircle4)
{
    mTopBoxBody = bodyTopBox;
    mRollBaseBody = bodyRollBase;
    mCircleBody1 = bodyCircle1;
    mCircleBody2 = bodyCircle2;
    mCircleBody3 = bodyCircle3;
    mCircleBody4 = bodyCircle4;
    mRollAngularVelocity = PlayerSpeed / mCircleBody1->getBody()->GetFixtureList()->GetShape()->m_radius;
}

boost::shared_ptr<Object> Player::ParsePlayer(CL_DomElement* tag, std::string& desc)
{
    Player* result = new Player(); // Smart pointer will be created at the end

    float x = 0, y = 0;
    // Parsing position
    {
        CL_DomNodeList positionTags = tag->get_elements_by_tag_name("Position");
        if (positionTags.get_length() == 1)
        {
            CL_DomNodeList position = positionTags.item(0).to_element().get_child_nodes();
            for (int i=0; i < position.get_length(); ++i)
            {
                CL_DomElement handle = position.item(i).to_element();
                if (handle.get_node_name() == "x")
                    x = Pixels2Meters(boost::lexical_cast<float>(handle.get_text().c_str()));
                else if (handle.get_node_name() == "y")
                    y = Pixels2Meters(boost::lexical_cast<float>(handle.get_text().c_str()));
            }
        }
        else if(positionTags.get_length() > 1 ||  positionTags.get_length()==0)
        {
            throw CL_Exception("Error: one, and only one tag Position can be in Player.");
        }
    }

    // Player consists of three objects: two rectangles on roll

    boost::shared_ptr<Body> topBoxHandle = boost::shared_ptr<Body>(new Body());
    boost::shared_ptr<Body> rollBaseHandle = boost::shared_ptr<Body>(new Body());
    boost::shared_ptr<Body> circle1Handle = boost::shared_ptr<Body>(new Body());
    boost::shared_ptr<Body> circle2Handle = boost::shared_ptr<Body>(new Body());
    boost::shared_ptr<Body> circle3Handle = boost::shared_ptr<Body>(new Body());
    boost::shared_ptr<Body> circle4Handle = boost::shared_ptr<Body>(new Body());

    // Top Box part

    b2BodyDef topBoxDef;
    topBoxDef.position.Set((x), (y));
    topBoxDef.type = b2_dynamicBody;
    topBoxDef.fixedRotation = true;
    b2Body* topBox = physicManager().getWorld().CreateBody(&topBoxDef);
    // Shape
    b2PolygonShape topBoxshape;
    b2Vec2 vertices[4];
    vertices[0] = b2Vec2(0, 0);
    vertices[1] = b2Vec2(PlayerWidth, 0);
    vertices[2] = b2Vec2(PlayerWidth, PlayerHeight/2);
    vertices[3] = b2Vec2(0, PlayerHeight/2);
    topBoxshape.Set(vertices, 4);
    b2FixtureDef  fixdefTop;
    fixdefTop.restitution = 0;
    fixdefTop.density = 1;
    fixdefTop.shape = &topBoxshape;
    fixdefTop.filter.groupIndex = -7;
    topBox->CreateFixture(&fixdefTop);

    // Bottom Box part
    b2PolygonShape bottomBoxshape;
    b2Vec2 vertices2[4];
    vertices2[0] = b2Vec2(0, PlayerHeight/2);
    vertices2[1] = b2Vec2(PlayerWidth, PlayerHeight/2);
    vertices2[2] = b2Vec2(PlayerWidth*3/4, PlayerHeight - PlayerWidth/2);
    vertices2[3] = b2Vec2(PlayerWidth/4, PlayerHeight - PlayerWidth/2);
    bottomBoxshape.Set(vertices2, 4);
    b2FixtureDef  fixdef;
    fixdef.shape = &bottomBoxshape;
    fixdef.restitution = 0;
    fixdef.filter.groupIndex = -7;
    fixdef.density = 1;
    topBox->CreateFixture(&fixdef);

    topBoxHandle->setBody(topBox);

    // Roll base

    b2BodyDef rollBaseDef;
    rollBaseDef.position.Set((x+PlayerWidth/2), (y+PlayerHeight- PlayerWidth/5));
    rollBaseDef.type = b2_dynamicBody;
    rollBaseDef.angularDamping = 1;
    b2Body* rollBase = physicManager().getWorld().CreateBody(&rollBaseDef);
    b2PolygonShape rollBaseShape;
    rollBaseShape.SetAsBox(PlayerWidth/2, PlayerWidth/15);
    b2FixtureDef  rollFixdef;
    rollFixdef.shape = &rollBaseShape;
    rollFixdef.filter.groupIndex = -7;
    rollFixdef.density = 1;
    rollFixdef.friction = 0;
    rollBase->CreateFixture(&rollFixdef);
    rollBaseHandle->setBody(rollBase);

    // Circles
    b2CircleShape circleShape;
    circleShape.m_radius = (PlayerWidth/5);
    b2FixtureDef fixdefCircle;
    fixdefCircle.shape = &circleShape;
    fixdefCircle.friction = 5;
    fixdefCircle.restitution = 0;
    fixdefCircle.density = 1;
    fixdefCircle.filter.groupIndex = -7;

    b2BodyDef circle1Def;
    circle1Def.position.Set(x + PlayerWidth/5 - 0.2, y + PlayerHeight - PlayerWidth/5 );
    circle1Def.type = b2_dynamicBody;
    b2Body* circle1 = physicManager().getWorld().CreateBody(&circle1Def);
    circle1->CreateFixture(&fixdefCircle);
    circle1Handle->setBody(circle1);

    b2BodyDef circle2Def;
    circle2Def.position.Set(x + PlayerWidth*2/5-0.1, y + PlayerHeight - PlayerWidth/5 );
    circle2Def.type = b2_dynamicBody;
    b2Body* circle2 = physicManager().getWorld().CreateBody(&circle2Def);
    circle2->CreateFixture(&fixdefCircle);
    circle2Handle->setBody(circle2);

    b2BodyDef circle3Def;
    circle3Def.position.Set(x + PlayerWidth*3/5+0.1, y + PlayerHeight - PlayerWidth/5 );
    circle3Def.type = b2_dynamicBody;
    b2Body* circle3 = physicManager().getWorld().CreateBody(&circle3Def);
    circle3->CreateFixture(&fixdefCircle);
    circle3Handle->setBody(circle3);

    b2BodyDef circle4Def;
    circle4Def.position.Set(x + PlayerWidth*4/5 +0.2, y + PlayerHeight - PlayerWidth/5 );
    circle4Def.type = b2_dynamicBody;
    b2Body* circle4 = physicManager().getWorld().CreateBody(&circle4Def);
    circle4->CreateFixture(&fixdefCircle);
    circle4Handle->setBody(circle4);

    b2MassData mass;
    mass.mass = PlayerMass*7/32;
    mass.I = 0.01;
    mass.center.Set(0,0);
    circle1->SetMassData(&mass);
    circle2->SetMassData(&mass);
    circle3->SetMassData(&mass);
    circle4->SetMassData(&mass);
    mass.mass = PlayerMass*2/32;
    topBox->SetMassData(&mass);
    rollBase->SetMassData(&mass);


    // Join the bodies

    b2RevoluteJointDef jointDef;
    jointDef.Initialize( rollBase, topBox,  rollBase->GetWorldCenter());
    jointDef.lowerAngle = -0.78; // pi/4
    jointDef.upperAngle = 0.78;
    jointDef.enableLimit = true;
    jointDef.collideConnected = false;
    physicManager().getWorld().CreateJoint(&jointDef);

    b2RevoluteJointDef circleJointDef;
    circleJointDef.Initialize( circle1, rollBase, circle1->GetWorldCenter());
    circleJointDef.collideConnected = false;
    physicManager().getWorld().CreateJoint(&circleJointDef);

    circleJointDef.Initialize( circle2, rollBase, circle2->GetWorldCenter());
    circleJointDef.collideConnected = false;
    physicManager().getWorld().CreateJoint(&circleJointDef);

    circleJointDef.Initialize( circle3, rollBase, circle3->GetWorldCenter());
    circleJointDef.collideConnected = false;
    physicManager().getWorld().CreateJoint(&circleJointDef);

    circleJointDef.Initialize( circle4, rollBase, circle4->GetWorldCenter());
    circleJointDef.collideConnected = false;
    physicManager().getWorld().CreateJoint(&circleJointDef);


    result->setPhysic(topBoxHandle, rollBaseHandle, circle1Handle, circle2Handle,circle3Handle,circle4Handle);

    return boost::shared_ptr<Object>(result);
}

