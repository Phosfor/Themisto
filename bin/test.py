import Core.LogManager
import Core.ConfigManager
import Core.Utils

Core.LogManager.LOG('fucking test!')
print Core.Utils.Pi
print Core.Utils.GetTemplateFolder()

handle = Core.ConfigManager.getInstance()
print handle.GetValue_s("application.media-folder")
print type(handle.GetValue_s("application.media-folder"))
