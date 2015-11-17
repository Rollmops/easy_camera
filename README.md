Installation
============

1. Just place the camera_plugin.exe where ever you want (I place mine in the Prepar3d´s Module directory)
2. Add the camera_plugin.exe to your exe.xml; the new entry should look like (of course with an adapted path):

<Launch.Addon>
  <Name>Camera Plugin</Name>
  <Disabled>False</Disabled>
  <Path>E:\Prepar3D\Modules\camera_plugin.exe</Path>
</Launch.Addon>

3. Unitl now you can not modify the keybindings for this plugin. So you have to delete all numpad related keybindings from the Prepar3d´s control setting menu.
4. Start your sim

How does it work?
=================

To move your head you have to use the Shift+(Numpad 0-9 and the Numpad_Delete button). 

Shift+Numpad5:		Center View
Shift+Numpad4:		Move Head Left
Shift+Numpad6:		Move Head Right
Shift+Numpad2:		Move Head Down
Shift+Numpad8:		Move Head Up
Shift+Numpad1:		Move Haead Backward
Shift+Numpad7:		Move Head Forward
Shift+Numpad9:		Look Down
Shift+Numpad3:		Look Up
Shift+Numpad0:		Look Left
Shift+Numpad_Del:	Look Right

(Does anyone need roll?)

If you setup a view position you like you can use Ctrl+(Numpad 1-9) to save it.
To move your head to a saved position just use Numpad 1-9.

The viewing positions are saved per aircraft. So you can define a camera set for each.
The camera positions are saved in a file named "camera_definitions.cfg" located in the same directory as the camera plugin. So you also can share it.