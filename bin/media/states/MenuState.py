import Core.LogManager
import Core.ApplicationManager
import Core.WorldManager
import Core.ResourceManager
import Core.LevelManager
import Core.InputManager
import Core.StateManager
import World.Objects.TypesManager

import Core.Utils

class MenuState(Core.StateManager.State):
    def __init__(self):
        super(MenuState, self).__init__()

        self.worldHandle = Core.WorldManager.getInstance()
        self.inputHandle = Core.InputManager.getInstance()

    def KeyDown(self, InputEvent, InputState):
        print 'Key Down!: ',
        print InputEvent.Str.CStr()

    def Init(self):
        print 'Init of MenuState...'
        Core.LevelManager.LoadScene('NewObjects.xml')

        self.inputHandle.ConnectKeyDown(self.KeyDown)

    def Update(self):
        self.worldHandle.Update()

    def Shutdown(self):
        print 'Shutdown...'

    def Type(self):
        return 'Python MenuState'
