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

#ifndef _TEMPLATES_PROCESSING_
#define _TEMPLATES_PROCESSING_

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>
#include <vector>
#include <iostream>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include "Core/Utils.hpp"
#include "Core/LogManager.hpp"

typedef boost::shared_ptr<CL_DomDocument> DocumentPtr;

class TemplatesProcessor
{
    private:
        std::map<std::string, DocumentPtr> mFileToDocument;
        std::list< std::pair<std::string, std::string> > mIncludeChain;

        void processTag(CL_DomElement* tag, DocumentPtr baseDocument, const std::string& baseFile);

        CL_DomElement getTemplate(const std::string& spec,
                                  DocumentPtr baseDocument,
                                  const std::string& baseFile);

        boost::shared_ptr<CL_DomElement>
        getSingleTemplate(const std::string& _spec,
                           DocumentPtr baseDocument,
                           const std::string& baseFile);

        boost::shared_ptr<CL_DomElement>
        getTemplatesTag(DocumentPtr baseDocument, const std::string& baseFile);

        std::vector<std::string>
        getIncludedFiles(DocumentPtr baseDocument,
                        const std::string& baseFile);

        boost::shared_ptr<CL_DomElement>
        getLocalTemplate(const std::string& name,
                         DocumentPtr baseDocument,
                         const std::string& baseFile);

        std::string solveFileName(const std::string& file, const std::string& basePath);

        DocumentPtr getFileDocument(const std::string& filePath);

        void applyTemplate(CL_DomElement* tag, const CL_DomElement& templateTag);
        void applyParent(CL_DomElement* tag, const CL_DomElement& parentTag);
        void applyTemplateRecursive(CL_DomElement* tag, const CL_DomElement& templateTag);
        void copyAttributes(CL_DomElement tag, CL_DomElement templateTag);
        bool tagsEquivalent(const CL_DomElement&  tag1, const CL_DomElement& tag2);

    public:
        TemplatesProcessor();
        DocumentPtr processTemplates(const std::string& sceneFile);
};


#endif
