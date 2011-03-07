#include "World/Objects/Foreground.hpp"

Foreground::Foreground()
{
    mGC = appManager().getGraphic();
    mYPos = 0;
    mFixed = true;
    mActualSize = 0;
}


void Foreground::init() {}
void Foreground::update(float elapsed){}

void Foreground::updateVisual(float newX, float newY)
{
    if (!mFixed) mYPos = levelManager().getCamViewport().top;
    mForeground.draw(mGC, levelManager().getCamViewport().left * 0.5, mYPos);
}

CL_Pointf Foreground::getPosition()
{
    return CL_Pointf(0, 0);
}
void Foreground::setPosition(CL_Pointf newPos) {}
CL_Rectf Foreground::getRectangle() 
{
    return CL_Rectf(0, 0, ScreenResolutionX, ScreenResolutionY);
}

std::vector<std::string> Foreground::getTextureInfo()
{
    std::vector<std::string> res;
    res.push_back(mSection);
    res.push_back(mTexture);
    return res;
}

void Foreground::setVisual(std::string textureName, std::string textureSection)
{
    setVisual(textureName, textureSection, -1, -1);
}

void Foreground::setFixed(bool state)
{
    mFixed = state;
}

bool Foreground::getFixed()
{
    return mFixed;
}

void Foreground::setActualSize(uint16_t size)
{
    mActualSize = size;
}

uint16_t Foreground::getActualSize()
{
    return mActualSize;
}

void Foreground::setVisual(std::string textureName, std::string textureSection, float width, float height)
{
    mForeground = resourceManager().getImage(textureSection, textureName);
    mTexture = textureName;
    mSection = textureSection;

    if(!mForeground.is_null())
    {
        float koefX, koefY;
        koefX = koefY = 1;
        if(width > 0) koefX = width / mForeground.get_width();
        if(height > 0) koefY = height / mForeground.get_height();
        mForeground.set_scale(koefX, koefY);
    }
}

boost::shared_ptr<Object> Foreground::ParseForeground(CL_DomElement *tag, std::string &desc)
{
    Foreground *result = new Foreground();

    CL_DomNodeList VisualTags = tag->get_elements_by_tag_name("Visual");
    std::string textureName;
    std::string textureSection;
    float width = -1;
    float height = -1;

    CL_DomNodeList childList = VisualTags.item(0).get_child_nodes();
    for (int i=0; i < childList.get_length(); ++i)
    {
        CL_DomElement tag2 = childList.item(i).to_element();
        if (tag2.get_node_name() == "Size")
        {
            width = boost::lexical_cast<float>(tag2.get_attribute("width").c_str());
            height = boost::lexical_cast<float>(tag2.get_attribute("height").c_str());
        }
        if (tag2.get_node_name() == "Texture")
        {
            textureSection = tag2.get_attribute("section").c_str();
            textureName = tag2.get_attribute("name").c_str();
        }
    }
    result->setVisual(textureName, textureSection, width, height);

    uint16_t actualSize = 0; // In percents
    bool fixed = true;

    CL_DomElement actualElement = tag->get_elements_by_tag_name("ActualSize").item(0).to_element();
    if (actualElement.has_attribute("value"))
        actualSize = boost::lexical_cast<uint16_t>(actualElement.get_attribute("value").c_str());

    CL_DomElement fixedElement = tag->get_elements_by_tag_name("Fixed").item(0).to_element();
    if (fixedElement.has_attribute("value"))
        fixed = (fixedElement.get_attribute("value") == "true");

    result->setActualSize(actualSize);
    result->setFixed(fixed);

    return boost::shared_ptr<Object>(result);
}

bool Foreground::checkCollision(CL_Pointf point) { return false; }

