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
    float speed = 0;
    // Right
    if(ev.id == CL_KEY_D)
    {
        speed = -mRollAngularVelocity;
    }
    // Left
    else if(ev.id == CL_KEY_A)
    {
        speed = mRollAngularVelocity;
    }
    if(speed != 0)
    {
        for(int i=0; i<4; i++)
        {
            mRolls[i]->getBody()->SetFixedRotation(false);
            mJoints[i]->SetMotorSpeed(speed);
            mJoints[i]->EnableMotor(true);
        }
    }
    // Jump
    else if( ev.id == CL_KEY_W || ev.id == CL_KEY_SPACE )
    {
        b2ContactEdge* ce = getFirstTouchingContact(mRolls[0]);
        if(ce == NULL) ce = getFirstTouchingContact(mRolls[1]);
        if(ce == NULL) ce = getFirstTouchingContact(mRolls[2]);
        if(ce == NULL) ce = getFirstTouchingContact(mRolls[3]);
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
            // Jumping up to pitch slower
            if( (jumpVelocity.x * m.normal.x < 0) && rotated )
            {
                float koef = m.normal.y*m.normal.y*m.normal.y*m.normal.y;
                jumpVelocity = koef * jumpVelocity;
            }
            jumpVelocity += b2Vec2(mMiddleBoxBody->getBody()->GetLinearVelocity().x, 0);

            for(unsigned int i =0; i<mBodies.size(); ++i)
            {
                mBodies[i]->getBody()->SetLinearVelocity(jumpVelocity);
            }
            ce->other->ApplyLinearImpulse((-1*PlayerMass)*jumpVelocity, b2Vec2(0,0));
        }
    }
}

