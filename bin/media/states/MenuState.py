import Core.LogManager
import Core.ApplicationManager
import Core.WorldManager
import Core.ResourceManager
import Core.LevelManager
import Core.StateManager
import World.Objects.TypesManager

import Core.Utils

class MenuState(Core.StateManager.State):
    def __init__(self):
        super(MenuState, self).__init__()

    def Init(self):
        print 'Init of MenuState...'

    def Update(self):
        print 'Updating of MenuState...'

    def Shutdown(self):
        print 'Shutdown...'

    def Type(self):
        return 'Python MenuState'
