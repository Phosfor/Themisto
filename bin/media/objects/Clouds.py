import Core.LogManager
import Core.ApplicationManager
import Core.WorldManager
import World.Objects.TypesManager
from World.Objects.Object import *
import Core.Utils
import random

class CloudData:
    def __init__(self):
        self.x = random.randint(0, Core.Utils.ScreenResolutionX)
        self.x_speed = self.speed_koef = self.y_offset = 0
        self.cloudType = self.timeout = 0
        self.image = None

class Clouds(Object):
    def __init__(self, maxObjects):
        super(Clouds, self).__init__()

        # We store drop objects here
        self.mClouds = []

        # Precalculated value
        self.mMaxObjects = int(maxObjects)

        self.mAppManagerHandle = Core.ApplicationManager.getInstance()
        self.mGC = self.mAppManagerHandle.GetGraphic()
        self.mWorldManagerHandle = Core.WorldManager.getInstance()

    def processClouds(self, windPower, curObject, firsTime):
        curObject.x_speed = 0

        size = Core.ResourceManager.SectionNumChildren("Clouds")
        cloud = random.randint(0, size-1)
        curObject.image = Core.ResourceManager.getInstance().GetImage("Clouds", str(cloud))
        color = ColorRGB(42, 72, 85)
        color.SetAlpha(1.1)
        curObject.image.SetColor(color)

        if not firsTime:
            if windPower < 0:
                curObject.x = Core.Utils.ScreenResolutionX + curObject.image.GetWidth()
            else:
                curObject.x = 0 - curObject.image.GetWidth()

            curObject.timeout = random.randint(0, 350)
        else:
            curObject.x = random.randint(0, Core.Utils.ScreenResolutionX)
            curObject.timeout = 0

        curObject.y_offset = random.randint(0, Core.Utils.ScreenResolutionY)
        curObject.speed_koef = random.randint(5, 15)


    def Init(self):
        windPower = self.mWorldManagerHandle.GetWindPower()

        # Init first rain drops
        for i in xrange(self.mMaxObjects-1):
            self.mClouds.append(CloudData())
            self.processClouds(windPower, self.mClouds[i], True)

    def Update(self, elapsed):
        windPower = self.mWorldManagerHandle.GetWindPower()
        newSpeed = windPower * elapsed / 1000 * 0.5

        for i in xrange(self.mMaxObjects-1):
            current = self.mClouds[i]
            if current.timeout > 0:
                current.timeout -= 1
            else:
                if windPower > 0:
                    if current.x > Core.Utils.ScreenResolutionX + current.image.GetWidth():
                        self.processClouds(windPower, current, False)
                else:
                    if current.x < -current.image.GetWidth():
                        self.processClouds(windPower, current, False)

                current.x += current.x_speed * elapsed
                current.x_speed += current.speed_koef * newSpeed

    def UpdateVisual(self, newX, newY):
        for i in xrange(self.mMaxObjects-1):
            current = self.mClouds[i]
            if current.timeout <= 0:
                current.image.Draw(self.mGC, current.x, current.y_offset)

    def GetPosition(self):
        return CL_Pointf(0, 0)

    def SetPosition(self):
        print 'Setting position'

    def GetRectangle(self):
        return CL_Rectf(0, 0, Core.Utils.ScreenResolutionX, Core.Utils.ScreenResolutionY)

    def CheckCollision(self):
        return False

    @staticmethod
    def ParseClouds(node):
        maxClouds = 0

        if node.HasAttribute(CL_DomString("maxClouds")):
            maxClouds = float(node.GetAttribute(CL_DomString("maxClouds")).CStr())

        area = Core.Utils.Pixels2Meters(Core.Utils.ScreenResolutionX) * \
              Core.Utils.Pixels2Meters(Core.Utils.ScreenResolutionY)

        return Clouds(area * maxClouds)

World.Objects.TypesManager.RegisterParser("CloudsObject", Clouds.ParseClouds)