void Player::keyUp(const CL_InputEvent& ev, const CL_InputState& state)
{
    if(!inputManager().keyPressed( CL_KEY_D) && !inputManager().keyPressed( CL_KEY_A) )
    {
        for(int i=0; i<4; i++)
        {
            mJoints[i]->EnableMotor(false);
            mRolls[i]->getBody()->SetAngularVelocity(0);
            mRolls[i]->getBody()->SetFixedRotation(true);
        }
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

void Player::setPhysic(std::vector< boost::shared_ptr<Body> > bodies,
                       std::vector<b2RevoluteJoint*> joints)
{
    mBodies = bodies;
    mRolls.push_back(bodies[0]);
    mRolls.push_back(bodies[1]);
    mRolls.push_back(bodies[2]);
    mRolls.push_back(bodies[3]);
    mTopBoxBody = bodies[4];
    mMiddleBoxBody = bodies[5];
    mShoulderBody = bodies[6];
    mRollBaseBody = bodies[7];

    mJoints = joints;
    mRollAngularVelocity = PlayerSpeed / mBodies[0]->getBody()->GetFixtureList()->GetShape()->m_radius;
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


    boost::shared_ptr<Body> topBoxHandle = boost::shared_ptr<Body>(new Body());
    boost::shared_ptr<Body> middleBoxHandle = boost::shared_ptr<Body>(new Body());
    boost::shared_ptr<Body> shoulderHandle = boost::shared_ptr<Body>(new Body());
    boost::shared_ptr<Body> rollBaseHandle = boost::shared_ptr<Body>(new Body());

    std::vector< boost::shared_ptr<Body> > bodyHandles;
    bodyHandles.push_back(boost::shared_ptr<Body>(new Body()));
    bodyHandles.push_back(boost::shared_ptr<Body>(new Body()));
    bodyHandles.push_back(boost::shared_ptr<Body>(new Body()));
    bodyHandles.push_back(boost::shared_ptr<Body>(new Body()));
    bodyHandles.push_back(topBoxHandle);
    bodyHandles.push_back(middleBoxHandle);
    bodyHandles.push_back(shoulderHandle);
    bodyHandles.push_back(rollBaseHandle);

    b2FixtureDef  fixdef;
    fixdef.restitution = 0;
    fixdef.filter.groupIndex = -7;
    fixdef.density = 1;

    // Top Box part

    b2BodyDef topBoxDef;
    topBoxDef.position.Set((x), (y));
    topBoxDef.type = b2_dynamicBody;
    b2Body* topBox = physicManager().getWorld().CreateBody(&topBoxDef);
    // Shape
    b2PolygonShape topBoxshape;
    b2Vec2 vertices[4];
    vertices[0] = b2Vec2(0, 0);
    vertices[1] = b2Vec2(PlayerWidth, 0);
    vertices[2] = b2Vec2(PlayerWidth, PlayerHeight/2);
    vertices[3] = b2Vec2(0, PlayerHeight/2);
    topBoxshape.Set(vertices, 4);
    fixdef.shape = &topBoxshape;
    topBox->CreateFixture(&fixdef);

    // Middle Box part
    b2BodyDef middleBoxDef;
    middleBoxDef.position.Set((x), (y+PlayerHeight/2));
    middleBoxDef.type = b2_dynamicBody;
    middleBoxDef.fixedRotation = true;
    b2Body* middleBox = physicManager().getWorld().CreateBody(&middleBoxDef);
    b2PolygonShape middleBoxshape;
    b2Vec2 vertices2[4];
    vertices2[0] = b2Vec2(0, 0);
    vertices2[1] = b2Vec2(PlayerWidth, 0);
    vertices2[2] = b2Vec2(PlayerWidth*3/4, PlayerHeight/2 - PlayerWidth/2);
    vertices2[3] = b2Vec2(PlayerWidth/4, PlayerHeight/2 - PlayerWidth/2);
    middleBoxshape.Set(vertices2, 4);
    fixdef.shape = &middleBoxshape;
    middleBox->CreateFixture(&fixdef);
    middleBoxHandle->setBody(topBox);

    // Shoulder
    b2BodyDef shoulderDef;
    shoulderDef.position.Set((x+PlayerWidth/2), (y+PlayerHeight/4));
    shoulderDef.type = b2_dynamicBody;
    b2Body* shoulder = physicManager().getWorld().CreateBody(&shoulderDef);
    b2CircleShape shoulderShape;
    shoulderShape.m_radius = PlayerWidth/4;
    fixdef.shape = &shoulderShape;
    shoulder->CreateFixture(&fixdef);
    shoulderHandle->setBody(topBox);

    // Roll base

    b2BodyDef rollBaseDef;
    rollBaseDef.position.Set((x+PlayerWidth/2), (y+PlayerHeight- PlayerWidth/5));
    rollBaseDef.type = b2_dynamicBody;
    rollBaseDef.angularDamping = 1;
    b2Body* rollBase = physicManager().getWorld().CreateBody(&rollBaseDef);
    b2PolygonShape rollBaseShape;
    rollBaseShape.SetAsBox(PlayerWidth/2, PlayerWidth/12);
    fixdef.shape = &rollBaseShape;
    rollBase->CreateFixture(&fixdef);
    rollBaseHandle->setBody(rollBase);

    // Circles
    b2BodyDef circleDef;
    circleDef.type = b2_dynamicBody;
    b2CircleShape circleShape;
    circleShape.m_radius = (PlayerWidth/5);
    b2FixtureDef fixdefCircle;
    fixdefCircle.shape = &circleShape;
    fixdefCircle.friction = 3;
    fixdefCircle.restitution = 0;
    fixdefCircle.density = 1;
    fixdefCircle.filter.groupIndex = -7;

    circleDef.position.Set(x + PlayerWidth/5, y + PlayerHeight - PlayerWidth/5 );
    b2Body* circle1 = physicManager().getWorld().CreateBody(&circleDef);
    circle1->CreateFixture(&fixdefCircle);
    bodyHandles[0]->setBody(circle1);

    circleDef.position.Set(x + PlayerWidth*2/5, y + PlayerHeight - PlayerWidth/5 );
    b2Body* circle2 = physicManager().getWorld().CreateBody(&circleDef);
    circle2->CreateFixture(&fixdefCircle);
    bodyHandles[1]->setBody(circle2);

    circleDef.position.Set(x + PlayerWidth*3/5, y + PlayerHeight - PlayerWidth/5 );
    b2Body* circle3 = physicManager().getWorld().CreateBody(&circleDef);
    circle3->CreateFixture(&fixdefCircle);
    bodyHandles[2]->setBody(circle3);

    circleDef.position.Set(x + PlayerWidth*4/5, y + PlayerHeight - PlayerWidth/5 );
    b2Body* circle4 = physicManager().getWorld().CreateBody(&circleDef);
    circle4->CreateFixture(&fixdefCircle);
    bodyHandles[3]->setBody(circle4);

    float m = 0;
    m+= rollBase->GetMass();
    m+= middleBox->GetMass();
    m+= topBox->GetMass();
    m+= shoulder->GetMass();
    b2MassData mass;
    mass.mass = (PlayerMass-m)/4;
    mass.I = 1;
    mass.center.Set(0,0);
    circle1->SetMassData(&mass);
    circle2->SetMassData(&mass);
    circle3->SetMassData(&mass);
    circle4->SetMassData(&mass);

    // Join the bodies

    std::vector<b2RevoluteJoint*> joints;

    b2RevoluteJointDef circleJointDef;
    circleJointDef.Initialize( circle1, rollBase, circle1->GetWorldCenter());
    circleJointDef.collideConnected = false;
    circleJointDef.maxMotorTorque = 3;
    joints.push_back((b2RevoluteJoint*) physicManager().getWorld().CreateJoint(&circleJointDef));

    circleJointDef.Initialize( circle2, rollBase, circle2->GetWorldCenter());
    joints.push_back((b2RevoluteJoint*) physicManager().getWorld().CreateJoint(&circleJointDef));

    circleJointDef.Initialize( circle3, rollBase, circle3->GetWorldCenter());
    joints.push_back((b2RevoluteJoint*) physicManager().getWorld().CreateJoint(&circleJointDef));

    circleJointDef.Initialize( circle4, rollBase, circle4->GetWorldCenter());
    joints.push_back((b2RevoluteJoint*) physicManager().getWorld().CreateJoint(&circleJointDef));

    b2RevoluteJointDef jointDef;
    jointDef.Initialize( rollBase,middleBox,  rollBase->GetWorldCenter());
    jointDef.lowerAngle = -0.78; // pi/4
    jointDef.upperAngle = 0.78;
    jointDef.enableLimit = true;
    jointDef.collideConnected = false;
    joints.push_back((b2RevoluteJoint*) physicManager().getWorld().CreateJoint(&jointDef));

    b2RevoluteJointDef topToMiddleDef;
    topToMiddleDef.Initialize( topBox, middleBox,  b2Vec2(x+PlayerWidth/2, y+PlayerHeight/2));
    topToMiddleDef.lowerAngle = 0;
    topToMiddleDef.upperAngle = 0;
    topToMiddleDef.enableLimit = true;
    topToMiddleDef.collideConnected = false;
    joints.push_back((b2RevoluteJoint*) physicManager().getWorld().CreateJoint(&topToMiddleDef));

    b2RevoluteJointDef shoudlerToTopDef;
    shoudlerToTopDef.Initialize( topBox, shoulder,  shoulder->GetWorldCenter());
    shoudlerToTopDef.lowerAngle = 0;
    shoudlerToTopDef.upperAngle = 0;
    shoudlerToTopDef.enableLimit = true;
    shoudlerToTopDef.collideConnected = false;
    joints.push_back((b2RevoluteJoint*) physicManager().getWorld().CreateJoint(&shoudlerToTopDef));


    result->setPhysic(bodyHandles, joints);

    return boost::shared_ptr<Object>(result);
}

