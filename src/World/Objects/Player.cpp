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
}

void Player::keyDown(const CL_InputEvent& ev, const CL_InputState& state)
{
    float speed = 10;
    if(ev.id == CL_KEY_D)
    {
        mJoint->EnableLimit(false);
        mJoint->SetMaxMotorTorque(1000);
        mJoint->SetMotorSpeed(-speed);
        mJoint->EnableMotor(true);
    }
    else if(ev.id == CL_KEY_A)
    {
        mJoint->EnableLimit(false);
        mJoint->SetMaxMotorTorque(1000);
        mJoint->SetMotorSpeed(speed);
        mJoint->EnableMotor(true);
    }
}

void Player::keyUp(const CL_InputEvent& ev, const CL_InputState& state)
{
    if( (ev.id == CL_KEY_D) || (ev.id == CL_KEY_A) )
    {
        mJoint->EnableMotor(false);
        mJoint->SetLimits(mJoint->GetJointAngle(),mJoint->GetJointAngle());
        mJoint->EnableLimit(true);
    }
}

void Player::updateVisual(float newX, float newY)
{
}

void Player::step(float32 elapsed)
{

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
                       boost::shared_ptr<Body> bodyCircle,
                       b2RevoluteJoint* joint)
{
    mTopBoxBody = bodyTopBox;
    mCircleBody = bodyCircle;
    mJoint = joint;
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
    boost::shared_ptr<Body> circleHandle = boost::shared_ptr<Body>(new Body());

    //* Core body
    b2BodyDef coreBodyDef;
    coreBodyDef.position.Set((x+PlayerWidth/2), (y+PlayerHeight - PlayerWidth/2 ));
    coreBodyDef.type = b2_dynamicBody;
    coreBodyDef.fixedRotation = true;
    b2Body* coreBody = physicManager().getWorld().CreateBody(&coreBodyDef);
    //b2PolygonShape coreShape;
    //coreShape.SetAsBox(0.1, PlayerWidth/2);
    //b2FixtureDef  fixdefCore;
    //fixdefCore.shape = &coreShape;
    //coreBody->CreateFixture(&fixdefCore);*/

    // Top Box part

    b2BodyDef topBoxDef;
    topBoxDef.position.Set((x), (y));
    topBoxDef.type = b2_dynamicBody;
    topBoxDef.fixedRotation = true;
    topBoxDef.bullet = true;
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
    fixdefTop.shape = &topBoxshape;
    fixdefTop.filter.groupIndex = -7;
    topBox->CreateFixture(&fixdefTop);

    // Bottom Box part
    b2PolygonShape bottomBoxshape;
    b2Vec2 vertices2[4];
    vertices2[0] = b2Vec2(0, PlayerHeight/2);
    vertices2[1] = b2Vec2(PlayerWidth, PlayerHeight/2);
    vertices2[2] = b2Vec2(PlayerWidth, PlayerHeight - PlayerWidth/2);
    vertices2[3] = b2Vec2(0, PlayerHeight - PlayerWidth/2);
    bottomBoxshape.Set(vertices2, 4);
    b2FixtureDef  fixdef;
    fixdef.shape = &bottomBoxshape;
    fixdef.restitution = 0;
    fixdef.filter.groupIndex = -7;
    topBox->CreateFixture(&fixdef);

    topBoxHandle->setBody(topBox);

    // Circle part

    b2BodyDef circleDef;
    circleDef.position.Set(x+PlayerWidth/2, y + PlayerHeight - PlayerWidth/2 );
    circleDef.type = b2_dynamicBody;
    b2Body* circle = physicManager().getWorld().CreateBody(&circleDef);

    circleHandle->setBody(circle);

    b2CircleShape circleShape;
    circleShape.m_radius = (PlayerWidth/2);
    b2FixtureDef fixdefCircle;
    fixdefCircle.shape = &circleShape;
    fixdefCircle.friction = 30;
    fixdefCircle.restitution = 0;
    fixdefCircle.density = 10;
    fixdefCircle.filter.groupIndex = -7;
    circle->CreateFixture(&fixdefCircle);

    //b2MassData mass;
    //mass.mass = PlayerMass/2;
    //circle->SetMassData(&mass);
    //topBox->SetMassData(&mass);

    // Join the bodies

    b2RevoluteJointDef jointDef;
    jointDef.Initialize( circle, topBox, circle->GetWorldCenter());
    jointDef.collideConnected = false;
    b2RevoluteJoint* joint = (b2RevoluteJoint*) physicManager().getWorld().CreateJoint(&jointDef);

/*
    // Amortization
    b2DistanceJointDef ajointDef;
    ajointDef.Initialize(coreBody, topBox, coreBody->GetWorldCenter(),  coreBody->GetWorldCenter());
    ajointDef.collideConnected = false;
    ajointDef.dampingRatio = 0;
    physicManager().getWorld().CreateJoint(&ajointDef);*/

    result->setPhysic(topBoxHandle, circleHandle, joint);

    return boost::shared_ptr<Object>(result);
}

