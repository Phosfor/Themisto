import Core.LogManager
import Core.ApplicationManager
import Core.WorldManager
import Core.ResourceManager
import World.Objects.TypesManager
from World.Objects.Object import *

import Core.Utils
import random, math

class Moon(Object):
    def __init__(self):
        super(Moon, self).__init__()

        # Precalculated value
        self.mRadius = Core.Utils.Meters2Pixels(7)
        self.mAngle = Core.Utils.Deg2Rad(-90)
        self.mRenderMoon = True

        self.mCenterX = 0
        self.mCenterY = Core.Utils.ScreenResolutionY

        self.mPosX = self.mPosY = 0

        self.mGC = Core.ApplicationManager.getInstance().GetGraphic()
        self.mWorldManagerHandle = Core.WorldManager.getInstance()

        self.mMoon = None
        self.mMoonColor = None

        self.t = 0
        self.mG = self.mB = 1

    def init(self):
        pass

    def setOrbitRadius(self, radius):
        self.mRadius = radius

    def setMoonAngle(self, angle):
        self.mAngle = angle

    def setVisual(self, texture, section, width, height):
        self.mMoon = Core.ResourceManager.getInstance().GetImage(section, texture)
        self.mMoonColor = self.mMoon.GetColor()
        self.mMoon.SetAlignment(origin_center_right)

        koefX = koefY = 1
        if width > 0:
            koefX = width / self.mMoon.GetWidth()
        if height > 0:
            koefY = height / self.mMoon.GetHeight()

        self.mMoon.SetScale(koefX, koefY)

    def update(self, elapsed):
       globalTime = self.mWorldManagerHandle.GetAbsTime()

       if not self.mRenderMoon and (globalTime == 0 or globalTime <= 0.3):
           self.mRenderMoon = True
           self.mAngle = Core.Utils.Deg2Rad(-90)
           self.mMoon.SetColor(CL_Colorf(1, 1, 1, 1))

       if not self.mRenderMoon:
           return

       self.mPosX = (self.mRadius * math.cos(self.mAngle)) * 1.3 + self.mCenterX
       self.mPosY = (self.mRadius * math.sin(self.mAngle)) * 1.1 + self.mCenterY

       # Work with moon color
       if globalTime <= 10.0:
           self.t = 0
       elif globalTime >= 12.0:
           self.t = 1
       else:
           self.t = (globalTime - 10.0) / (12.0 - 10.0)

           self.mG = (1-self.t)*0.25 + 0.75
           self.mB = (1-self.t)*0.45 + 0.45

           self.mMoon.SetColor(CL_Colorf(1, self.mG, self.mB, 1))

       mRadIncrease = (Core.Utils.Deg2Rad(95) * Core.Utils.GameSeconds(elapsed * 1000.0)) / (12.0*60.0*60.0)
       if self.mAngle <= Core.Utils.Deg2Rad(10):
           self.mAngle += mRadIncrease
       else:
           self.mRenderMoon = False
           pass

    def updateVisual(self, newX, newY):
        if not self.mRenderMoon:
            return

        self.mMoon.Draw(self.mGC, self.mPosX, self.mPosY)

    def getPosition(self):
        return CL_Pointf(self.mPosX, self.mPosY)

    def setPosition(self):
        print 'Setting position'

    def getRectangle(self):
        return CL_Rectf(self.mPosX - self.mMoon.GetWidth(),  \
                        self.mPosY - self.mMoon.GetHeight(), \
                        self.mPosX, self.mPosY + self.mMoon.GetHeight()/2.0)

    def checkCollision(self):
        return False

    @staticmethod
    def ParseMoon(node):
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

        moonHandle.setVisual(name, section, width, height)

        # Parse common settings
        radius = Core.Utils.Meters2Pixels(7)
        angle = Core.Utils.Deg2Rad(-90)

        radiusElement = node.GetElementsByTagName(CL_DomString("Radius")).Item(0).ToElement()
        if radiusElement.HasAttribute(CL_DomString("value")):
            radius = Core.Utils.Meters2Pixels(float(radiusElement.GetAttribute(CL_DomString("value")).CStr()))

        angleElement = node.GetElementsByTagName(CL_DomString("StartAngle")).Item(0).ToElement()
        if angleElement.HasAttribute(CL_DomString("value")):
            angle = Core.Utils.Deg2Rad(float(angleElement.GetAttribute(CL_DomString("value")).CStr()))

        moonHandle.setOrbitRadius(radius)
        moonHandle.setMoonAngle(angle)

        return moonHandle

World.Objects.TypesManager.RegisterParser("MoonObject", Moon.ParseMoon)
