import Core.LogManager
import Core.ApplicationManager
import Core.WorldManager
import Core.ResourceManager
import World.Objects.TypesManager
from World.Objects.Object import *
import Core.Utils
import random

class StarData:
    def __init__(self):
        self.x = random.randint(0, Core.Utils.ScreenResolutionX)
        # TODO: Calculate or get actualSize here!
        self.y = random.randint(0, Core.Utils.ScreenResolutionY)
        self.brightness = random.uniform(0.2, 1.0)

        # Choose random picture for current star
        numStars = Core.ResourceManager.SectionNumChildren("Stars")
        star = random.randint(0, numStars-1)
        self.imageHandle = Core.ResourceManager.getInstance().GetImage("Stars", str(star))

        scale = random.uniform(0.09, 0.2)
        self.imageHandle.SetScale(scale, scale)
        self.imageHandle.SetAlpha(self.brightness)

class Stars(Object):
    def __init__(self, maxObjects):
        super(Stars, self).__init__()

        # We store drop objects here
        self.mStarData = []

        # Precalculated value
        self.mMaxObjects = int(maxObjects)
        self.mBloomSize = 0.0
        self.t1 = self.t2 = 0
        self.mNight = self.mDrawStars = False

        self.mGC = Core.ApplicationManager.getInstance().GetGraphic()
        self.mWorldManagerHandle = Core.WorldManager.getInstance()

    def init(self):
        # TODO: Read actual size here...
        for i in xrange(self.mMaxObjects-1):
            self.mStarData.append(StarData())

    def update(self, elapsed):
       globalTime = self.mWorldManagerHandle.GetAbsTime()
       if globalTime <= 10.0:
           self.t1 = 0
       elif globalTime >= 14.0:
           self.t1 = 1.0
           self.mNight = True
       else:
           self.mDrawStars = True
           self.t1 = ((globalTime - 10.0) / (14.0 - 10.0))
           self.mBloomSize = self.t1

       if self.mNight:
           if globalTime <= 20.8:
               self.t2 = 0
           elif globalTime >= 23.8:
               self.t2 = 1.0
               self.mNight = False
               self.mDrawStars = False
           else:
               self.t2 = ((globalTime - 20.8) / (23.8 - 20.8))
               self.mBloomSize = 1.0 - self.t2

       # If it's day now skip rendering
       if not self.mDrawStars:
           return

       if self.mNight:
           self.mBloomSize += 0.001
       else:
           self.mBloomSize -= 0.001

    def updateVisual(self, newX, newY):
        if not self.mDrawStars:
            return

        for i in xrange(self.mMaxObjects-1):
            # Blink star
            if random.randint(0, 70) == 0:
                continue

            current = self.mStarData[i]
            if self.mBloomSize <= current.brightness:
                current.imageHandle.SetAlpha(self.mBloomSize)

            current.imageHandle.Draw(self.mGC, current.x, current.y)

    def getPosition(self):
        return CL_Pointf(0, 0)

    def setPosition(self):
        print 'Setting position'

    def getRectangle(self):
        return CL_Rectf(0, 0, Core.Utils.ScreenResolutionX, Core.Utils.ScreenResolutionY)

    def checkCollision(self):
        return False

    @staticmethod
    def ParseStars(node):
        maxStars = 0

        if node.HasAttribute(CL_DomString("maxStars")):
            maxStars = float(node.GetAttribute(CL_DomString("maxStars")).CStr())

        area = Core.Utils.Pixels2Meters(Core.Utils.ScreenResolutionX) * \
              Core.Utils.Pixels2Meters(Core.Utils.ScreenResolutionY)

        return Stars(area * maxStars)

World.Objects.TypesManager.RegisterParser("StarsObject", Stars.ParseStars)
