import Core.LogManager
import Core.ApplicationManager
import Core.WorldManager
import Core.ResourceManager
import Core.LevelManager
import World.Objects.TypesManager
from World.Objects.Object import *

import Core.Utils
import random, math

class Foreground(Object):
    def __init__(self):
        super(Foreground, self).__init__()

        # Precalculated value
        self.mGC = Core.ApplicationManager.getInstance().GetGraphic()

        self.mYPos = 0
        self.mFixed = True

    def Init(self):
        self.mCamera = Core.LevelManager.getInstance().GetCamera()

    def GetTextureInfo(self):
        return [self.mSection, self.mTexture]

    def SetFixed(self, fixed):
        self.mFixed = fixed

    def GetFixed(self):
        return self.mFixed

    def SetVisual(self, texture, section, width, height):
        self.mTexture = texture
        self.mSection = section

        self.mImage = Core.ResourceManager.getInstance().GetImage(section, texture)
        if self.mImage.IsNull():
            print 'Failed to retreive image: ' + str(texture)

        koefX = koefY = 1.0
        if width > 0:
            koefX = width / self.mImage.GetWidth()
        if height > 0:
            koefY = height / self.mImage.GetHeight()

        self.mImage.SetScale(koefX, koefY)

    def Update(self, elapsed):
        pass

    def UpdateVisual(self, newX, newY):
        viewport = self.mCamera.GetViewport()
        if not self.mFixed:
            self.mYPos = viewport.top

        self.mImage.Draw(self.mGC, viewport.left * 0.5, self.mYPos)

    def GetPosition(self):
        return CL_Pointf(0, 0)

    def SetPosition(self, point):
        pass

    def GetRectangle(self):
        return CL_Rectf(0, 0, Core.Utils.ScreenResolutionX, Core.Utils.ScreenResolutionY)

    def CheckCollision(self):
        return False

    @staticmethod
    def ParseForeground(node):
        obj = Foreground()
        # Parse visuals -----------------------------------
        VisualTags = node.GetElementsByTagName("Visual")
        if VisualTags.GetLength() == 1:
            section = name = None
            width = height = -1.0

            childList = VisualTags.Item(0).GetChildNodes()
            for i in xrange(childList.GetLength()):
                tag2 = childList.Item(i).ToElement()
                if tag2.GetNodeName() == "Size":
                    width = float(tag2.GetAttribute("width"))
                    height = float(tag2.GetAttribute("height"))
                elif tag2.GetNodeName() == "Texture":
                    section = str(tag2.GetAttribute("section"))
                    name = str(tag2.GetAttribute("name"))
            obj.SetVisual(name, section, width, height)
        else:
            Core.LOG('Error: only one `Visual` tag may be in Foreground object!')

        # Parse additional ---------------------------------
        fixed = True;
        fixedElement = node.GetElementsByTagName("Fixed").Item(0).ToElement()
        if fixedElement.HasAttribute("value"):
            fixed = fixedElement.GetAttribute("value") == "true"
        obj.SetFixed(fixed)

        return obj

World.Objects.TypesManager.RegisterParser("ForegroundObject", Foreground.ParseForeground)
