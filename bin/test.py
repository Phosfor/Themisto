import Core.LogManager
import Core.ConfigManager
import Core.ApplicationManager
import Core.Utils
import Core.GuiManager

Core.LogManager.LOG('fucking test!')

coreHandle = Core.Utils.getInstance()
print Core.Utils.Pi
print coreHandle.GetTemplateFolder()

configHandle = Core.ConfigManager.getInstance()
print configHandle.GetValue_s("application.media-folder")
print type(configHandle.GetValue_s("application.media-folder"))

appHandle = Core.ApplicationManager.getInstance()
print 'Running: ' + str(appHandle.GetRunning())
print appHandle.GetWindow().GetMinimumSize()

obj = CL_Size(100, 250)
print obj

obj2 = CL_Size(10, 20)
obj3 = CL_Size(10, 20)
print 'Obj2: ' + str(obj2)
print 'Obj3: ' + str(obj3)
print obj2 == obj3

obj = CL_Sizef(100.3, 250.5)
print obj

obj = CL_Vec2i(10, 40)
obj2 = CL_Vec2i(30, 50)
print obj
print obj2
print obj.Dot(obj2)

angle = CL_Angle.FromDegrees(60)
angle2 = CL_Angle.FromDegrees(61)

print angle == angle2
