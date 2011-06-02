import Core.LogManager
import Core.ApplicationManager
import Core.WorldManager
import World.Objects.TypesManager
from World.Objects.Object import *
import Core.Utils
import random

class LeafData:
    def __init__(self):
        self.x = self.y = self.x_speed = self.y_speed = 0.0
        self.k1 = self.k2 = self.speed_koef = 0.0
        self.addedAngle = 0.0
        self.y_sin = 0

        self.image = None
        self.animation = False

class Leaves(Object):
    def __init__(self, maxObjects):
        super(Leaves, self).__init__()

        self.mLeaves = []

        # Precalculated value
        self.mMaxObjects = int(maxObjects)
        self.functionValue = +1

        self.mAppManagerHandle = Core.ApplicationManager.getInstance()
        self.mGC = self.mAppManagerHandle.GetGraphic()
        self.mWorldManagerHandle = Core.WorldManager.getInstance()

    def processLeaves(self, windPower, curObject, firsTime):
        curObject.x_speed = curObject.y_speed = 0.0
        curObject.timer = curObject.addedAngle = 0.0
        curObject.animation = False

        curObject.y = Core.Utils.ScreenResolutionY - Core.Utils.ScreenResolutionY * random.uniform(0.2, 0.8)

        curObject.k1 = random.randint(20, 50)
        curObject.k2 = random.uniform(0.3, 1.0)

        size = Core.ResourceManager.SectionNumChildren("Leaves")
        leaf = random.randint(0, size-1)
        curObject.image = Core.ResourceManager.getInstance().GetSprite("Leaves", str(leaf))
        angle = random.randint(1, 360)
        curObject.image.SetAngle(CL_Angle.FromDegrees(angle))

        scale = random.uniform(0.5, 0.9)
        curObject.image.SetScale(scale, scale)

        color = ColorRGB(17, 30, 35)
        curObject.image.SetColor(color)

        curObject.speed_koef = random.randint(50, 120)

        if not firsTime:
            if windPower < 0:
                curObject.x = Core.Utils.ScreenResolutionX + curObject.image.GetWidth()
            else:
                curObject.x = 0 - curObject.image.GetWidth()

            curObject.timeout = random.randint(0, 250)
        else:
            curObject.x = random.randint(0, Core.Utils.ScreenResolutionX)
            curObject.timeout = 0

    def Init(self):
        windPower = self.mWorldManagerHandle.GetWindPower()

        for i in xrange(self.mMaxObjects-1):
            self.mLeaves.append(LeafData())
            self.processLeaves(windPower, self.mLeaves[i], True)

    def Update(self, elapsed):
        windPower = self.mWorldManagerHandle.GetWindPower()

        elapsed_ = elapsed # /  1000
        newSpeed1 = windPower * elapsed_ * 0.4
        newSpeed2 = Core.Utils.Gravity * elapsed_

        for i in xrange(self.mMaxObjects-1):
            current = self.mLeaves[i]
            if current.timeout > 0:
                current.timeout -= 1
            else:
                if windPower > 0:
                    if current.x > Core.Utils.ScreenResolutionX + current.image.GetWidth():
                        self.processLeaves(windPower, current, False)
                else:
                    if current.x < -current.image.GetWidth():
                        self.processLeaves(windPower, current, False)

                current.x += current.x_speed * elapsed_
                current.y += current.y_speed * elapsed_

                # Process animation
                y = current.k1 * math.sin(current.k2 * Core.Utils.Deg2Rad(current.x)) + current.y
                tempValue = current.k1 * current.k2 * math.cos(current.k2 * Core.Utils.Deg2Rad(current.x))
                current.y_sin = y

                if tempValue * self.functionValue < 0:
                    current.animation = True
                functionValue = tempValue

                if current.animation:
                    angle = random.randint(1, 3)
                    current.addedAngle += angle
                    current.image.RotatePitch(CL_Angle.FromDegrees(angle))

                    if current.addedAngle >= 180:
                        current.animation = False
                        current.addedAngle = 0

                current.x_speed = current.speed_koef * newSpeed1
                current.y_speed = newSpeed2

    def UpdateVisual(self, newX, newY):
        for i in xrange(self.mMaxObjects-1):
            current = self.mLeaves[i]
            if current.timeout <= 0:
                current.image.Draw(self.mGC, current.x, current.y_sin)

    def GetPosition(self):
        return CL_Pointf(0, 0)

    def SetPosition(self):
        print 'Setting position'

    def GetRectangle(self):
        return CL_Rectf(0, 0, Core.Utils.ScreenResolutionX, Core.Utils.ScreenResolutionY)

    def CheckCollision(self):
        return False

    @staticmethod
    def ParseLeaves(node):
        maxLeaves = 0

        if node.HasAttribute("maxLeaves"):
            maxLeaves = float(node.GetAttribute("maxLeaves"))

        area = Core.Utils.Pixels2Meters(Core.Utils.ScreenResolutionX) * \
              Core.Utils.Pixels2Meters(Core.Utils.ScreenResolutionY)

        return Leaves(area * maxLeaves)

World.Objects.TypesManager.RegisterParser("LeavesObject", Leaves.ParseLeaves)
