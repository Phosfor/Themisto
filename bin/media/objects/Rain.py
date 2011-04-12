import Core.LogManager
import Core.ApplicationManager
import Core.WorldManager
import World.Objects.TypesManager
from World.Objects.Object import *
import Core.Utils
import random

class DropData:
    def __init__:
        self.x = random.randint(0, Core.Utils.ScreenResolutionX)
        self.y = 0

        self.timeout = random.randint(0, 130)
        self.x_speed = self.y_speed = 0

class Rain(Object):
    def __init__(self, maxDrops):
        super(Rain, self).__init__()

        # We store drop objects here
        self.mDropData = []
        self.mDropColor = CL_Colorf(1.0, 1.0, 1.0, random.uniform(3.0, 8.0))

        # Precalculated value
        self.mKoef1 = ScreenResolutionY / Core.Utils.Gravity
        self.mMaxDrops = maxDrops

        self.mGC = Core.ApplicationManager.getInstance().GetGraphic()

    def processDrops(self, windPower, curObject, firstTime = False):
        left = right = 0
        x1 = self.mKoef1 * windPower

        # Look at wind direction
        if windPower < 0:
            left = 0
            right = Core.Urils.ScreenResolutionX - x1
        else:
            left = -x1
            right = Core.Utils.ScreenResolutionX

        # Start drop position (y is 0)
        curObject.x = random.randint(left, right-left)
        curObject.x_speed = windPower
        curObject.y_speed = Core.Utils.Gravity * 0.7

        # First and further initializations are different
        if firstTime:
            curObject.timeout = 0
            curObject.y = random.randint(0, Core.Utils.ScreenResolutionY)
        else:
            curObject.timeout = randint(0, 130)
            curObject.y = 0

    def init(self):
        windPower = Core.WorldManager.getInstance().GetWindPower()

        # Init first rain drops
        for i in xrange(self.mMaxDrops):
            self.mDropData.push(DropData())
            self.processDrops(windPower, self.mDropData[i], True)

    def update(self, elapsed):
        pass
        #print 'update with elapsed'

    def updateVisual(self, newX, newY):
        pass
        #print 'Updating object with ZIndex: ' + str(self.GetIndex())

    def getPosition(self):
        print 'returning position'
        return CL_Pointf(0, 0)

    def setPosition(self):
        print 'Setting position'

    def getRectangle(self):
        return CL_Rectf(0, 0, Core.Utils.ScreenResolutionX, Core.Utils.ScreenResolutionY)

    def checkCollision(self):
        return False

    @staticmethod
    def ParseRain(node):
        return TestObj()

World.Objects.TypesManager.RegisterParser("RainObject", Rain.ParseRain)
