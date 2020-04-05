[![EN](https://user-images.githubusercontent.com/9499881/33184537-7be87e86-d096-11e7-89bb-f3286f752bc6.png)](https://github.com/r57zone/Half-Life-Alyx-novr/blob/master/README.md)
[![RU](https://user-images.githubusercontent.com/9499881/27683795-5b0fbac6-5cd8-11e7-929c-057833e01fb1.png)](https://github.com/r57zone/Half-Life-Alyx-novr/blob/master/README.RU.md) 
# Half Life Alyx no VR driver
Driver allows to play in [Half-Life: Alyx](https://store.steampowered.com/app/546560/HalfLife_Alyx/) without VR on keyboard and mouse. It is not clear yet whether it will be possible to go through the whole game in this way, but if in some place a problem arises, can use saves or other methods to bypass the problem moment in the game.

[![](https://user-images.githubusercontent.com/9499881/78283334-ad9e4e80-752e-11ea-8aea-a2cf18eae511.gif)](https://youtu.be/-8tchjRHSrA) 
[![](https://user-images.githubusercontent.com/9499881/78290591-fce77d80-7534-11ea-9b2f-f80d3b2c176a.gif)](https://youtu.be/-8tchjRHSrA)
[![](https://user-images.githubusercontent.com/9499881/78290158-57340e80-7534-11ea-9b9a-bdc3b558ac17.gif)](https://youtu.be/BkFEMmqxKlU)
[![](https://user-images.githubusercontent.com/9499881/78291122-e7bf1e80-7535-11ea-9a6a-8621f6ddee68.gif)](https://youtu.be/-8tchjRHSrA)

## Setup
Detailed setup process can be seen in [this video](https://youtu.be/66HIE3DFfjo). 



After installing SteamVR and TrueOpenVR, need to go to the SteamVR state window -> Setup room -> Small room -> Calibrate-> 170cm.



Also must add launch options.

For Steam, go to the game’s properties, click on the launch options and enter `+ vr_gg_gesture_pull 0`. For a regular shortcut, go to the shortcut properties and add the same `"C:\Games\Half-Life Alyx\bin\win64\hlvr.exe" +vr_gg_gesture_pull 0`.


After starting the console, must enter the console `vr_gg_gesture_pull 0`. This is necessary so that objects are [pulled up without movement](https://youtu.be/RWQbwlXjtjI).



In the game settings, you need to select "Movement: Smooth" (for smooth walking as in regular games).



**Warning!** Select the "Headset Window" or press "F7" for the driver to work.
## Control
![](https://user-images.githubusercontent.com/9499881/78459634-600b1880-76cb-11ea-9ba8-efb6cff657a2.PNG)

Button | description | Designation in VR
------------ | ------------- | -------------
Up, down, left and right | Going forward, backward, left and right | Touch panel of the left controller, with clicks on it
Left mouse button or Numpad 1 | Shooting or hold right-handed items | Right Controller Trigger
Right mouse button | The movement mode of the left controller also activates hold on the left mouse button | Trigger and movement of the left controller
Middle mouse button | Weapon selection mode | Pressing up the touch panel of the right controller
Space | Teleport | Pressing the bottom of the touch panel of the right controller
Right control | Sit down | Sit down
Right Shift | Hold of items by the left controller and pulling up items (you must use the command above, read the setting) | Left Controller Trigger
Enter | Actions of some items | Left Controller Menu
Escape | Game menu | Left Controller Grip
Numpad 0 | Actions of some items | Right Controller Grip
"?" | Using a first aid kit syringe, use after pressing "Enter" | Rotate and move to the right side of the left controller
"\|" | Using a backpack, use with the Right Shift | Moving the left controller over the left side
"'" | Fixing left controller in place (Part 5: North Star) | fixing left controller in space
Backspace | Health and ammo display | Left controller rotation
";" | Movement of the left controller forward (for throwing objects) | Movement of the left controller forward
PageUP | Move up | Moving controllers and heads up
PageDown | Move down | Move controllers and heads down
"{" | Moving the left controller up, in the movement mode of the left controller (right mouse button) | Move left controller up in height
"}" | Moving the left controller down, in the movement mode of the left controller (right mouse button) | Move the left controller down in height
"U", "J", "H", "K" | Controllers rotation (use only for certain moments necessary in the game - part 5: North Star), reset after use | Rotation of both controllers
"Y" | Reset controllers rotation with the buttons above | Return controllers to state before rotation

## Saves or workarounds for problem areas
Share via ["issues"](https://github.com/r57zone/Half-Life-Alyx-novr/issues) problem areas, methods for solving them and saves. Saves can be placed in ["issues"](https://github.com/r57zone/Half-Life-Alyx-novr/issues) with the prefix "[Save]" Level name and screenshot, also can add label - "Saves".

## Problems
**Red screen** - switch the focus to the game, press the left mouse button and then switch again to the "Headset window". Also can change the "DebugMode" parameter to true, in the "default.vrsettings" configuration file.



Please read [opened](https://github.com/r57zone/Half-Life-Alyx-novr/issues)[closing] and (https://github.com/r57zone/Half-Life-Alyx-novr/issues?q=is%3Aissue+is%3Aclosed) issues and carefully read the description. Perhaps your problem already has a solution.

## Game in 3D 
You can stream game play 3D to smartphone, for example, through the application Moonlight (for Nvidia 600 series +) or Trinus.

## Download
>Version for TrueOpenVR & SteamVR.<br>
**[Download](https://github.com/r57zone/Half-Life-Alyx-novr/releases)**

## Feedback
`r57zone[at]gmail.com`