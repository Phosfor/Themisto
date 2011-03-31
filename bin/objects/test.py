import Core.LogManager
import World.Objects.TypesManager
from World.Objects.Object import *
import Core.Utils

class TestObj(Object):
    def __init__(self):
        print 'Initing of test object...'

    def init(self):
        print 'init function of Test'

    def update(self, elapsed):
        print 'update with elapsed'

    def updateVisual(self, newX, newY):
        print 'Update visual with newX, newY'

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
    def ParseTestObj(node, desc):
        pass

print 'registering parser here....'
World.Objects.TypesManager.RegisterParser("test_obj", TestObj.ParseTestObj)
World.Objects.TypesManager.DumpParsers();
