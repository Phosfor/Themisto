import Core.LogManager
import Core.ApplicationManager
import Core.WorldManager
import Core.ResourceManager
import Core.LevelManager
import Core.InputManager
import Core.PhysicManager
import Core.StateManager
import Core.SoundManager
import World.Objects.TypesManager

from Core.InputManager import *

import Core.Utils
from Physic.Physic import *

class MenuState(Core.StateManager.State):
    def __init__(self):
        super(MenuState, self).__init__()

        self.worldHandle = Core.WorldManager.getInstance()
        self.inputHandle = Core.InputManager.getInstance()
        self.levelHandle = Core.LevelManager.getInstance()
        self.soundHandle = Core.SoundManager.getInstance()
        self.physicHandle = Core.PhysicManager.getInstance()

        self.mDrawDebug = False
        self.mDetailedOutput = False
        self.mShowConsole = False

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
        # Init physic Drag&Drop
        self.mDnD = DebugDragAndDrop()
        self.mDnD.Init()

        # Load level data
        Core.LevelManager.LoadScene('NewObjects.xml')

        # Init state main attributes global
        self.mGC = Core.ApplicationManager.getInstance().GetGraphic()
        self.mCamera = self.levelHandle.GetCamera()
        self.mPhysicWorld = self.physicHandle.GetWorld()

        self.inputHandle.ConnectKeyDown(self.OnKeyDown)

        # Add raining sound
        raining = self.soundHandle.AddSoundObject('BgRaining', 'Rain.wav')
        raining.GetBuffer().SetVolume(0.2)
        raining.GetBuffer().Play(True)

    def Update(self):
        self.worldHandle.Update()

        if self.mDrawDebug:
            self.mPhysicWorld.DrawDebugData()

    def Shutdown(self):
        print 'Shutdown...'

    def Type(self):
        return 'Python MenuState'
