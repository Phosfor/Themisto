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

#include "Scripts/Core/TypesConverters.hpp"

// Pointer to functions for processing overloaded functions

// CL_VEC2
// INT
CL_Vec2<int> &(CL_Vec2<int>::*Vec2iNorm)() = &CL_Vec2<int>::normalize;
int (CL_Vec2<int>::*Vec2iDot)(const CL_Vec2<int>&) const = &CL_Vec2<int>::dot;
CL_Vec2<int> &(CL_Vec2<int>::*Vec2iRound)() = &CL_Vec2<int>::round;
// FLOAT
CL_Vec2<float> &(CL_Vec2<float>::*Vec2fNorm)() = &CL_Vec2<float>::normalize;
float (CL_Vec2<float>::*Vec2fDot)(const CL_Vec2<float>&) const = &CL_Vec2<float>::dot;
CL_Vec2<float> &(CL_Vec2<float>::*Vec2fRound)() = &CL_Vec2<float>::round;

// CL_Draw::gradient_fill
void (*GradientFill_1)(CL_GraphicContext&,
        float, float, float, float, const CL_Gradient&) = CL_Draw::gradient_fill;
void (*GradientFill_2)(CL_GraphicContext&,
        const CL_Pointf&, const CL_Pointf&, const CL_Gradient&) = CL_Draw::gradient_fill;
void (*GradientFill_3)(CL_GraphicContext&,
        const CL_Rectf&, const CL_Gradient&) = CL_Draw::gradient_fill;

// CL_Draw::line
void (*DrawLine_1)(CL_GraphicContext&,
        float, float, float, float, const CL_Colorf&) = CL_Draw::line;
void (*DrawLine_2)(CL_GraphicContext&,
        const CL_Pointf&, const CL_Pointf&, const CL_Colorf&) = CL_Draw::line;

// CL_Draw::point
void (*DrawPoint_1)(CL_GraphicContext&, float, float, const CL_Colorf&) = CL_Draw::point;

// CL_Draw::circle
void (*DrawCircle_1)(CL_GraphicContext&, float, float, float, const CL_Colorf&) = CL_Draw::circle;

// CL_DomElement: has_attribute, get_attribute
bool (CL_DomElement::*HasAttribute)(const CL_DomString&) const = &CL_DomElement::has_attribute;
CL_DomString (CL_DomElement::*GetAttribute)(const CL_DomString&) const = &CL_DomElement::get_attribute;

// CL_Image
void (CL_Image::*ImageDraw_1)(CL_GraphicContext&, float, float) const = &CL_Image::draw;
void (CL_Image::*SetColor)(const CL_Colorf&) = &CL_Image::set_color;

void (CL_Sprite::*SpriteDraw_1)(CL_GraphicContext&, float, float) = &CL_Sprite::draw;
void (CL_Sprite::*SetColorSprite)(const CL_Colorf&) = &CL_Sprite::set_color;

// CL_GUI:
void (CL_GUIManager::*SetCss)(const CL_String&) = &CL_GUIManager::set_css_document;

CL_Colorf ColorRGB(float r, float g, float b)
{
    return CL_Colorf(r/255.0f, g/255.0f, b/255.0f);
}

CL_Rectf BoundingRect(CL_Rectf first, CL_Rectf second)
{
    return static_cast<CL_Rectf>(first.bounding_rect(second));
}

