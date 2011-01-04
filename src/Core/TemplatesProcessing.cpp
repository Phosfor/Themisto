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
    LOG("Get single template " + _spec + "; Base file: " + baseFile);
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
            if(boost::filesystem::exists(COMMON_TEMPLATES_FILE))
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
            }
            // Check in included files
            if(!result)
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
            std::string file = includeTags.item(i).get_node_value().c_str();
            LOG(std::string("Include tag name: ") +  includeTags.item(i).get_node_name().c_str());
            LOG("Found included file " + file);
            std::string filePath = solveFileName(file, baseFile);
            files.push_back(filePath);
        }
    }
    LOG("Found " + IntToStr(files.size()) + " included files.");
    return files;
}

boost::shared_ptr<CL_DomElement>
TemplatesProcessor::getLocalTemplate(const std::string& name,
                                     DocumentPtr baseDocument,
                                     const std::string& baseFile)
{
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
    LOG("Solving file: " + file + "; base file: " + basePath);

    // Find in base dir
    boost::filesystem::path scenePath(basePath);
    std::string baseDir = scenePath.native_directory_string();
    LOG("Base dirrectory: " + baseDir);
    boost::filesystem::path templatePath(baseDir);
    templatePath /= file;
    if( boost::filesystem::exists(templatePath) )
    {
        resultPath = std::string(templatePath.string());
        std::cout << "Found in base dir: " << resultPath << std::endl;
    }
    // Find in media/templates dir
    else
    {
        boost::filesystem::path templatesDirRelativePath = boost::filesystem::path(TEMPLATES_DIRRECTORY);
        templatesDirRelativePath /= file;
        if( boost::filesystem::exists(templatesDirRelativePath) )
        {
            resultPath = std::string(templatesDirRelativePath.string());
            std::cout << "Found in templates dirrectory: " << resultPath << std::endl;
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

bool TemplatesProcessor::tagsEquivalent(const CL_DomElement&  tag1, const CL_DomElement& tag2)
{
    std::string id1 = "";
    if(tag1.has_attribute("id") )
    {
        id1 = tag1.get_attribute("id");
        boost::trim(id1);
    }
    std::string id2 = "";
    if(tag2.has_attribute("id") )
    {
        id2 = tag2.get_attribute("id");
        boost::trim(id2);
    }
    return (tag1.get_node_name() == tag2.get_node_name()) && (id1 == id2);
}

void TemplatesProcessor::applyTemplate(CL_DomElement* _tag, const CL_DomElement& templateTag)
{
    CL_DomElement tag = *_tag;
    std::cout << "applyTemplate iteration...\n";
    std::cout << "current node_name: " << tag.get_node_name().c_str() << "\n";
    std::cout << "template node_name: " << templateTag.get_node_name().c_str() << "\n";

   if(tagsEquivalent(tag, templateTag))
   {
        std::cout << "Tags are equivalent!\n";

        std::string action = "merge";
        if (tag.has_attribute("action"))
        {
            action = tag.get_attribute("action");
        }
        std::cout << "tag action: " << action << "\n";

        // If ignore - remove element from xml-tree
        if (action == "ignore")
        {
            tag.get_parent_node().remove_child(tag);
        }
        else if (action == "override")
        {
            // Nothing to do
            return;
        }
        else if (action == "merge")
        {
            copyAttributes(tag, templateTag);
            CL_DomNodeList templateChildren = templateTag.get_child_nodes();
            // Check on qeuivalent dublicates in template
            for (int i=0; i < templateChildren.get_length(); ++i)
            {
                for (int j=i+1; j < templateChildren.get_length(); ++j)
                {
                    if(tagsEquivalent(templateChildren.item(i).to_element(), templateChildren.item(j).to_element()))
                    {
                        throw CL_Exception("Several equivalent tags in template.");
                    }
                }
            }
            // Apply template childs to equivalent tags in current tag
            // or simple add, if no equivalent tags.
            CL_DomNodeList tagChildren = tag.get_child_nodes();
            for (int tpli=0; tpli < templateChildren.get_length(); ++tpli)
            {

                // Find equivalent tags
                bool applied = false;
                CL_DomElement templateChild = templateChildren.item(tpli).to_element();
                std::cout << "\t - Iteration through template children. Tag name: " << templateChild.get_node_name().c_str() << "\n";
                for (int ti=0; ti < tagChildren.get_length(); ++ti)
                {
                    CL_DomElement tagChild = tagChildren.item(ti).to_element();
                    if(tagsEquivalent(tagChild, templateChild))
                    {
                        applyTemplate(&tagChild, templateChild);
                        applied = true;
                    }
                }
                if(!applied)
                {
                    std::cout << "No equivalent tags, add children\n";
                    CL_DomNode toAdd = tag.get_owner_document().import_node(templateChild, true);
                    tag.append_child(toAdd);
                }
            }
        }
    }
    // Not equivalent
    else
    {
        std::cout << "Tags are NOT equivalent!\n";
        CL_DomNode parent = tag.get_parent_node();
        CL_DomNode toAdd = tag.get_owner_document().import_node(templateTag, true);
        parent.append_child(toAdd);
    }
}

void TemplatesProcessor::copyAttributes(CL_DomElement tag, CL_DomElement templateTag)
{
    CL_DomNamedNodeMap templateAttributes = templateTag.get_attributes();
    for (int i=0; i < templateAttributes.get_length(); ++i)
    {
        CL_DomAttr attribute = templateAttributes.item(i).to_attr();
        if(!tag.has_attribute(attribute.get_name()))
        {
            tag.set_attribute(attribute.get_name(), attribute.get_value());
        }
    }
}











