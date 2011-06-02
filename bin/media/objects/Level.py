import Core.LogManager
import Core.ApplicationManager
import Core.WorldManager
import Core.ResourceManager
import World.Objects.TypesManager
from World.Objects.Object import *
from Physic.Physic import *

import Core.Utils
import random, math

class Level(Object):
    def __init__(self):
        super(Level, self).__init__()

        # Precalculated value
        self.mGC = Core.ApplicationManager.getInstance().GetGraphic()
        self.mBody = Body()

    def Init(self):
        pass

    def GetBody(self):
        return self.mBody

    def GetTextureInfo(self):
        return [self.mSection, self.mTexture]

    def SetVisual(self, texture, section, width, height):
        self.mTexture = texture
        self.mSection = section

        self.mImage = Core.ResourceManager.getInstance().GetSprite(section, texture)
        if self.mImage.IsNull():
            print 'Failed to retreive image: ' + str(texture)

        koefX = koefY = 1.0
        if width > 0:
            koefX = width / self.mImage.GetWidth()
        if height > 0:
            koefY = height / self.mImage.GetHeight()

        self.mImage.SetScale(koefX, koefY)
        self.mImage.SetLinearFilter(True)

        # TODO: Generate collision outline here...

    def Update(self, elapsed):
        pass

    def UpdateVisual(self, newX, newY):
        self.mImage.Draw(self.mGC, newX, newY)

        # TODO: Set rotation for collision

    def GetPosition(self):
        pos = self.mBody.GetBody().Position
        return CL_Pointf(Core.Utils.Meters2Pixels(pos.x), Core.Utils.Meters2Pixels(pos.y))

    def SetPosition(self, point):
        print 'Can`t set position for body after it`s creating.'

    def GetRectangle(self):
        bodyFixtures = self.mBody.GetBody().Fixtures
        rect = bodyFixtures[0].GetAABB()

        for i in xrange(len(bodyFixtures)):
            rect.Combine(rect, bodyFixtures[i].GetAABB())

        return CL_Rectf(Core.Utils.Meters2Pixels(rect.LowerBound.x), \
                        Core.Utils.Meters2Pixels(rect.LowerBound.y), \
                        Core.Utils.Meters2Pixels(rect.UpperBound.x), \
                        Core.Utils.Meters2Pixels(rect.UpperBound.y))

    def CheckCollision(self):
        return False

    @staticmethod
    def ParseLevel(node):
        obj = Level()
        # Parse visuals -----------------------------------
        VisualTags = node.GetElementsByTagName("Visual")
        if VisualTags.GetLength() == 1:
            section = name = None
            width = height = -1.0

            childList = VisualTags.Item(0).GetChildNodes()
            for i in xrange(childList.GetLength()):
                tag2 = childList.Item(i).ToElement()
                if tag2.GetNodeName() == "Size":
                    width = float(tag2.GetAttribute("width"))
                    height = float(tag2.GetAttribute("height"))
                elif tag2.GetNodeName() == "Texture":
                    section = str(tag2.GetAttribute("section"))
                    name = str(tag2.GetAttribute("name"))
            obj.SetVisual(name, section, width, height)
        else:
            Core.LOG('Error: only one `Visual` tag may be in Physic object!')

        # Parse physic -------------------------------------
        BodyTags = node.GetElementsByTagName("Body")
        if BodyTags.GetLength() == 1:
            bodyTag = BodyTags.Item(0).ToElement()
            physicHandle = ParsePhysicBody(bodyTag)
            obj.mBody = physicHandle

        return obj

World.Objects.TypesManager.RegisterParser("LevelObject", Level.ParseLevel)
