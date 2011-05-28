import Core.LogManager
import Core.ApplicationManager
import Core.WorldManager
import Core.ResourceManager
import Core.LevelManager
import Core.InputManager
import Core.StateManager
import World.Objects.TypesManager

from Core.InputManager import *

import Core.Utils

class MenuState(Core.StateManager.State):
    def __init__(self):
        super(MenuState, self).__init__()

        self.worldHandle = Core.WorldManager.getInstance()
        self.inputHandle = Core.InputManager.getInstance()
        self.levelHandle = Core.LevelManager.getInstance()

        self.mDrawDebug = False

    def OnKeyDown(self, InputEvent, InputState):
        keyId = InputEvent.Id

        # If Ctrl+D is pressed - enable debug mode
        if keyId == CL_KEY_D and self.inputHandle.IsKeyPressed(CL_KEY_CONTROL):
               self.mDrawDebug = not self.mDrawDebug
               self.mCamera.SetDrawDebugData(self.mDrawDebug)

        # Process camera movement
        cameraSpeed = self.mCamera.GetSpeed()
        if not self.inputHandle.IsKeyPressed(CL_KEY_CONTROL):
            if keyId == CL_KEY_RIGHT:
                self.mCamera.Translate(-cameraSpeed, 0)
            elif keyId == CL_KEY_LEFT:
                self.mCamera.Translate(cameraSpeed, 0)
            elif keyId == CL_KEY_UP:
                self.mCamera.Translate(0, cameraSpeed)
            elif keyId == CL_KEY_DOWN:
                self.mCamera.Translate(0, -cameraSpeed)

    def Init(self):
        print 'Init of MenuState...'
        Core.LevelManager.LoadScene('NewObjects.xml')

        self.mCamera = self.levelHandle.GetCamera()
        self.inputHandle.ConnectKeyDown(self.OnKeyDown)

    def Update(self):
        self.worldHandle.Update()

    def Shutdown(self):
        print 'Shutdown...'

    def Type(self):
        return 'Python MenuState'
