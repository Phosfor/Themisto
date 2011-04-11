import Core.LogManager
import Core.WorldManager
import World.Objects.TypesManager
from World.Objects.Object import *
from Core.Utils import *

class Sky(Object):
    def __init__(self):
        # Don't forget this for any derivative class
        super(Sky, self).__init__()

        # Color of top part of the sky
        self.mColorTop = CL_Colorf(25.0/255.0, 60.0/255.0, 141.0/255.0, 1.0)
        self.mColorMiddle = CL_Colorf(54.0/255.0, 119.0/255.0, 198.0/255.0, 1.0)
        self.mColorBottom = CL_Colorf(83.0/255.0, 178.0/255.0, 234.0/255.0, 1.0)

        # Save values to use in future...
        self.mColorTopConst = self.mColorTop
        self.mColorMiddleConst = self.mColorMiddle
        self.mColorBottomConst = self.mColorBottom

        # New color of the top part (transaction)
        self.mColorTopTrans = CL_Colorf(53.0/255.0, 23.0/255.0, 104.0/255.0, 1.0)
        self.mColorMiddleTrans = CL_Colorf(54.0/255.0, 85.0/255.0, 198.0/255.0, 1.0)
        self.mColorBottomTrans = CL_Colorf(234.0/255.0, 197.0/255.0, 83.0/255.0, 1.0)

        # Time interval
        self.t1 = self.t2 = self.t3 = 0.0

        # Is it night now?
        self.mNight = False

        # Getting of Graphic Context
        self.mGC = Core.ApplicationManager.getInstance().GetGraphic()

        # First part of the sky:
        self.quad1 = CL_Rectf(0, 0, ScreenResolutionX, ScreenResolutionY/2.0)
        self.quad2 = CL_Rectf(0, ScreenResolutionY/2.0, ScreenResolutionX, ScreenResolutionY)

        # Instance of world manager
        self.worldManagerHandle = Core.WorldManager.getInstance()

    def init(self):
        # Nothing to do here for the sky...
        pass

    def update(self, elapsed):
        globalTime = self.worldManagerHandle.GetAbsTime()

        #  First transaction part [day] -----------------------------------------------------
        if globalTime <= 7.0:
            self.t1 = 0.0
        elif globalTime >= 10.0:
            self.t1 = 1.0
        else:
            self.t1 = (globalTime - 7.0) / (10.0 - 7.0)

            # Process color transaction
            r = self.t1 * (self.mColorBottomTrans.r - self.mColorBottomConst.r) + self.mColorBottomConst.r
            g = self.t1 * (self.mColorBottomTrans.g - self.mColorBottomConst.g) + self.mColorBottomConst.g
            b = self.t1 * (self.mColorBottomTrans.b - self.mColorBottomConst.b) + self.mColorBottomConst.b

            self.mColorBottom = CL_Colorf(r, g, b, 1.0)

        #  Second transaction part [night] --------------------------------------------------
        if globalTime <= 12.0:
            self.t2 = 0.0
        elif globalTime >= 14.0:
            self.t2 = 1.0
            self.mNight = True
        else:
            self.t2 = (globalTime - 12.0) / (14.0 - 12.0)

            # Remove atmosphere scattering effect
            r = self.t2 * (self.mColorBottomConst.r - self.mColorBottomTrans.r) + self.mColorBottomTrans.r
            g = self.t2 * (self.mColorBottomConst.g - self.mColorBottomTrans.g) + self.mColorBottomTrans.g
            b = self.t2 * (self.mColorBottomConst.b - self.mColorBottomTrans.b) + self.mColorBottomTrans.b

            self.mColorBottom = CL_Colorf(r, g, b, 1.0)

            # Night becomes here... [middle part]
            r = self.t2 * (self.mColorTopTrans.r - self.mColorTopConst.r) + self.mColorTopConst.r
            g = self.t2 * (self.mColorTopTrans.g - self.mColorTopConst.g) + self.mColorTopConst.g
            b = self.t2 * (self.mColorTopTrans.b - self.mColorTopConst.b) + self.mColorTopConst.b

            self.mColorTop = CL_Colorf(r, g, b, 1.0)

            # Night becomes also for the middle part of the sky
            r = self.t2 * (self.mColorMiddleTrans.r - self.mColorMiddleConst.r) + self.mColorMiddleConst.r
            g = self.t2 * (self.mColorMiddleTrans.g - self.mColorMiddleConst.g) + self.mColorMiddleConst.g
            b = self.t2 * (self.mColorMiddleTrans.b - self.mColorMiddleConst.b) + self.mColorMiddleConst.b

            self.mColorMiddle = CL_Colorf(r, g, b, 1.0)

        #  Third transaction part [middle] ---------------------------------------------------
        if self.mNight:
            if globalTime <= 21.5:
                self.t3 = 0.0
            elif globalTime >= 23.8:
                t3 = 1.0
                self.mNight = False
            else:
                self.t3 = (globalTime - 21.5) / (23.8 - 21.5)

                # Make top part of the sky lighter!
                r = self.t3 * (self.mColorTopConst.r - self.mColorTopTrans.r) + self.mColorTopTrans.r;
                g = self.t3 * (self.mColorTopConst.g - self.mColorTopTrans.g) + self.mColorTopTrans.g;
                b = self.t3 * (self.mColorTopConst.b - self.mColorTopTrans.b) + self.mColorTopTrans.b;

                self.mColorTop = CL_Colorf(r, g, b, 1.0)

                # Make middle part of the sky lighter
                r = self.t3 * (self.mColorMiddleConst.r - self.mColorMiddleTrans.r) + self.mColorMiddleTrans.r;
                g = self.t3 * (self.mColorMiddleConst.g - self.mColorMiddleTrans.g) + self.mColorMiddleTrans.g;
                b = self.t3 * (self.mColorMiddleConst.b - self.mColorMiddleTrans.b) + self.mColorMiddleTrans.b;

                self.mColorMiddle = CL_Colorf(r, g, b, 1.0);

    def updateVisual(self, newX, newY):
        CL_Draw.GradientFill(self.mGC, self.quad1, CL_Gradient(self.mColorTop, self.mColorMiddle))
        CL_Draw.GradientFill(self.mGC, self.quad2, CL_Gradient(self.mColorMiddle, self.mColorBottom))

    def getPosition(self):
        return CL_Pointf(0, 0)

    def setPosition(self):
        print 'Setting position of the Sky object...'

    def getRectangle(self):
        return CL_Rectf(0, 0, Core.Utils.ScreenResolutionX, Core.Utils.ScreenResolutionY)

    def checkCollision(self):
        # Sky can't collide with mouse
        return False

    @staticmethod
    def ParseSky(node):
        return Sky()

World.Objects.TypesManager.RegisterParser("SkyObject", Sky.ParseSky)
