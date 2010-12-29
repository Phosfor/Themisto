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
        fileHandle = open(fileLocation, 'w')

        fixtureCounter = 0
        vertexCounter = 0
        # A bit ugly...
        fileHandle.write('<Template name="'+ self.options.filename +'">\n <Object type="PhysicObject">\n  <Body id="tbody1">\n')
        for fixture in self.fixturesList:
            fileHandle.write('   <Part id="part'+ str(fixtureCounter) +'">\n    <b2Fixture>\n')
            fileHandle.write('     <Shape>\n      <Type>'+ fixture[0] +'</Type>\n')

            if fixture[0] == "e_polygon":
                fileHandle.write('      <Vertices>\n')
                for i in range(1, len(fixture)):
                    fileHandle.write('       <Vertex id="vertex'+ str(vertexCounter) +'">\n')
                    fileHandle.write('        <x>')
                    fileHandle.write(str(fixture[i][0]))
                    fileHandle.write('</x>\n')
                    fileHandle.write('        <y>')
                    fileHandle.write(str(fixture[i][1]))
                    fileHandle.write('</y>\n')
                    fileHandle.write('       </Vertex>\n')
                    vertexCounter += 1
                fileHandle.write('      </Vertices>\n')
                fixtureCounter += 1
                vertexCounter = 0

            elif fixture[0] == "e_circle":
                fileHandle.write('      <Radius>'+ str(fixture[1]) +'</Radius>\n')

            fileHandle.write('     </Shape>\n    </b2Fixture>\n   </Part>\n')

        fileHandle.write('  </Body>\n </Object>\n</Template>\n')

    def effect(self):
        root = self.document.getroot()

        # Get list of fixtures
        fixturesList = self.document.xpath("//svg:path", namespaces=inkex.NSS)

        # Iterate through fixtures
        for fixture in fixturesList:
            fixtureData = []

            # Available:
            #    - e_polygon
            #    - e_circle
            fixtureType = "e_polygon"
            fixtureId = fixture.attrib['id']

            # If the object is circle
            for att in fixture.attrib:
                res = re.findall(r'{.*}type', att)
                if res:
                    if fixture.attrib[res[0]] == "arc":
                        fixtureType = "e_circle"

            fixtureData.append(fixtureType)

            if fixtureType == "e_circle":
                centerX = centerY = radius = 0.0
                # Circle center coordinates:
                for att in fixture.attrib:
                    attData = re.findall(r'{.*}cx', att)
                    if attData:
                        centerX = round(float(fixture.attrib[attData[0]]), 1)

                    attData = re.findall(r'{.*}cy', att)
                    if attData:
                        centerY = round(float(fixture.attrib[attData[0]]), 1)

                    attData = re.findall(r'{.*}rx', att)
                    if attData:
                        radius = round(float(fixture.attrib[attData[0]]), 1)

                fixtureData.append(radius)
                fixtureData.append([centerX, centerY])

                self.fixturesList.append(fixtureData)

            # if fixture type is e_polygon
            else:
                # Convert path data into stuctured array
                pathData = simplepath.parsePath(fixture.attrib['d'])

                # Check whether path is closed (if 'Z' element is presented)
                # Z is always in latest element
                if pathData[-1].count('Z') is 0:
                    raise Exception, fixtureId + ': Should be closed path!'

                # Check whether number of vertices is right
                # -2 because latest is 'Z' and latest-1 is the same point, as first one
                if not (2 <= (len(pathData)-2) <= 8):
                    raise Exception, fixtureId + ' convex should has 2 <= vertex <= 8!'

                # We need only list with 2 points in it
                counter = 0
                for point in pathData:
                    if not point[1] or counter is len(pathData)-2:
                        continue
                    point[1][0] = round(point[1][0], 1)
                    point[1][1] = round(point[1][1], 1)
                    fixtureData.append(point[1])
                    counter += 1

                self.fixturesList.append(fixtureData)

        inkex.errormsg(_(self.fixturesList))
        self.dumpXML()

if __name__ == "__main__":
    handle = Svg2Themisto_Shape()
    handle.affect()
