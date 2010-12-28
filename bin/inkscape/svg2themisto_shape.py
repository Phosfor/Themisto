#!/usr/bin/env python2

"""
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
"""

import os, sys, inkex, locale, gettext, simplepath, re
_ = gettext.gettext

locale.setlocale(locale.LC_ALL, '')

class Svg2Themisto_Shape(inkex.Effect):
    def __init__(self):
        inkex.Effect.__init__(self)
        self.fixturesList = []

        self.OptionParser.add_option("--directory", action="store",
                                        type="string", dest="directory",
                                        default=None, help="")

        self.OptionParser.add_option("--filename", action="store",
                                        type="string", dest="filename",
                                        default=None, help="")

    def check_dir_exists(self, dir):
        if not os.path.isdir(dir):
            os.makedirs(dir)

    def dumpXML(self):
        self.check_dir_exists(self.options.directory)
        fileLocation = os.path.join(self.options.directory, self.options.filename) + '.shape'
        inkex.errormsg(_(fileLocation))
        fileHandle = open(fileLocation, 'w')

        fixtureCounter = 0
        vertexCounter = 0
        # A bit ugly...
        fileHandle.write('<Template name="'+ self.options.filename +'">\n <Object type="PhysicObject">\n  <Body id="tbody1">\n')
        for fixture in self.fixturesList:
            fileHandle.write('   <Part id="part'+ str(fixtureCounter) +'">\n    <b2Fixture>\n')
            fileHandle.write('     <Shape>\n      <Vertices>\n')
            for point in fixture:
                fileHandle.write('       <Vertex id="vertex'+ str(vertexCounter) +'">\n')
                fileHandle.write('        <x>')
                fileHandle.write(str(point[0]))
                fileHandle.write('</x>\n')
                fileHandle.write('        <y>')
                fileHandle.write(str(point[1]))
                fileHandle.write('</y>\n')
                fileHandle.write('       </Vertex>\n')
                vertexCounter += 1
            fileHandle.write('      </Vertices>\n     </Shape>\n   </b2Fixture>\n')
            fixtureCounter += 1
            vertexCounter = 0

        fileHandle.write('  </Body>\n </Object>\n</Template>\n')

    def effect(self):
        root = self.document.getroot()

        # Get list of fixtures
        fixturesList = self.document.xpath("//svg:path", namespaces=inkex.NSS)

        # Iterate through fixtures
        for fixture in fixturesList:
            fixtureData = []
            fixtureId = fixture.attrib['id']

            # Convert path data into stuctured array
            pathData = simplepath.parsePath(fixture.attrib['d'])

            # Check whether path is closed (if 'Z' element is presented)
            # Z is always in latest element
            if pathData[-1].count('Z') is 0:
                raise Exception, fixtureId + ': Should be closed path!'

            # Check whether number of vertices is right
            if not (2 <= (len(pathData)-1) <= 8):
                raise Exception, fixtureId + ' convex should has <=2 vertex <= 8!'

            # We need only list with 2 points in it
            for point in pathData:
                if not point[1]:
                    continue
                point[1][0] = round(point[1][0], 1)
                point[1][1] = round(point[1][1], 1)
                fixtureData.append(point[1])

            self.fixturesList.append(fixtureData)

        self.dumpXML()

if __name__ == "__main__":
    handle = Svg2Themisto_Shape()
    handle.affect()
