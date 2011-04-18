import Core.LogManager
import Core.ApplicationManager
import Core.WorldManager
import Core.ResourceManager
import World.Objects.TypesManager
from World.Objects.Object import *

import Core.Utils
import random, math

class Physic(Object):
    def __init__(self):
        super(Physic, self).__init__()

        # Precalculated value
        self.mGC = Core.ApplicationManager.getInstance().GetGraphic()

    def Init(self):
        pass

    def GetBody(self):
        return self.mBody

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

        # TODO: Generate collision outline here...

    def Update(self, elapsed):
        pass

    def UpdateVisual(self, newX, newY):
        CL_Angle rotation = CL_Angle.FromRadians(self.mBody.GetBody().GetAngle())
        self.mImage.SetAngle(rotation)
        self.mImage.draw(self.mGC, newX, newY)

        # TODO: Set rotation for collision

    def GetPosition(self):
        pos = self.mBody.GetBody().GetPosition()
        return CL_Pointf(Core.Utils.Meters2Pixels(pos.x), Core.Utils.Meters2Pixels(pos.y))

    def SetPosition(self, point):
        print 'Can`t set position for body after it`s creating.'

    def GetRectangle(self):
        pass
        #rect = None
        #fixture = self.mBody.GetBody().GetFixtureList()
        #if (fixture != None):
        #    rect = fixture.GetAABB()

        # TODO: Write this

    def CheckCollision(self):
        return False

    @staticmethod
    def ParsePhysic(node):
        moonHandle = Moon()

        VisualTags = node.GetElementsByTagName(CL_DomString("Visual"))
        name = section = None
        width = height = -1

        # Parse visual settings
        childList = VisualTags.Item(0).GetChildNodes()
        for i in xrange(childList.GetLength()):
            tag2 = childList.Item(i).ToElement()
            if tag2.GetNodeName().CStr() == "Size":
                width = int(tag2.GetAttribute(CL_DomString("width")).CStr())
                height = int(tag2.GetAttribute(CL_DomString("height")).CStr())
            if tag2.GetNodeName().CStr() == "Texture":
                section = str(tag2.GetAttribute(CL_DomString("section")).CStr())
                name = str(tag2.GetAttribute(CL_DomString("name")).CStr())

        moonHandle.SetVisual(name, section, width, height)

        # Parse common settings
        radius = Core.Utils.Meters2Pixels(7)
        angle = Core.Utils.Deg2Rad(-90)

        radiusElement = node.GetElementsByTagName(CL_DomString("Radius")).Item(0).ToElement()
        if radiusElement.HasAttribute(CL_DomString("value")):
            radius = Core.Utils.Meters2Pixels(float(radiusElement.GetAttribute(CL_DomString("value")).CStr()))

        angleElement = node.GetElementsByTagName(CL_DomString("StartAngle")).Item(0).ToElement()
        if angleElement.HasAttribute(CL_DomString("value")):
            angle = Core.Utils.Deg2Rad(float(angleElement.GetAttribute(CL_DomString("value")).CStr()))

        moonHandle.SetOrbitRadius(radius)
        moonHandle.SetMoonAngle(angle)

        return moonHandle

World.Objects.TypesManager.RegisterParser("PhysicObject", Physic.ParsePhysic)
