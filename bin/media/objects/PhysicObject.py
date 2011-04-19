import Core.LogManager
import Core.ApplicationManager
import Core.WorldManager
import Core.ResourceManager
import World.Objects.TypesManager
from World.Objects.Object import *
from Physic.Physic import *

import Core.Utils
import random, math

class Physic(Object):
    def __init__(self):
        super(Physic, self).__init__()

        # Precalculated value
        self.mGC = Core.ApplicationManager.getInstance().GetGraphic()
        self.mBody = Body()

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
        rotation = CL_Angle.FromRadians(self.mBody.GetBody().Angle)
        self.mImage.SetAngle(rotation)
        self.mImage.Draw(self.mGC, newX, newY)

        # TODO: Set rotation for collision

    def GetPosition(self):
        pos = self.mBody.GetBody().Position
        return CL_Pointf(Core.Utils.Meters2Pixels(pos.x), Core.Utils.Meters2Pixels(pos.y))

    def SetPosition(self, point):
        print 'Can`t set position for body after it`s creating.'

    def GetRectangle(self):
        rect = b2AABB()
        bodyFixtures = self.mBody.GetBody().Fixtures

        for i in xrange(len(bodyFixtures)):
            rect.Combine(rect, bodyFixtures[i].GetAABB())

        return CL_Rectf(Core.Utils.Meters2Pixels(rect.LowerBound.x), \
                        Core.Utils.Meters2Pixels(rect.LowerBound.y), \
                        Core.Utils.Meters2Pixels(rect.UpperBound.x), \
                        Core.Utils.Meters2Pixels(rect.UpperBound.y))

    def CheckCollision(self):
        return False

    @staticmethod
    def ParsePhysic(node):
        obj = Physic()
        # Parse visuals -----------------------------------
        VisualTags = node.GetElementsByTagName(CL_DomString("Visual"))
        if VisualTags.GetLength() == 1:
            section = name = None
            width = height = -1

            childList = VisualTags.Item(0).GetChildNodes()
            for i in xrange(childList.GetLength()):
                tag2 = childList.Item(i).ToElement()
                if tag2.GetNodeName().CStr() == "Size":
                    width = int(tag2.GetAttribute(CL_DomString("width")).CStr())
                    height = int(tag2.GetAttribute(CL_DomString("height")).CStr())
                elif tag2.GetNodeName().CStr() == "Texture":
                    section = str(tag2.GetAttribute(CL_DomString("section")).CStr())
                    name = str(tag2.GetAttribute(CL_DomString("name")).CStr())
            obj.SetVisual(name, section, width, height)
        else:
            Core.LOG('Error: only one `Visual` tag may be in Physic object!')

        # Parse physic -------------------------------------
        BodyTags = node.GetElementsByTagName(CL_DomString("Body"))
        if BodyTags.GetLength() == 1:
            bodyTag = BodyTags.Item(0).ToElement()
            physicHandle = ParsePhysicBody(bodyTag)
            obj.mBody = physicHandle

        return obj

World.Objects.TypesManager.RegisterParser("PhysicObject", Physic.ParsePhysic)
