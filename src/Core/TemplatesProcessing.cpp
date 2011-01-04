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
    LOG("Processing templates at " + sceneFile);
    CL_File fileHandle = CL_File(sceneFile);
    DocumentPtr document = DocumentPtr(new CL_DomDocument(fileHandle));
    CL_DomElement document_element = document->get_document_element();
    processTag(&document_element, document, sceneFile);
    return document;
}

void TemplatesProcessor::processTag(CL_DomElement* tag, DocumentPtr baseDocument, const std::string& baseFile)
{
    LOG(std::string("Process tag ") + tag->get_node_name().c_str());
    if(tag->has_attribute("template"))
    {
         std::string tplLocation = tag->to_element().get_attribute("template").c_str();
         CL_DomElement tagTemplate = getTemplate(tplLocation, baseDocument, baseFile);
         applyTemplate(tag, tagTemplate);
    }

    CL_DomNodeList childList = tag->get_child_nodes();

    for (int i=0; i < childList.get_length(); ++i)
    {
        CL_DomNode childNode = childList.item(i);
        if(childNode.is_element())
        {
            CL_DomElement child = childNode.to_element();
            processTag(&child, baseDocument, baseFile);
        }
    }
}

CL_DomElement TemplatesProcessor::getTemplate(const std::string& spec, DocumentPtr baseDocument,
        const std::string& baseFile)
{
    LOG("Get template " + spec);
    std::vector<std::string> parts;
    boost::split(parts, spec, boost::is_any_of(","));
    boost::shared_ptr<CL_DomElement> prevTemplate;
    bool first = true;
    std::vector<std::string>::iterator partsIt;
    for(partsIt = parts.begin(); partsIt != parts.end(); ++partsIt)
    {
        std::string templateSpec = *partsIt;
        boost::trim(templateSpec);
        boost::shared_ptr<CL_DomElement>  ptpl = getSingleTemplate(templateSpec, baseDocument, baseFile);
        if(!ptpl)
        {
            throw CL_Exception("Template '" + spec + "' not found.");
        }
        if(!first)
        {
            first = false;
            applyTemplate(ptpl.get(), *prevTemplate);
        }
        prevTemplate = ptpl;
    }
    return *prevTemplate;
}

// Attention! Recursive function!
boost::shared_ptr<CL_DomElement>
TemplatesProcessor::getSingleTemplate(const std::string& _spec,
                                      DocumentPtr baseDocument,
                                      const std::string& baseFile)
{
    LOG("Get single template " + _spec + " from file " + baseFile);
    std::string spec(_spec);
    boost::trim(spec);
    // Anti circle include
    std::pair<std::string, std::string> currentPair = std::pair<std::string, std::string>(spec, baseFile);
    std::list< std::pair<std::string, std::string> >::iterator includeChainIt;
    for(includeChainIt = mIncludeChain.begin(); includeChainIt != mIncludeChain.end(); ++includeChainIt)
    {
        if(*includeChainIt == currentPair)
        {
            throw CL_Exception("Circle include. Template: " + spec + "; File: " + baseFile + ".");
        }
    }
    mIncludeChain.push_back(currentPair);

    boost::shared_ptr<CL_DomElement> result;

    // No file, no local
    if(spec.find("/") == spec.npos && spec.find("\\") == spec.npos)
    {
        std::string templateName = spec;
        // Check local
        boost::shared_ptr<CL_DomElement> localSearchResult = getLocalTemplate(templateName, baseDocument, baseFile);
        if(localSearchResult)
        {
            LOG("Found local");
            result = localSearchResult;
            if(result)
            {
                processTag(result.get(), baseDocument, baseFile);
            }
        }
        // Check common.tpl
        else
        {
            DocumentPtr commonDocument = getFileDocument(COMMON_TEMPLATES_FILE);
            boost::shared_ptr<CL_DomElement> comonSearchResult =
                getSingleTemplate(templateName, commonDocument, COMMON_TEMPLATES_FILE);
            if(comonSearchResult)
            {
                LOG("Found common");
                result = comonSearchResult;
                if(result)
                {
                    processTag(result.get(), commonDocument, COMMON_TEMPLATES_FILE);
                }
            }
            // Check in included files
            else
            {
                std::vector<std::string> includedFiles = getIncludedFiles(baseDocument, baseFile);
                std::vector<std::string>::iterator filesIt;
                for(filesIt = includedFiles.begin(); filesIt != includedFiles.end(); ++filesIt)
                {
                    std::cout << " check file " << *filesIt << std::endl;
                    DocumentPtr fileDocument = getFileDocument(*filesIt);
                    boost::shared_ptr<CL_DomElement> fileSearchResult =
                        getSingleTemplate(templateName, fileDocument, *filesIt);
                    if(fileSearchResult) LOG("Found in include file: " + *filesIt);
                    if(fileSearchResult)
                    {
                        if(!result)
                        {
                            result = fileSearchResult;
                        }
                        else
                        {
                            throw CL_Exception("Several templates matches to specifier '" + templateName + "'.");
                        }
                    }
                }
                if(result)
                {

                    processTag(result.get(), baseDocument, baseFile);
                }
            }
        }
    }
    // Local only
    else if(spec.find("local") == 0)
    {
        std::vector<std::string> parts;
        boost::split(parts, spec, boost::is_any_of("/\\"));
        if(parts.size() == 2)
        {
            std::string templateName = parts.back();
            result = getLocalTemplate(templateName, baseDocument, baseFile);
            if(result)
            {
                LOG("Found local");
                processTag(result.get(), baseDocument, baseFile);
            }
        }
        else
        {
            throw CL_Exception("Invalid use of keyword 'local': " + spec);
        }
    }
    // File
    else
    {
        boost::filesystem::path src = boost::filesystem::path(spec);
        std::string filePath = solveFileName(src.native_directory_string(), baseFile);
        std::string templateName = src.native_file_string();
        DocumentPtr fileDocument = getFileDocument(filePath);
        result = getSingleTemplate(templateName, fileDocument, filePath);
        if(result)
        {
            LOG("Found in file " + filePath);
            processTag(result.get(), fileDocument, filePath);
        }
    }

    mIncludeChain.pop_back();
    return result;
}

