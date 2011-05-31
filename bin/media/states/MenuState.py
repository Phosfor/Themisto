import Core.LogManager
import Core.ApplicationManager
import Core.WorldManager
import Core.ResourceManager
import Core.LevelManager
import Core.InputManager
import Core.PhysicManager
import Core.StateManager
import Core.GuiManager
import Core.SoundManager
import World.Objects.TypesManager

from Core.InputManager import *

import Core.Utils
from Physic.Physic import *

class MenuState(Core.StateManager.State):
    def __init__(self):
        super(MenuState, self).__init__()

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

    def InitGui(self):
        handle = self.guiHandle.GetHandle()
        wrapper = self.guiHandle.GetWrapper()
        clientArea = self.appHandle.GetGraphic().GetCliprect()

        # Load css for current widgets
        handle.SetCssDocument(Core.Utils.getInstance().GetMediaFolder() + '/local.css');

        self.mConsoleLabel = CL_Label(wrapper)
        self.mConsoleLabel.SetClassName('console-label')
        self.mConsoleLabel.SetText('>')
        self.mConsoleLabel.SetVisible(True, True)

        self.mConsole = CL_LineEdit(wrapper)
        self.mConsole.SetGeometry(CL_Recti(30, clientArea.GetHeight()-30, clientArea.GetWidth() + 30, clientArea.GetHeight()))
        self.mConsole.SetVisible(True, True)
        self.mConsole.SetClassName('console-edit')

    def Init(self):
        self.worldHandle = Core.WorldManager.getInstance()
        self.inputHandle = Core.InputManager.getInstance()
        self.levelHandle = Core.LevelManager.getInstance()
        self.soundHandle = Core.SoundManager.getInstance()
        self.guiHandle = Core.GuiManager.getInstance()
        self.appHandle = Core.ApplicationManager.getInstance()
        self.physicHandle = Core.PhysicManager.getInstance()

        self.mDrawDebug = False
        self.mDetailedOutput = False
        self.mShowConsole = False

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
        raining = self.soundHandle.AddSoundObject('BgRaining', 'Rain.wav').GetBuffer()
        raining.SetVolume(0.2)
        raining.Play(True)

        # Init game gui
        self.InitGui()

    def Update(self):
        self.worldHandle.Update()

        if self.mDrawDebug:
            self.mPhysicWorld.DrawDebugData()

    def Shutdown(self):
        print 'Shutdown...'

    def Type(self):
        return 'Python MenuState'
