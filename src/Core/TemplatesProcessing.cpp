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

#include "Core/TemplatesProcessing.hpp"

TemplatesProcessor::TemplatesProcessor()
{
    mSceneDocument = NULL;
    mErrorMessage = "";
}

std::string TemplatesProcessor::processTemplates(CL_DomDocument* scene) // No way to return reference, it's bad
{
    mErrorMessage = "";
    mSceneDocument = scene;
    CL_DomElement document_element = mSceneDocument->get_document_element();
    processTag(&document_element);
    mSceneDocument = NULL;
    return mErrorMessage;
}

void TemplatesProcessor::processTag(CL_DomElement* tag)
{
    std::cout << "Process tag " << tag->get_node_name().c_str() << std::endl;
    if(tag->has_attribute("template")) //Runtime fail!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    {
         std::string tplLocation = tag->to_element().get_attribute("template").c_str();
         CL_DomElement* tagTemplate = getTemplate(tplLocation);
         if( tagTemplate != NULL)
         {
            applyTemplate(tag, *tagTemplate);
         }
    }

    CL_DomNodeList childList = tag->get_child_nodes();

    for (int i=0; i < childList.get_length(); ++i)
    {
        CL_DomNode childNode = childList.item(i);
        if(childNode.is_element())
        {
            CL_DomElement child = childNode.to_element();
            processTag(&child);
        }
    }
}

CL_DomElement* TemplatesProcessor::getTemplate(const std::string& spec)
{
    std::cout << "Get template " <<spec <<std::endl;
    std::vector<std::string> parts;
    boost::split(parts, spec, boost::is_any_of(","));
    CL_DomElement* prevTemplate = NULL;
    std::vector<std::string>::iterator partsIt;
    for(partsIt = parts.begin(); partsIt != parts.end(); ++partsIt)
    {
        std::string templateSpec = *partsIt;
        boost::trim(templateSpec);
        CL_DomElement* tpl = getSingleTemplate(templateSpec);
        if(prevTemplate != NULL)
        {
            applyTemplate(tpl, *prevTemplate);
        }
        prevTemplate = tpl;
    }
    return prevTemplate;
}

// Without multiply specifiers
CL_DomElement* TemplatesProcessor::getSingleTemplate(const std::string& _spec)
{
    std::cout << "Get single template " << _spec << std::endl;
    std::string spec(_spec);
    boost::trim(spec);

    CL_DomElement* tpl = NULL;

    // No file, no local
    if(spec.find("/") == spec.npos && spec.find("\\") == spec.npos)
    {
        // Check local
        tpl = getLocalTemplate(spec);
        if(tpl == NULL)
        {
            // Check in Include files

            // Check recursively in Include files with test on cicle include
        }
    }
    else if(spec.find("local") == 0)
    {
        std::vector<std::string> parts;
        boost::split(parts, spec, boost::is_any_of("/\\"));
        if(parts.size() == 2)
        {
            std::string templateName = parts.back();
            tpl = getLocalTemplate(templateName);
        }
        else
        {
            mErrorMessage += "Invalid use of keyword 'local'";
            tpl = NULL;
        }
    }
    else
    {
        unsigned int lastSlashInd1 = spec.find_last_of('/');
        unsigned int lastSlashInd2 = spec.find_last_of('\\');
        unsigned int lastSlashInd = 0;
        if(lastSlashInd1 == spec.npos)
        {
            lastSlashInd = lastSlashInd2;
        }
        else if(lastSlashInd2 == spec.npos)
        {
            lastSlashInd = lastSlashInd1;
        }
        else
        {
            lastSlashInd = (lastSlashInd1 > lastSlashInd2)? lastSlashInd1 : lastSlashInd2;
        }
        std::string path = spec.substr(0, lastSlashInd);
        std::string name = spec.substr(lastSlashInd +1, spec.size() - lastSlashInd +1);
        tpl = getTemplateFromFile(path, name);
    }
    return tpl;
}

CL_DomElement* TemplatesProcessor::getLocalTemplate(const std::string& name)
{
    std::cout << std::endl << "Get local template " << name << std::endl;
    return NULL;
}



CL_DomElement* TemplatesProcessor::getTemplateFromFile(const std::string& file, const std::string& name)
{
    std::cout << std::endl << "Get template from file " << file << " template name: " << name << std::endl;
    return NULL;
}


void TemplatesProcessor::applyTemplate(CL_DomElement* tag, const CL_DomElement& templateTag)
{
    std::cout <<std::endl << "Applying template.." << std::endl;
}










