import Core.LogManager
import Core.ApplicationManager
import Core.WorldManager
import World.Objects.TypesManager
from World.Objects.Object import *
import Core.Utils
import random

class DropData:
    def __init__(self):
        self.x = random.randint(0, Core.Utils.ScreenResolutionX)
        self.y = 0

        self.timeout = random.randint(0, 130)
        self.x_speed = self.y_speed = 0

class Rain(Object):
    def __init__(self, maxDrops):
        super(Rain, self).__init__()

        # We store drop objects here
        self.mDropData = []
        self.mDropColor = CL_Colorf(0, 0, 0, random.uniform(0.3, 0.8))

        # Precalculated value
        self.mKoef1 = ScreenResolutionY / Core.Utils.Gravity
        self.mMaxDrops = int(maxDrops)
        self.kTail = 0.015

        self.mGC = Core.ApplicationManager.getInstance().GetGraphic()
        self.mWorldManagerHandle = Core.WorldManager.getInstance()

    def processDrops(self, windPower, curObject, firstTime = False):
        left = right = 0
        x1 = int(self.mKoef1 * windPower)

        # Look at wind direction
        if windPower < 0:
            left = 0
            right = Core.Urils.ScreenResolutionX - x1
        else:
            left = -x1
            right = Core.Utils.ScreenResolutionX

        # Start drop position
        curObject.x = random.randint(left*3, right-left)

        curObject.x_speed = windPower
        curObject.y_speed = Core.Utils.Gravity * 0.7

        # First and further initializations are different
        if firstTime:
            curObject.timeout = 0
            curObject.y = random.randint(0, Core.Utils.ScreenResolutionY)
        else:
            curObject.timeout = random.randint(0, 130)
            curObject.y = 0

    def init(self):
        windPower = self.mWorldManagerHandle.GetWindPower()

        # Init first rain drops
        for i in xrange(self.mMaxDrops-1):
            self.mDropData.append(DropData())
            self.processDrops(windPower, self.mDropData[i], True)

    def update(self, elapsed):
        windPower = self.mWorldManagerHandle.GetWindPower()
        newXSpeed = windPower * elapsed
        newYSpeed = Core.Utils.Gravity * elapsed * 0.7

        for i in xrange(self.mMaxDrops-1):
            current = self.mDropData[i]
            if current.timeout > 0:
                current.timeout -= 1
            else:
                if current.y > Core.Utils.ScreenResolutionY or \
                   current.x > Core.Utils.ScreenResolutionX or \
                   current.x < 0:
                       self.processDrops(windPower, current)

                current.x += current.x_speed * elapsed
                current.y += current.y_speed * elapsed

                current.x_speed += newXSpeed
                current.y_speed += newYSpeed

    def updateVisual(self, newX, newY):
        for i in xrange(self.mMaxDrops-1):
            current = self.mDropData[i]

            # Draw the drop
            CL_Draw.Line(self.mGC, current.x, current.y,
                current.x - current.x_speed * self.kTail,
                current.y - current.y_speed * self.kTail,
                self.mDropColor)

    def getPosition(self):
        return CL_Pointf(0, 0)

    def setPosition(self):
        print 'Setting position'

    def getRectangle(self):
        return CL_Rectf(0, 0, Core.Utils.ScreenResolutionX, Core.Utils.ScreenResolutionY)

    def checkCollision(self):
        return False

    @staticmethod
    def ParseRain(node):
#        maxDrops = 0
#
#        if node.HasAttribute(CL_DomString("maxDrops")):
#            maxDrops = float(node.GetAttribute(CL_DomString("maxDrops")))
#
#        area = Core.Utils.Pixels2Meters(Core.Utils.ScreenResolutionX) * \
#               Core.Utils.Pixels2Meters(Core.Utils.ScreenResolutionY)
#
#        return Rain(area * maxDrops)
         return Rain(300)

World.Objects.TypesManager.RegisterParser("RainObject", Rain.ParseRain)
