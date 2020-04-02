[![EN](https://user-images.githubusercontent.com/9499881/33184537-7be87e86-d096-11e7-89bb-f3286f752bc6.png)](https://github.com/r57zone/Half-Life-Alyx-novr/blob/master/README.md)
[![RU](https://user-images.githubusercontent.com/9499881/27683795-5b0fbac6-5cd8-11e7-929c-057833e01fb1.png)](https://github.com/r57zone/Half-Life-Alyx-novr/blob/master/README.RU.md) 
# Half Life Alyx No vr driver
Driver allows to play in [Half-Life: Alyx](https://store.steampowered.com/app/546560/HalfLife_Alyx/) without VR on keyboard and mouse. It is not clear yet whether it will be possible to go through the whole game in this way, but if in some place a problem arises, can use saves or other methods to bypass the problem moment in the game.

[![](https://user-images.githubusercontent.com/9499881/78283334-ad9e4e80-752e-11ea-8aea-a2cf18eae511.gif)](https://youtu.be/-8tchjRHSrA) 
[![](https://user-images.githubusercontent.com/9499881/78290591-fce77d80-7534-11ea-9b2f-f80d3b2c176a.gif)](https://youtu.be/-8tchjRHSrA)
[![](https://user-images.githubusercontent.com/9499881/78290158-57340e80-7534-11ea-9b9a-bdc3b558ac17.gif)](https://youtu.be/BkFEMmqxKlU)
[![](https://user-images.githubusercontent.com/9499881/78291122-e7bf1e80-7535-11ea-9a6a-8621f6ddee68.gif)](https://youtu.be/-8tchjRHSrA)

## Setup
Detailed setup process can be seen in [this video](https://youtu.be/66HIE3DFfjo). 

Also must add launch options.

`-allowdebug -vconport 29000 -vcon -console`

for the game shortcut in the game properties. After adding, the launch path will be something like this

`"C:\Games\Half-Life Alyx\bin\win64\hlvr.exe" -allowdebug -vconport 29000 -vcon -console"`.


After starting the console, must enter the console `vr_gg_gesture_pull 0`. This is necessary so that objects are pulled up without movement.

## Управление
![](https://user-images.githubusercontent.com/9499881/78291469-7fbd0800-7536-11ea-84fe-b9f5166613d8.PNG)

Button | description | Designation in VR
------------ | ------------- | -------------
Up, down, left and right | Going forward, backward, left and right | Touch panel of the left controller, with clicks on it
Left mouse button or Numpad 1 | Shooting or hold right-handed items | Right Controller Trigger
Right mouse button | The movement mode of the left controller also activates hold on the left mouse button | Trigger and movement of the left controller
Middle mouse button | Weapon selection mode | Pressing up the touch panel of the right controller
Space | Teleport | Pressing the bottom of the touch panel of the right controller
Right control | Sit down | Sit down
Right Shift | Hold of items by the left controller | Left Controller Trigger
Enter | Actions of some items | Left Controller Menu
Tab | Game menu | Left Controller Grip
Numpad 0 | Actions of some items | Right Controller Grip
"?" | Using a first aid kit syringe, use after pressing "Enter" | Rotate and move to the right side of the left controller
"\|" | Using a backpack, use with the Right Shift | Moving the left controller over the left side
PageUP | Move up | Moving controllers and heads up
PageDown | Move down | Move controllers and heads down

## Saves or workarounds for problem areas
Share via ["Issue"](https://github.com/r57zone/Half-Life-Alyx-novr/issues) problem areas, methods for solving them and saves.

## Download
>Version for TrueOpenVR & SteamVR.<br>
**[Download](https://github.com/r57zone/Half-Life-Alyx-novr/releases)**

## Feedback
`r57zone[at]gmail.com`