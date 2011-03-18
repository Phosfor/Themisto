import Core.LogManager
import Core.ConfigManager
import Core.Utils

Core.LogManager.LOG('fucking test!')

coreHandle = Core.Utils.getInstance()
print Core.Utils.Pi
print coreHandle.GetTemplateFolder()

handle = Core.ConfigManager.getInstance()
print handle.GetValue_s("application.media-folder")
print type(handle.GetValue_s("application.media-folder"))
