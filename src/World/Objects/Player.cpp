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
}

void Player::keyUp(const CL_InputEvent& ev, const CL_InputState& state)
{
    //mBody->getBody()->SetLinearVelocity(b2Vec2(0,0));
}

void Player::updateVisual(float newX, float newY)
{
}

void Player::step(float32 elapsed)
{
    b2Vec2 velocity = mBody->getBody()->GetLinearVelocity();
    float speed = 10;
    if(mInputManager->keyPressed(CL_KEY_D))
    {
        if(velocity.x < speed)
        {
            velocity.x += speed;
            if(velocity.x > speed) velocity.x = speed;
        }
        mBody->getBody()->SetLinearVelocity(velocity);
    }
    else if(mInputManager->keyPressed(CL_KEY_A))
    {
        if(velocity.x > -speed)
        {
            velocity.x -= speed;
            if(velocity.x < -speed) velocity.x = -speed;
        }
        mBody->getBody()->SetLinearVelocity(velocity);
    }

}


void Player::setPosition(CL_Pointf newPos)
{
    throw CL_Exception("Can't move body, after it created.");
}

CL_Pointf Player::getPosition()
{
    b2Vec2 position = mBody->getBody()->GetPosition();
    return CL_Pointf(Meters2Pixels(position.x), Meters2Pixels(position.y));
}

CL_Rectf Player::getRectangle()
{
    b2AABB rect;
    b2Fixture* fixture = mBody->getBody()->GetFixtureList();
    if (fixture != NULL) rect = fixture->GetAABB();
    for(; fixture != NULL; fixture=fixture->GetNext())
        rect.Combine(rect, fixture->GetAABB());

    return CL_Rectf(Meters2Pixels(rect.lowerBound.x), Meters2Pixels(rect.lowerBound.y),
            Meters2Pixels(rect.upperBound.x), Meters2Pixels(rect.upperBound.y));
}

void Player::update(float elapsed)
{
    step(elapsed);
}

 boost::shared_ptr<Body> Player::getBody()
{
    return mBody;
}

void Player::setBody(boost::shared_ptr<Body> body)
{
    mBody = body;
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
    b2BodyDef bodyDef;
    bodyDef.position.Set(x, y);
    bodyDef.type = b2_dynamicBody;
    bodyDef.fixedRotation = true;
    b2Body* body = physicManager().getWorld().CreateBody(&bodyDef);

    // Box shape
    b2PolygonShape shape;
    shape.SetAsBox(Pixels2Meters(PlayerWidth), Pixels2Meters(PlayerHeight));
    b2FixtureDef  fixdef;
    fixdef.shape = &shape;
    fixdef.friction = 1;
    fixdef.restitution = 0;
    body->CreateFixture(&fixdef);

    // Mass
    b2MassData mass;
    mass.mass = PlayerMass;
    body->SetMassData(&mass);

    boost::shared_ptr<Body> bodyHandle = boost::shared_ptr<Body>(new Body());
    bodyHandle->setBody(body);
    result->setBody(bodyHandle);

    return boost::shared_ptr<Object>(result);
}

