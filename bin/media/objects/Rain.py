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
        self.mDropColorAlpha = 0.6
        self.mDropColor = ColorRGB(150, 150, 150)
        self.mDropColor.SetAlpha(self.mDropColorAlpha)

        # Precalculated value
        self.mKoef1 = ScreenResolutionY / Core.Utils.Gravity
        self.mMaxDrops = int(maxDrops)
        self.kTail = 0.025
        self.kYSpeed = 0.6

        self.mGC = Core.ApplicationManager.getInstance().GetGraphic()
        self.mWorldManagerHandle = Core.WorldManager.getInstance()

        penAntialiased = CL_Pen()
        penAntialiased.EnableLineAntialiasing(True)
        self.mGC.SetPen(penAntialiased)

    # Control rain interface
    def SetDropAlpha(self, value):
        self.mDropColorAlpha = value
        self.mDropColor.SetAlpha(value)

    def SetDropColor(self, color):
        self.mDropColor = color
        self.mDropColor.SetAlpha(self.mDropColorAlpha)

    def SetTail(self, value):
        self.kTail = value

    def SetYSpeedKoef(self, value):
        self.kYSpeed = value
    # ----------------------

    def processDrops(self, windPower, curObject):
        left = right = 0
        x1 = int(self.mKoef1 * windPower)

        # Look at wind direction
        if windPower < 0:
            left = 0
            right = Core.Utils.ScreenResolutionX - x1
        else:
            left = -x1
            right = Core.Utils.ScreenResolutionX

        # Start drop position
        curObject.x = random.randint(left, right)
        curObject.y = 0

        curObject.x_speed = windPower
        curObject.y_speed = Core.Utils.Gravity * self.kYSpeed

        curObject.timeout = random.randint(0, 130)

    def Init(self):
        windPower = self.mWorldManagerHandle.GetWindPower()

        # Init first rain drops
        for i in xrange(self.mMaxDrops-1):
            self.mDropData.append(DropData())
            self.processDrops(windPower, self.mDropData[i])

    def Update(self, elapsed):
        windPower = self.mWorldManagerHandle.GetWindPower()
        newXSpeed = windPower * elapsed
        newYSpeed = Core.Utils.Gravity * elapsed * self.kYSpeed

        for i in xrange(self.mMaxDrops-1):
            current = self.mDropData[i]
            if current.timeout > 0:
                current.timeout -= 1
            else:
                # Maybe current drop is out of the screen?
                if current.y > Core.Utils.ScreenResolutionY or \
                   (windPower > 0 and current.x > Core.Utils.ScreenResolutionX) or \
                   (windPower < 0 and current.x < Core.Utils.ScreenResolutionX):
                       self.processDrops(windPower, current)

                # If not, change it's position and speed
                current.x += current.x_speed * elapsed
                current.y += current.y_speed * elapsed

                current.x_speed += newXSpeed
                current.y_speed += newYSpeed

    def UpdateVisual(self, newX, newY):
        for i in xrange(self.mMaxDrops-1):
            current = self.mDropData[i]
            if current.timeout <= 0:
                CL_Draw.Line(self.mGC, current.x, current.y,
                    current.x - current.x_speed * self.kTail,
                    current.y - current.y_speed * self.kTail,
                    self.mDropColor)

    def GetPosition(self):
        return CL_Pointf(0, 0)

    def SetPosition(self):
        print 'Setting position'

    def GetRectangle(self):
        return CL_Rectf(0, 0, Core.Utils.ScreenResolutionX, Core.Utils.ScreenResolutionY)

    def CheckCollision(self):
        return False

    @staticmethod
    def ParseRain(node):
        maxDrops = 0

        if node.HasAttribute(CL_DomString("maxDrops")):
            maxDrops = float(node.GetAttribute(CL_DomString("maxDrops")).CStr())

        area = Core.Utils.Pixels2Meters(Core.Utils.ScreenResolutionX) * \
              Core.Utils.Pixels2Meters(Core.Utils.ScreenResolutionY)

        return Rain(area * maxDrops)

World.Objects.TypesManager.RegisterParser("RainObject", Rain.ParseRain)
