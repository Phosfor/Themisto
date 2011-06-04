import Core.LogManager
import Core.ApplicationManager
import Core.WorldManager
import Core.ResourceManager
import Core.LevelManager
import World.Objects.TypesManager
from World.Objects.Object import *

import Core.Utils
import random, math

class AnimatedDecoration(Object):
    def __init__(self):
        super(AnimatedDecoration, self).__init__()

        # Precalculated value
        self.mGC = Core.ApplicationManager.getInstance().GetGraphic()
        self.mPos = None
        self.mSprite = None
        self.mBoundingBox = None

    def Init(self):
        pass

    def SetVisual(self, section, texture, width, height):
        # Load animation
        size = Core.ResourceManager.SectionNumChildren(section)
        Names = []
        for i in xrange(size):
            Names.append(str(i))

        self.mSprite = Core.ResourceManager.getInstance().GetSprite(section, Names)

        koefX = koefY = 1
        if width > 0:
            koefX = width / self.mSprite.GetWidth()
        if height > 0:
            koefY = height / self.mSprite.GetHeight()

        self.mSprite.SetScale(koefX, koefY)
        self.mSprite.SetLinearFilter(True)

        self.mBoundingBox = CL_Rectf(self.mPos.x, self.mPos.y, \
           self.mPos.x + self.mSprite.GetWidth() * koefX,      \
           self.mPos.y + self.mSprite.GetHeight() * koefY)

    def Update(self, elapsed):
        pass

    def UpdateVisual(self, newX, newY):
        self.mSprite.Update(-1)
        self.mSprite.Draw(self.mGC, newX, newY)

    def GetPosition(self):
        return self.mPos

    def SetPosition(self, point):
        self.mPos = point

    def GetRectangle(self):
        return self.mBoundingBox

    def CheckCollision(self):
        return False

    @staticmethod
    def ParseAnimatedDecoration(node):
        obj = AnimatedDecoration()

        # Parse visual settings --------------------------------------
        VisualTags = node.GetElementsByTagName("Visual")
        width = height = -1
        section = ''
        x = y = 0.0

        childList = VisualTags.Item(0).GetChildNodes()
        for i in xrange(childList.GetLength()):
            tag2 = childList.Item(i).ToElement()
            if tag2.GetNodeName() == "Size":
                width = float(tag2.GetAttribute("width"))
                height = float(tag2.GetAttribute("height"))
            if tag2.GetNodeName() == "Texture":
                section = str(tag2.GetAttribute("section"))
            if tag2.GetNodeName() == "Position":
                x = float(tag2.GetAttribute("x"))
                y = float(tag2.GetAttribute("y"))

        obj.SetPosition(CL_Pointf(x, y))
        obj.SetVisual(section, '', width, height)
        return obj

World.Objects.TypesManager.RegisterParser("AnimatedDecorationObject", AnimatedDecoration.ParseAnimatedDecoration)
