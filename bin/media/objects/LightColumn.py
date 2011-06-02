import Core.LogManager
import Core.ApplicationManager
import Core.WorldManager
import Core.ResourceManager
import World.Objects.TypesManager
from World.Objects.Object import *

import Core.Utils
import random, math

class LightBug:
    def __init__(self):
        self.boundingValue = 10
        self.koef1 = 1
        self.koef2 = 3

        self.x = self.y = self.offsetX = self.offsetY = 0
        self.image = None

        self.theta = random.uniform(0.5, 1.0)

class LightColumn(Object):
    def __init__(self):
        super(LightColumn, self).__init__()

        # Precalculated value
        self.mGC = Core.ApplicationManager.getInstance().GetGraphic()
        self.mAbsTime = 0.0

        self.mLightings = []
        self.mLightingsPos = []
        self.mBugs = []

        self.mBoundingBox = None
        self.mImage = None
        self.mPos = None

    def Init(self):
        pass

    def SetVisual(self, texture, section, width, height):
        self.mImage = Core.ResourceManager.getInstance().GetSprite(section, texture)

        koefX = koefY = 1
        if width > 0:
            koefX = width / self.mImage.GetWidth()
        if height > 0:
            koefY = height / self.mImage.GetHeight()

        self.mImage.SetScale(koefX, koefY)
        self.mImage.SetLinearFilter(True)
        self.mImage.SetRotationHotspot(origin_top_left, 0, 0)

        self.mBoundingBox = CL_Rectf(self.mPos.x, self.mPos.y, \
                self.mPos.x + self.mImage.GetWidth() * koefX,  \
                self.mPos.y + self.mImage.GetHeight() * koefY)

    def AddLightning(self, texture, section, width, height):
        light = Core.ResourceManager.getInstance().GetSprite(section, texture)

        koefX = koefY = 1
        if width > 0:
            koefX = width / light.GetWidth()
        if height > 0:
            koefY = height / light.GetHeight()

        light.SetScale(koefX, koefY)
        light.SetLinearFilter(True)
        self.mLightings.append(light)

        pos = self.mLightingsPos[-1]
        self.mBoundingBox = BoundingRect(               \
            self.mBoundingBox,                          \
            CL_Rectf(                                   \
                self.mPos.x + pos.x,                    \
                self.mPos.y + pos.y,                    \
                self.mPos.x + pos.x + light.GetWidth(), \
                self.mPos.y + pos.y + light.GetHeight() \
            ))

    def AddBug(self, bug):
        self.mBugs.append(bug)

    def SetLightingPos(self, pos):
        self.mLightingsPos.append(pos)

    def Update(self, elapsed):
        self.mAbsTime += elapsed / 3.5

        for bug in self.mBugs:
            bug.x = bug.boundingValue * math.sin(bug.koef1 * self.mAbsTime + bug.theta)
            bug.y = bug.boundingValue * math.sin(bug.koef2 * self.mAbsTime)

    def UpdateVisual(self, newX, newY):
        # Draw main picture
        self.mImage.Draw(self.mGC, newX, newY)

        # Draw lightnings
        for i in xrange(len(self.mLightings)):
            self.mLightings[i].Draw(self.mGC,       \
                    newX + self.mLightingsPos[i].x, \
                    newY + self.mLightingsPos[i].y)

        # Draw bugs
        for bug in self.mBugs:
            bug.image.Draw(self.mGC,            \
                    newX + bug.x + bug.offsetX, \
                    newY + bug.y + bug.offsetY)

    def GetPosition(self):
        return self.mPos

    def SetPosition(self, newPos):
        self.mPos = newPos

    def GetRectangle(self):
        return self.mBoundingBox

    def CheckCollision(self):
        return False

    @staticmethod
    def ParseLightColumn(node):
        light = LightColumn()

        # Parse visual settings --------------------------------------
        VisualTags = node.GetElementsByTagName("Visual")
        name = section = None
        width = height = -1
        x = y = 0.0

        childList = VisualTags.Item(0).GetChildNodes()
        for i in xrange(childList.GetLength()):
            tag2 = childList.Item(i).ToElement()
            if tag2.GetNodeName() == "Size":
                width = float(tag2.GetAttribute("width"))
                height = float(tag2.GetAttribute("height"))
            if tag2.GetNodeName() == "Texture":
                section = str(tag2.GetAttribute("section"))
                name = str(tag2.GetAttribute("name"))
            if tag2.GetNodeName() == "Position":
                x = float(tag2.GetAttribute("x"))
                y = float(tag2.GetAttribute("y"))

        light.SetPosition(CL_Pointf(x, y))
        light.SetVisual(name, section, width, height)

        # Parse lighting ------------------------------------------
        lightTags = node.GetElementsByTagName("Lighting")

        # Iterate through each lighting
        for i in xrange(lightTags.GetLength()):
            handle = lightTags.Item(i).ToElement()
            handleChildren = handle.GetChildNodes()

            width = height = -1
            name = section = None
            x = y = 0.0

            for j in xrange(handleChildren.GetLength()):
                child = handleChildren.Item(j).ToElement()

                if child.GetNodeName() == "Size":
                    width = float(child.GetAttribute("width"))
                    height = float(child.GetAttribute("height"))
                if child.GetNodeName() == "Texture":
                    section = str(child.GetAttribute("section"))
                    name = str(child.GetAttribute("name"))
                if child.GetNodeName() == "Position":
                    x = float(child.GetAttribute("x"))
                    y = float(child.GetAttribute("y"))

            light.SetLightingPos(CL_Pointf(x, y))
            light.AddLightning(name, section, width, height)

        # Parse bugs ------------------------------------------------
        bugTags = node.GetElementsByTagName("Bug")

        # Iterate through each lighting
        for i in xrange(bugTags.GetLength()):
            handle = bugTags.Item(i).ToElement()
            handleChildren = handle.GetChildNodes()

            bounding = 0.0
            a = b = 1.0
            width = height = -1
            scaleX = scaleY = 1.0
            x = y = 0.0
            name = section = None

            for j in xrange(handleChildren.GetLength()):
                child = handleChildren.Item(j).ToElement()

                if child.GetNodeName() == "Bounding":
                    bounding = float(child.GetAttribute("value"))
                if child.GetNodeName() == "Texture":
                    section = str(child.GetAttribute("section"))
                    name = str(child.GetAttribute("name"))
                if child.GetNodeName() == "Position":
                    x = float(child.GetAttribute("x"))
                    y = float(child.GetAttribute("y"))
                if child.GetNodeName() == "Koef":
                    a = float(child.GetAttribute("a"))
                    b = float(child.GetAttribute("b"))
                if child.GetNodeName() == "TextureScale":
                    scaleX = float(child.GetAttribute("x"))
                    scaleY = float(child.GetAttribute("y"))

            bug = LightBug()
            bugImage = Core.ResourceManager.getInstance().GetSprite(section, name)
            bugImage.SetAlignment(origin_center)
            bugImage.SetScale(scaleX, scaleY)
            bug.boundingValue = bounding
            bug.koef1 = a
            bug.koef2 = b
            bug.offsetX = x
            bug.offsetY = y
            bug.image = bugImage

            light.AddBug(bug)

        return light

World.Objects.TypesManager.RegisterParser("LightColumnObject", LightColumn.ParseLightColumn)
