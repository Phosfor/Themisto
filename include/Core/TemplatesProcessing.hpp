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
//#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string.hpp>

//typedef boost::shared_ptr<CL_DomDocument> DocumentPtr;

class TemplatesProcessor
{
    private:
        //std::map<std::string, DocumentPtr> mFileToDocument;

        // These two fileds are using in most private functions;
        // For syntax clear, I keep them in fields, not passing as params to everywhere
        CL_DomDocument* mSceneDocument;
        std::string mErrorMessage;

        void processTag(CL_DomElement* tag);
        void applyTemplate(CL_DomElement* tag, const CL_DomElement& templateTag);
        CL_DomElement* getTemplate(const std::string& spec);
        CL_DomElement* getSingleTemplate(const std::string& _spec); // Without multiply specifiers
        CL_DomElement* getLocalTemplate(const std::string& name);
        CL_DomElement* getTemplateFromFile(const std::string& file,const std::string& name);



    public:
        TemplatesProcessor();
        std::string processTemplates(CL_DomDocument* scene); // Returns error message ( empty string if ok)
};


#endif