boost::shared_ptr<CL_DomElement>
TemplatesProcessor::getTemplatesTag(DocumentPtr baseDocument, const std::string& baseFile)
{
    boost::shared_ptr<CL_DomElement> result;

    std::vector<CL_DomNode> templatesElements;
    if(baseDocument->get_document_element().get_node_name() == "ThemistoLevel")
    {
        templatesElements = baseDocument->select_nodes("ThemistoLevel/Templates");
    }
    else
    {
        templatesElements = baseDocument->select_nodes("Templates"); // Funny, I know :)
    }
    if(templatesElements.size() == 1)
    {
        CL_DomElement templatesElement = templatesElements.front().to_element();
        result = boost::shared_ptr<CL_DomElement>(new CL_DomElement(templatesElement));
    }
    else if(templatesElements.size() > 1)
    {
        throw CL_Exception("Several tag 'Templates' in '" +baseFile +"'.");
    }
    return result;
}

std::vector<std::string>
TemplatesProcessor::getIncludedFiles(DocumentPtr baseDocument,
                                     const std::string& baseFile)
{
    std::vector<std::string> files;
    boost::shared_ptr<CL_DomElement> templatesTag = getTemplatesTag(baseDocument, baseFile);
    if(templatesTag)
    {
        CL_DomNodeList includeTags = templatesTag->get_elements_by_tag_name("Include");
        for(int i=0; i<includeTags.get_length(); ++i)
        {
            CL_DomElement includeTag = includeTags.item(i).to_element();
            std::string file = includeTag.get_node_value();
            std::string filePath = solveFileName(file, baseFile);
            files.push_back(filePath);
        }
    }
    return files;
}

boost::shared_ptr<CL_DomElement>
TemplatesProcessor::getLocalTemplate(const std::string& name,
                                     DocumentPtr baseDocument,
                                     const std::string& baseFile)
{
    std::cout << std::endl << "Get local template " << name << std::endl;
    boost::shared_ptr<CL_DomElement> result;

    boost::shared_ptr<CL_DomElement> templatesTag = getTemplatesTag(baseDocument, baseFile);
    if(templatesTag)
    {
        std::vector<CL_DomNode> templates =
            templatesTag->select_nodes("Template[@name='" + name + "']");
        if(templates.size() == 1)
        {
            result = boost::shared_ptr<CL_DomElement>(new CL_DomElement(templates.front().to_element()));
        }
        else if(templates.size() > 1)
        {
            throw CL_Exception("Several templates with equal names in local scope.");
        }
    }
    return result;
}

std::string TemplatesProcessor::solveFileName(const std::string& file, const std::string& basePath)
{
    std::string resultPath = "";

    // Find in scene dir
    boost::filesystem::path scenePath(basePath);
    std::string baseDir = scenePath.native_directory_string();
    boost::filesystem::path templatePath(baseDir);
    templatePath /= file;
    if( boost::filesystem::exists(templatePath) )
    {
        resultPath = std::string(templatePath.string());
        std::cout << "Found in scene dir" << std::endl;
    }
    // Find in media/templates dir
    else
    {
        boost::filesystem::path templatesDirRelativePath = boost::filesystem::path(TEMPLATES_DIRRECTORY);
        templatesDirRelativePath /= file;
        if( boost::filesystem::exists(templatesDirRelativePath) )
        {
            resultPath = std::string(templatesDirRelativePath.string());
            std::cout << "Found in templates dirrectory" << std::endl;
        }
        else
        {
            throw CL_Exception( "Template file '" + file + "' not found.");
        }
    }
    return resultPath;
}

DocumentPtr TemplatesProcessor::getFileDocument(const std::string& filePath)
{

    boost::shared_ptr<CL_DomElement> result;

    DocumentPtr document;
    std::map<std::string, DocumentPtr>::iterator documentIt= mFileToDocument.find(filePath);
    if( documentIt != mFileToDocument.end())
    {
        document = documentIt->second;
    }
    else
    {
        if(boost::filesystem::exists(filePath))
        {
            CL_File fileHandle = CL_File(filePath);
            document = DocumentPtr(new CL_DomDocument(fileHandle));
            mFileToDocument.insert(std::pair<std::string, DocumentPtr>(filePath, document));
        }
        else
        {
            throw CL_Exception("File " + filePath + " not exists.");
        }
    }
    return document;
}


void TemplatesProcessor::applyTemplate(CL_DomElement* tag, const CL_DomElement& templateTag)
{
    std::cout <<std::endl << "Applying template.." << std::endl;
}










