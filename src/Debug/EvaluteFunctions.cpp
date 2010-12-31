/*
   Copyright 2010 Tyslenko Max, Bogatirev Pavel.

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

#include "Debug/EvaluteFunctions.hpp"
#include "Debug/Watch.hpp"

#include "World/Environ/Object.hpp"
#include "Physic/BodyMaterial.hpp"
#include "Physic/BodyState.hpp"
#include "Physic/BodyPart.hpp"

std::string evalute::evalute_material(Watch* watch)
{
    std::string name = watch->MemberName;
    std::string result = "Error, member not found";
    BodyMaterial* m = boost::get<BodyMaterial*>(watch->Object);
    if(name == "Name") result = m->Name;
    else if(name == "KindleTemperature") result = FloatToStr(m->KindleTemperature);
    else if(name == "KindleReceptivity") result = FloatToStr(m->KindleReceptivity);
    else if(name == "FlameTemperature") result = FloatToStr(m->FlameTemperature);
    else if(name == "SelfFlareUpRate") result = FloatToStr(m->SelfFlareUpRate);
    else if(name == "CarbonizeRate") result = FloatToStr(m->CarbonizeRate);
    else if(name == "ElectricalConductivity") result = FloatToStr(m->ElectricalConductivity);
    else if(name == "ThermalReceptivity") result = FloatToStr(m->ThermalReceptivity);
    else if(name == "DampReceptivity") result = FloatToStr(m->DampReceptivity);
    else if(name == "FrozingTemperature") result = FloatToStr(m->FrozingTemperature);
    else if(name == "InflDampnessToFriction") result = FloatToStr(m->InflDampnessToFriction);
    else if(name == "InflDampnessToKindleTemperature") result = FloatToStr(m->InflDampnessToKindleTemperature);
    else if(name == "InflDampnessToMaxKindle") result = FloatToStr(m->InflDampnessToMaxKindle);
    else if(name == "InflDampnessToKindleReceptivity") result = FloatToStr(m->InflDampnessToKindleReceptivity);
    else if(name == "InflDampnessToFrozingTemperature") result = FloatToStr(m->InflDampnessToFrozingTemperature);
    else if(name == "InflCarbonizeLevelToMaxKindle") result = FloatToStr(m->InflCarbonizeLevelToMaxKindle);
    else if(name == "InflCarbonizeLevelToMaxDampness") result = FloatToStr(m->InflCarbonizeLevelToMaxDampness);
    else if(name == "InflCarbonizeLevelToElecticalConductivity") result = FloatToStr(m->InflCarbonizeLevelToElecticalConductivity);
    else if(name == "InflMoistenToKindleLevel") result = FloatToStr(m->InflMoistenToKindleLevel);
    else if(name == "InflTemperatureToDampness") result = FloatToStr(m->InflTemperatureToDampness);

    return result;
}

std::string evalute::evalute_state(Watch* watch)
{
    std::string name = watch->MemberName;
    std::string result = "Error, member not found";
    BodyState* m = boost::get<BodyState*>(watch->Object); 
    if(name == "IsFrozen") result = FloatToStr(m->IsFrozen);
    else if(name == "KindleLevel") result = FloatToStr(m->KindleLevel);
    else if(name == "CarbonizeLevel") result = FloatToStr(m->CarbonizeLevel);
    else if(name == "Temperature") result = FloatToStr(m->Temperature);
    else if(name == "Dampness") result = FloatToStr(m->Dampness);
    return result;
}

std::string evalute::evalute_b2Fixture(Watch* watch)
{
    std::string name = watch->MemberName;
    std::string result = "";
    b2Fixture* m = boost::get<b2Fixture*>(watch->Object); 

    std::string shapeTypePrefix = "", shapeVetexCountPrefix = "",
        shapeVerticesPrefix = "", shapeRadiusPrefix = "", shapeCenterPrefix = "";
    bool shape = false;
    b2Shape* xshape = m->GetShape();
    if(name == "Shape")
    {
        result += "{ ";
        shape = true;
        shapeTypePrefix = "Type = ";
        shapeVetexCountPrefix = ", VertexCount = ";
        shapeVerticesPrefix = ", Vertices = ";
        shapeRadiusPrefix = ", Radius = ";
        shapeCenterPrefix = ", Center = ";
    }
    if(name == "Shape.Type" || shape)
    {
        std::string type = "";
        if(xshape->GetType() == b2Shape::e_unknown) type = "e_unknown";
        else if(xshape->GetType() == b2Shape::e_circle) type = "e_circle";
        else if(xshape->GetType() == b2Shape::e_polygon) type = "e_polygon";
        else if(xshape->GetType() == b2Shape::e_typeCount) type = "e_typeCount";
        result += shapeTypePrefix + type;
    }
    if(name == "Shape.Center" || shape)
    {
        if(xshape->GetType() == b2Shape::e_circle) 
        {
            b2CircleShape* cshape= reinterpret_cast<b2CircleShape*>(xshape);
            result += shapeCenterPrefix + VectorToStr(cshape->m_p);
        }
        else if(xshape->GetType() == b2Shape::e_polygon)
        {
            b2PolygonShape* pshape= reinterpret_cast<b2PolygonShape*>(xshape);
            result += shapeCenterPrefix + VectorToStr(pshape->m_centroid);
        }
    }
    if(name == "Shape.Radius" || shape) result += shapeRadiusPrefix + FloatToStr(m->GetShape()->m_radius);
    if(name == "Shape.VertexCount" || shape)
    {
        if(xshape->GetType() == b2Shape::e_polygon)
        {
            b2PolygonShape* pshape= reinterpret_cast<b2PolygonShape*>(xshape);
            result += shapeVetexCountPrefix + IntToStr(pshape->m_vertexCount);
        }
        else 
        {
            if(!shape)
            {
                result = "shape is circle";
            }
        }
    }
    if(name == "Shape.Vertices" || shape)
    {
        if(xshape->GetType() == b2Shape::e_polygon)
        {
            b2PolygonShape* pshape= reinterpret_cast<b2PolygonShape*>(xshape);
            result += shapeVerticesPrefix;
            for(int i =0; i<pshape->m_vertexCount; ++i)
            {
                result += VectorToStr(pshape->m_vertices[i]);
                result += " ";
            }
        }
        else 
        {
            if(!shape)
            {
                result = "shape is circle";
            }
        }
    }
    if(shape) result += " }";
    else if(name == "IsSensor") result = FloatToStr(m->IsSensor());
    bool filter = false;
    std::string filterCategoryPrefix = "", filterMaskPrefix = "", filterGroupPrefix = "";
    if(name == "Filter")
    {
        filter = true;
        result += "{ ";
        filterCategoryPrefix = "CategoryBits = ";
        filterMaskPrefix = ", MaskBits = ";
        filterGroupPrefix = ", GroupIndex = ";
    }
    if( name == "Filter.CategoryBits" || filter) 
        result += filterCategoryPrefix + IntToStr(m->GetFilterData().categoryBits);
    if( name == "Filter.MaskBits" || filter) 
        result += filterMaskPrefix + HexToStr(m->GetFilterData().maskBits);
    if( name == "Filter.GroupIndex") 
        result += filterGroupPrefix + IntToStr(m->GetFilterData().groupIndex);
    if(filter)
    {
        result += " }";  
    }
    else if(name == "ParentBody")
    {
        Body* body = reinterpret_cast<Body*>(m->GetBody()->GetUserData());
        if(body != NULL)
            result = body->getName();
        else
            result = "Has no atached Body object";
    }
    bool mass = false;
    std::string massValuePrefix = "", massCenterPrefix = "", massRotationInertiaPrefix = "";
    b2MassData massData;
    m->GetMassData(&massData);
    if( name == "Mass")
    {
        mass = true;
        result += "{ ";
        massValuePrefix = "Value = ";
        massCenterPrefix = ", Center = ";
        massRotationInertiaPrefix = ", RotationInertia = ";
    }
    if( name == "Mass.Value" || mass) result += massValuePrefix + FloatToStr(massData.mass);
    if( name == "Mass.Center" || mass) result += massCenterPrefix + VectorToStr(massData.center);
    if( name == "Mass.RotationInertia" || mass) 
        result += massRotationInertiaPrefix + FloatToStr(massData.I);
    if(mass) result += " }";

    else if(name == "Density") result = FloatToStr(m->GetDensity());
    else if(name == "Restitution") result = FloatToStr(m->GetRestitution());
    else if(name == "Friction") result = FloatToStr(m->GetFriction());

    bool aabb = false;
    std::string aabbTopPrefix = "", aabbBottomPrefix = "", aabbLeftPrefix = "", aabbRightPrefix = "";
    if( name == "AABB" )
    {
        aabb = true;
        result += "{ ";
        aabbTopPrefix = "Top = ";
        aabbBottomPrefix = "Bottom = ";
        aabbLeftPrefix = "Left = ";
        aabbRightPrefix = "Right = ";
    }
    if( name == "AABB.Top" || aabb) result += aabbTopPrefix + FloatToStr(m->GetAABB().upperBound.y);
    if( name == "AABB.Bottom" || aabb) result += aabbBottomPrefix + FloatToStr(m->GetAABB().lowerBound.y);
    if( name == "AABB.Left" || aabb) result += aabbLeftPrefix + FloatToStr(m->GetAABB().upperBound.x);
    if( name == "AABB.Right" || aabb) result += aabbRightPrefix + FloatToStr(m->GetAABB().upperBound.x);
    if(aabb) result += " }";

    if( result == "") result = "Erorr, can't evalute";
    return result;
}
std::string evalute::evalute_b2Body(Watch* watch)
{
    std::string name = watch->MemberName;
    std::string result = "";
    b2Body* body = boost::get<b2Body*>(watch->Object); 

    if( name == "Type")
    {
        b2BodyType type = body->GetType();
        switch(type)
        {
            case b2_staticBody: result = "StaticBody"; break;
            case b2_dynamicBody: result = "DynamicBody"; break;
            case b2_kinematicBody: result = "KinematicBody"; break;
            default: result = "Unknown type";
        }
    }
    else if(name == "Position") result = VectorToStr(body->GetPosition());
    else if(name == "Angle") result = FloatToStr(body->GetAngle());
    else if(name == "WorldCenter") result = VectorToStr(body->GetWorldCenter());
    else if(name == "LocalCenter") result = VectorToStr(body->GetLocalCenter());
    else if(name == "LinearVelocity") result = VectorToStr(body->GetLinearVelocity());
    else if(name == "AngularVelocity") result = FloatToStr(body->GetAngularVelocity());
    else if(name == "Inertia") result = FloatToStr(body->GetInertia());
    else if(name == "LinearDamping") result = FloatToStr(body->GetLinearDamping());
    else if(name == "AngularDamping")result = FloatToStr(body->GetAngularDamping());
    else if(name == "Bullet") result = BoolToStr(body->IsBullet());
    else if(name == "SleepingAllowed") result = BoolToStr(body->IsSleepingAllowed());
    else if(name == "Awake") result = BoolToStr(body->IsAwake());
    else if(name == "Active") result = BoolToStr(body->IsActive());
    else if(name == "FixedRotation") result = BoolToStr(body->IsFixedRotation());
    bool mass = false;
    std::string massValuePrefix = "", massCenterPrefix = "", massRotationInertiaPrefix = "";
    b2MassData massData;
    body->GetMassData(&massData);
    if( name == "Mass")
    {
        mass = true;
        result = "{ ";
        massValuePrefix = "Value = ";
        massCenterPrefix = ", Center = ";
        massRotationInertiaPrefix = ", RotationInertia = ";
    }
    if( name == "Mass.Value" || mass) result += massValuePrefix + FloatToStr(massData.mass);
    if( name == "Mass.Center" || mass) result += massCenterPrefix + VectorToStr(massData.center);
    if( name == "Mass.RotationInertia" || mass) 
        result += massRotationInertiaPrefix + FloatToStr(massData.I);
    if(mass) result += " }";

    if(result == "") result = "Can't evalute";

    return result;
}

std::string evalute::evalute_BodyPart(Watch* watch)
{
    std::string name = watch->MemberName;
    std::string result = "";
    BodyPart* part = boost::get<BodyPart*>(watch->Object); 

    if( name == "IsDefaultMaterial") result = BoolToStr(part->IsDefaultMaterial());
    else if( name == "MaxKindleLevel") result = FloatToStr(part->getMaxKindleLevel());
    else if( name == "MaxDampness") result = FloatToStr(part->getMaxDampness());
    else if( name == "AcceptsCord") result = BoolToStr(part->getAcceptsCord());
    else if( name == "Name") result = part->getName();

    if(result == "") result = "Can't evalute";
    return result;
}

std::string evalute::evalute_Body(Watch* watch)
{
    std::string name = watch->MemberName;
    std::string result = "";
    Body* body = boost::get<Body*>(watch->Object); 
    if( name == "Buoyancy") result = FloatToStr(body->getBuoyancy());
    else if( name == "Name") result = body->getName();

    if(result == "") result = "Can't evalute";
    return result;
}

std::string evalute::evalute_EnvironObject(Watch* watch)
{
    std::string name = watch->MemberName;
    std::string result = "";
    EnvironObject* obj = boost::get<EnvironObject*>(watch->Object); 
    if(name == "Enabled") result = BoolToStr(obj->getEnabled());
    else if(name == "Limit") result = IntToStr(obj->getLimit());

    if(result == "") result = "Can't evalute";

    return result;
}


std::string evalute::evalute_elapsed(Watch* watch)
{
    ApplicationManager* app = boost::get<ApplicationManager*>(watch->Object); 
    return IntToStr(app->getElapsed());
}