BOOST_PYTHON_MODULE(TypesConverters)
{
    using namespace ScriptTypesConverters;
    bp::def("ColorRGB", &ColorRGB);

    /////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////// TYPES CONVERTERS /////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////
    // std::vector
    bp::to_python_converter<std::vector<std::string>, VecToList<std::string>>();
    bp::to_python_converter<std::vector<int>, VecToList<int>>();
    bp::to_python_converter<std::vector<float>, VecToList<float>>();

    // std::pair
    bp::to_python_converter<std::pair<int, int>, PairToTuple<int, int>>();
    bp::to_python_converter<std::pair<std::string, std::string>, PairToTuple<std::string, std::string>>();

    // CL_String8
    bp::to_python_converter<CL_String8, cl_string8_to_python_str>();
    cl_string8_from_python_str();


    /////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////// EXPORTING CLASSES ////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////

    /////////////////////////////////// CL_POINT ////////////////////////////////////////
    // CL_Point [integer] -------------------------------------------------------------
    bp::class_<CL_Point>("CL_Point", bp::init<int, int>())
        .def(bp::init< CL_Vec2<int> >())
        .def(bp::self_ns::str(bp::self_ns::self));  // Export __str__

    // CL_Point [float] ---------------------------------------------------------------
    bp::class_<CL_Pointf>("CL_Pointf", bp::init<float, float>())
        .def(bp::init< CL_Vec2<float> >())
        .def_readonly("x", &CL_Pointf::x)
        .def_readonly("y", &CL_Pointf::y)
        .def(bp::self_ns::str(bp::self_ns::self));  // Export __str__

    // CL_Colorf [float] --------------------------------------------------------------
    bp::class_<CL_Colorf>("CL_Colorf", bp::init<float, float, float, float>())
        .def(bp::init<float, float, float>())
        .def_readonly("r", &CL_Colorf::r)
        .def_readonly("g", &CL_Colorf::g)
        .def_readonly("b", &CL_Colorf::b)
        .def_readonly("a", &CL_Colorf::a)
        .def("SetAlpha", &CL_Colorf::set_alpha);

    // CL_Pen -----------------------------------------------------------------------
    bp::class_<CL_Pen>("CL_Pen")
        .def("EnableLineAntialiasing", &CL_Pen::enable_line_antialiasing);

    // CL_Gradient -------------------------------------------------------------------
    bp::class_<CL_Gradient>("CL_Gradient", bp::init<const CL_Colorf&, const CL_Colorf&,
            const CL_Colorf&, const CL_Colorf&>())
        .def(bp::init<const CL_Colorf&, const CL_Colorf&>());

    // CL_Draw -----------------------------------------------------------------------
    bp::class_<CL_Draw>("CL_Draw")
        .def("GradientFill", GradientFill_1)
        .def("GradientFill", GradientFill_2)
        .def("GradientFill", GradientFill_3)
        .staticmethod("GradientFill")
        .def("Line", DrawLine_1)
        .def("Line", DrawLine_2)
        .staticmethod("Line")
        .def("Point", DrawPoint_1)
        .staticmethod("Point")
        .def("Circle", DrawCircle_1)
        .staticmethod("Circle");

    // CL_Image ----------------------------------------------------------------------
    bp::class_<CL_Image>("CL_Image")
        .def("SetScale", &CL_Image::set_scale)
        .def("SetAlpha", &CL_Image::set_alpha)
        .def("SetAlignment", &CL_Image::set_alignment, SetAlignmentImageOverloads())
        .def("Draw", ImageDraw_1)
        .def("IsNull", &CL_Image::is_null)
        .def("GetColor", &CL_Image::get_color)
        .def("SetColor", SetColor)
        .def("GetWidth", &CL_Image::get_width)
        .def("GetHeight", &CL_Image::get_height);

    // CL_Sprite ---------------------------------------------------------------------
    bp::class_<CL_Sprite>("CL_Sprite")
        .def("SetScale", &CL_Sprite::set_scale)
        .def("SetAlpha", &CL_Sprite::set_alpha)
        .def("SetAngle", &CL_Sprite::set_angle)
        .def("RotatePitch", &CL_Sprite::rotate_pitch)
        .def("SetAlignment", &CL_Sprite::set_alignment, SetAlignmentSpriteOverloads())
        .def("SetLinearFilter", &CL_Sprite::set_linear_filter)
        .def("SetRotationHotspot", &CL_Sprite::set_rotation_hotspot)
        .def("Draw", SpriteDraw_1)
        .def("IsNull", &CL_Sprite::is_null)
        .def("GetColor", &CL_Sprite::get_color)
        .def("SetColor", SetColorSprite)
        .def("GetWidth", &CL_Sprite::get_width)
        .def("GetHeight", &CL_Sprite::get_height);

    // CL_Origin ---------------------------------------------------------------------
    bp::enum_<CL_Origin>("CL_Origin")
        .value("origin_top_center", origin_top_center)
        .value("origin_top_right", origin_top_right)
        .value("origin_top_left", origin_top_left)

        .value("origin_center_left", origin_center_left)
        .value("origin_center_right", origin_center_right)
        .value("origin_center", origin_center)

        .value("origin_bottom_left", origin_bottom_left)
        .value("origin_bottom_right", origin_bottom_right)
        .value("origin_bottom_center", origin_bottom_center)
        .export_values();

    // CL_Angle ----------------------------------------------------------------------
    bp::class_<CL_Angle>("CL_Angle")
        .def("FromRadians", CL_Angle::from_radians).staticmethod("FromRadians")
        .def("FromDegrees", CL_Angle::from_degrees).staticmethod("FromDegrees")
        .def("Normalize", &CL_Angle::normalize, bp::return_value_policy<bp::copy_non_const_reference>())
        .def("ToDegrees", &CL_Angle::to_degrees)
        .def("ToRadians", &CL_Angle::to_radians)

        // Export overriden operators
        .def(bp::self += CL_Angle()).def(bp::self -= CL_Angle())
        .def(bp::self + CL_Angle()) .def(bp::self - CL_Angle())
        .def(bp::self *= CL_Angle()).def(bp::self /= CL_Angle())
        .def(bp::self * CL_Angle()) .def(bp::self / CL_Angle())
        .def(bp::self == CL_Angle()).def(bp::self != CL_Angle())
        .def(bp::self < CL_Angle()) .def(bp::self > CL_Angle())
        .def(bp::self <= CL_Angle()) .def(bp::self >= CL_Angle());

    /////////////////////////////////// CL_VECTOR /////////////////////////////////////
    // CL_Vec2 [integer] --------------------------------------------------------------
    bp::class_< CL_Vec2<int> >("CL_Vec2i", bp::init<int, int>())
        .def("Length", &CL_Vec2<int>::length)
        .def("Dot", Vec2iDot)
        .def("Normalize", Vec2iNorm, bp::return_value_policy<bp::copy_non_const_reference>())
        .def("Angle", &CL_Vec2<int>::angle)
        .def("Distance", &CL_Vec2<int>::distance)
        .def("Round", Vec2iRound, bp::return_value_policy<bp::copy_non_const_reference>())
        .def(bp::self_ns::str(bp::self_ns::self))  // Export __str__

        // Export overriden operators
        .def(bp::self += CL_Vec2<int>()).def(bp::self -= CL_Vec2<int>())
        .def(bp::self + CL_Vec2<int>()) .def(bp::self - CL_Vec2<int>())
        .def(bp::self *= CL_Vec2<int>()).def(bp::self /= CL_Vec2<int>())
        .def(bp::self * CL_Vec2<int>()) .def(bp::self / CL_Vec2<int>())
        .def(bp::self == CL_Vec2<int>()).def(bp::self != CL_Vec2<int>())
        .def(bp::self < CL_Vec2<int>()) .def(bp::self > CL_Vec2<int>())
        .def(bp::self <= CL_Vec2<int>()) .def(bp::self >= CL_Vec2<int>());

    // CL_Vec2 [float] ----------------------------------------------------------------
    bp::class_< CL_Vec2<float> >("CL_Vec2f", bp::init<float, float>())
    .def("Length", &CL_Vec2<float>::length)
    .def("Dot", Vec2fDot)
    .def("Normalize", Vec2fNorm, bp::return_value_policy<bp::copy_non_const_reference>())
    .def("Angle", &CL_Vec2<float>::angle)
    .def("Distance", &CL_Vec2<float>::distance)
    .def("Round", Vec2fRound, bp::return_value_policy<bp::copy_non_const_reference>())
    .def(bp::self_ns::str(bp::self_ns::self))  // Export __str__

    // Export overriden operators
    .def(bp::self += CL_Vec2<float>()).def(bp::self -= CL_Vec2<float>())
    .def(bp::self + CL_Vec2<float>()) .def(bp::self - CL_Vec2<float>())
    .def(bp::self *= CL_Vec2<float>()).def(bp::self /= CL_Vec2<float>())
    .def(bp::self * CL_Vec2<float>()) .def(bp::self / CL_Vec2<float>())
    .def(bp::self == CL_Vec2<float>()).def(bp::self != CL_Vec2<float>())
    .def(bp::self < CL_Vec2<float>()) .def(bp::self > CL_Vec2<float>())
    .def(bp::self <= CL_Vec2<float>()) .def(bp::self >= CL_Vec2<float>());

    /////////////////////////////////// CL_SIZE ////////////////////////////////////////
    // CL_Size [integer] --------------------------------------------------------------
    bp::class_<CL_Size>("CL_Size", bp::init<int, int>())
        .def(bp::init< CL_Sizex<int> >())
        .def(bp::init< CL_Sizex<float> >())

        .def_readwrite("width", &CL_Size::width)
        .def_readwrite("height", &CL_Size::height)

        .def(bp::self_ns::str(bp::self_ns::self))  // Export __str__

        // Export overriden operators
        .def(bp::self += int()).def(bp::self -= int())
        .def(bp::self + int()) .def(bp::self - int())
        .def(bp::self *= int()).def(bp::self /= int())
        .def(bp::self * int()) .def(bp::self / int())
        .def(bp::self == CL_Size()).def(bp::self != CL_Size());

    // CL_Size [float] -----------------------------------------------------------------
    bp::class_<CL_Sizef>("CL_Sizef", bp::init<float, float>())
        .def(bp::init< CL_Sizex<int> >())
        .def(bp::init< CL_Sizex<float> >())

        .def_readwrite("width", &CL_Sizef::width)
        .def_readwrite("height", &CL_Sizef::height)

        .def(bp::self_ns::str(bp::self_ns::self))  // Export __str__

        // Export overriden operators
        .def(bp::self += float()).def(bp::self -= float())
        .def(bp::self + float()) .def(bp::self - float())
        .def(bp::self *= float()).def(bp::self /= float())
        .def(bp::self * float()) .def(bp::self / float())
        .def(bp::self == CL_Sizef()).def(bp::self != CL_Sizef());

    /////////////////////////////////// CL_RECT ////////////////////////////////////////
    // CL_Rect [integer] ---------------------------------------------------------------
    bp::class_<CL_Rect>("CL_Recti", bp::init<int, int, int, int>())
        .def(bp::init< CL_Rectx<int> >())
        .def(bp::init< CL_Rectx<float> >())
        .def(bp::init< CL_Sizex<int> >())

        .def_readonly("left", &CL_Rect::left)
        .def_readonly("top", &CL_Rect::top)
        .def_readonly("right", &CL_Rect::right)
        .def_readonly("bottom", &CL_Rect::bottom)

        .def(bp::self_ns::str(bp::self_ns::self));  // Export __str__

    // CL_Rectf [float] ---------------------------------------------------------------
    bp::def("BoundingRect", &BoundingRect);
    bp::class_<CL_Rectf>("CL_Rectf", bp::init<float, float, float, float>())
        .def(bp::init< CL_Rectx<float> >())
        .def(bp::init< CL_Rectx<float> >())
        .def(bp::init< CL_Sizex<float> >())

        .def_readonly("left", &CL_Rectx<float>::left)
        .def_readonly("top", &CL_Rectx<float>::top)
        .def_readonly("right", &CL_Rectx<float>::right)
        .def_readonly("bottom", &CL_Rectx<float>::bottom)

        .def(bp::self_ns::str(bp::self_ns::self));  // Export __str__

    bp::class_<CL_DisplayWindow>("CL_DisplayWindow")
        .def("GetMinimumSize", &CL_DisplayWindow::get_minimum_size, GetMinimumSizeOverloads());

    bp::class_<CL_GraphicContext>("CL_GraphicContext")
        .def("SetPen", &CL_GraphicContext::set_pen)
        .def("ResetPen", &CL_GraphicContext::reset_pen);

    bp::class_<CL_DomNodeList>("CL_DomNodeList")
        .def("Item", &CL_DomNodeList::item)
        .def("GetLength", &CL_DomNodeList::get_length);

    bp::class_<CL_DomNode>("CL_DomNode")
        .def("GetChildNodes", &CL_DomNode::get_child_nodes)
        .def("ToElement", &CL_DomNode::to_element);

    bp::class_<CL_DomElement>("CL_DomElement")
        .def("HasAttribute", HasAttribute)
        .def("GetAttribute", GetAttribute)
        .def("GetElementsByTagName", &CL_DomElement::get_elements_by_tag_name)
        .def("GetChildNodes", &CL_DomElement::get_child_nodes)
        .def("GetNodeName", &CL_DomElement::get_node_name);

    bp::class_<CL_StringRef8>("CL_DomString", bp::init<const std::string&>())
        .def("CStr", &CL_StringRef8::c_str);

    bp::class_<Camera>("Camera", bp::init<CL_Size>())
        .def("SetViewport", &Camera::setViewport)
        .def("GetViewport", &Camera::getViewport)

        .def("Translate", &Camera::translate)
        .def("GetAbsolutePos", &Camera::getAbsolutePos)

        .def("GetSpeed", &Camera::getSpeed)
        .def("SetSpeed", &Camera::setSpeed)

        .def("GetDrawDebugData", &Camera::getDrawDebugData)
        .def("SetDrawDebugData", &Camera::setDrawDebugData);

    // Used for CL_Slot && CL_Signal (Input)
    bp::class_<CL_InputEvent>("CL_InputEvent")
        .def_readonly("Id", &CL_InputEvent::id)
        .def_readonly("Str", &CL_InputEvent::str)
        .def_readonly("Alt", &CL_InputEvent::alt)
        .def_readonly("Ctrl", &CL_InputEvent::ctrl)
        .def_readonly("Shift", &CL_InputEvent::shift);

    bp::class_<CL_InputState>("CL_InputState");

    bp::class_<CL_String8>("CL_String8", bp::init<const std::string &>())
        .def("CStr", &CL_String8::c_str);

    // CLANGUI ===================================================================
    bp::class_<CL_GUIManager>("CL_GUIManager")
        .def("SetCssDocument", SetCss);

    bp::class_<CL_GUIComponent, boost::noncopyable>("CL_GUIComponent", bp::init<CL_GUIComponent*>());

    bp::class_<CL_Label, boost::noncopyable>("CL_Label", bp::init<CL_GUIComponent*>())
        .def("SetGeometry", &CL_Label::set_geometry)
        .def("SetClassName", &CL_Label::set_class_name)
        .def("SetText", &CL_Label::set_text)
        .def("SetVisible", &CL_Label::set_visible);

    bp::class_<CL_Slider, boost::noncopyable>("CL_Slider", bp::init<CL_GUIComponent*>())
        .def("SetGeometry", &CL_Slider::set_geometry)
        .def("SetHorizontal", &CL_Slider::set_horizontal)
        .def("SetVisible", &CL_Slider::set_visible)
        .def("SetPosition", &CL_Slider::set_position)
        .def("SetRanges", &CL_Slider::set_ranges);
}
