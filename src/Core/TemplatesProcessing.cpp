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
}

DocumentPtr TemplatesProcessor::processTemplates(const std::string& sceneFile)
{
    CL_File fileHandle = CL_File(sceneFile);
    DocumentPtr document = DocumentPtr(new CL_DomDocument(fileHandle));
    mSceneDocument = document;
    mSceneFile = sceneFile;
    CL_DomElement document_element = mSceneDocument->get_document_element();
    processTag(&document_element);
    mSceneDocument.reset();
    mSceneFile = "";
    return document;
}

void TemplatesProcessor::processTag(CL_DomElement* tag)
{
    std::cout << "Process tag " << tag->get_node_name().c_str() << std::endl;
    if(tag->has_attribute("template"))
    {
         std::string tplLocation = tag->to_element().get_attribute("template").c_str();
         CL_DomElement tagTemplate = getTemplate(tplLocation);
         applyTemplate(tag, tagTemplate);
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

CL_DomElement TemplatesProcessor::getTemplate(const std::string& spec)
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
        CL_DomElement tpl = getSingleTemplate(templateSpec);
        if(prevTemplate != NULL)
        {
            applyTemplate(&tpl, *prevTemplate);
        }
        prevTemplate = &tpl;
    }
    return *prevTemplate;
}

// Without multiply specifiers
CL_DomElement TemplatesProcessor::getSingleTemplate(const std::string& _spec)
{
    std::cout << "Get single template " << _spec << std::endl;
    std::string spec(_spec);
    boost::trim(spec);

    // No file, no local
    if(spec.find("/") == spec.npos && spec.find("\\") == spec.npos)
    {
        // Check local
        try
        {
            return getLocalTemplate(spec);
        }
        catch(CL_Exception&)
        {

            // Check in Include files

        }
    }
    else if(spec.find("local") == 0)
    {
        std::vector<std::string> parts;
        boost::split(parts, spec, boost::is_any_of("/\\"));
        if(parts.size() == 2)
        {
            std::string templateName = parts.back();
            return getLocalTemplate(templateName);
        }
        else
        {
            throw CL_Exception("Invalid use of keyword 'local': " + spec);
        }
    }
    else
    {
        unsigned int lastSlashInd1 = spec.find_last_of('/');
        unsigned int lastSlashInd2 = spec.find_last_of('\\');
        int lastSlashInd = 0;
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
        std::string path = spec.substr(0, lastSlashInd-1);
        std::string name = spec.substr(lastSlashInd +1, spec.size() - lastSlashInd +1);
        return getTemplateFromFile(path, name);
    }
}

std::vector<std::string> TemplatesProcessor::getIncludedFiles(CL_DomElement templatesTag)
{
    std::vector<std::string> files;
    CL_DomNodeList includeTags = templatesTag.get_elements_by_tag_name("Include");
    for(int i=0; i<includeTags.get_length(); ++i)
    {
        CL_DomElement includeTag = includeTags.item(i).to_element();
        std::string file = includeTag.get_node_value();
        std::string filePath = getTemplateFilePath(file);
        files.push_back(filePath);
    }
    return files;
}

CL_DomElement TemplatesProcessor::getLocalTemplate(const std::string& name)
{
    std::cout << std::endl << "Get local template " << name << std::endl;

    return CL_DomElement();
}

std::string TemplatesProcessor::getTemplateFilePath(const std::string& file)
{
    std::string resultPath = "";

    // Find in scene dir
    boost::filesystem::path scenePath(mSceneFile);
    std::string sceneDir = scenePath.native_directory_string();
    boost::filesystem::path templatePath(sceneDir);
    templatePath /= file;
    if( boost::filesystem::exists(templatePath) )
    {
        resultPath = std::string(templatePath.string());
        std::cout << "Found in scene dir" << std::endl;
    }
    // Find in media/templates dir
    else
    {
        std::string templatesDirRelativePath = "media/templates/" + file;
        if( boost::filesystem::exists(templatesDirRelativePath) )
        {
            resultPath = templatesDirRelativePath;
            std::cout << "Found in templates dirrectory" << std::endl;
        }
        else
        {
            throw CL_Exception( "Template file '" + file + "' not found.");
        }
    }
    return resultPath;
}

CL_DomElement TemplatesProcessor::getTemplateFromFile(const std::string& file, const std::string& name)
{
    std::cout << std::endl << "Get template from file " << file << " template name: " << name << std::endl;
    // Check recursively in Include files with test on cicle include
    std::string resultPath = getTemplateFilePath(file);
    DocumentPtr document;
    std::map<std::string, DocumentPtr>::iterator documentIt= mFileToDocument.find(resultPath);
    if( documentIt != mFileToDocument.end())
    {
        document = documentIt->second;
    }
    else
    {
        CL_File fileHandle = CL_File(resultPath);
        document = DocumentPtr(new CL_DomDocument(fileHandle));
        mFileToDocument.insert(std::pair<std::string, DocumentPtr>(resultPath, document));
    }
    std::vector<CL_DomNode> templatesElements = document->select_nodes("Templates/Template[@name='" + name +"']");
    if(templatesElements.size() == 1)
    {
        return templatesElements.front().to_element();
    }
    else if( templatesElements.size() == 0)
    {

        std::vector<std::string> includedFiles = getIncludedFiles(document->get_document_element());
        std::vector<std::string>::iterator filesIt;
        for(filesIt = includedFiles.begin(); filesIt != includedFiles.end(); ++filesIt)
        {
            try
            {
                return getTemplateFromFile(*filesIt, name);
            }
            catch(TemplateFileNotFound&)
            {
            }
        }
        throw TemplateFileNotFound( "Template " + name +" not found in " + file + ".");
    }
    else if(templatesElements.size() > 1)
    {
        throw CL_Exception( "Corrupted template file: there are several templates with equal names. File: "
            + file +"; Template name: " + name + ".");
    }
}


void TemplatesProcessor::applyTemplate(CL_DomElement* tag, const CL_DomElement& templateTag)
{
    std::cout <<std::endl << "Applying template.." << std::endl;
}










