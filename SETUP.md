# Setup
## New method
1. Install [SteamVR](https://store.steampowered.com/app/250820/SteamVR/).
2. Unzip the driver ["HalfLifeAlyx.SteamVR.driver.zip"](https://github.com/r57zone/Half-Life-Alyx-novr/releases), in the folder "...\Steam\steamapps\common\SteamVR\drivers" .
3. Change the settings in the configuration file "...\Steam\steamapps\common\SteamVR\drivers\hlalyx\resources\settings\default.vrsettings", "renderWidth" width, and "renderHeight" the height of the game’s resolution (for example, for a game in 1920 by 1080, you need to set "renderWidth" 1920, and "renderHeight" 1080 (to reduce FOV, you can set "ZoomWidth" to 0.8 and "ZoomHeight" to 0.8, so this will help reduce the "black corners"), if the game slows down, then these parameters can be made smaller, for example, 1280 and 720).
4. Start SteamVR and in the SteamVR status window, click -> Room settings -> Small room -> Calibration -> `170` cm. You can close the SteamVR tutorial, and SteamVR Home is disabled in the SteamVR settings.
5. For Steam, go to the games properties, click on the launch options and enter `+vr_gg_gesture_pull 0`. For a regular shortcut, go to the shortcut properties and add the same: `"C:\Games\Half-Life Alyx\bin\win64\hlvr.exe " +vr_gg_gesture_pull 0`. This is necessary so that objects [are pulled up by one button](https://youtu.be/RWQbwlXjtjI). Also, if you are using a pirated version of the game, then you need to add ` -vr -noasserts -nopassiveasserts +map startup`.
6. Run the game, select the game and click on it, then press ALT + TAB and switch to the "Headset Window" and click. The SteamVR status window and SteamVR messages can be hidden.
7. In the game settings you need to select "Movement: Sliding" (for a smooth walk like in normal games) and turn off the rotation (so that the inscription does not interfere with the rotation).
8. (Optional) To play in spectator mode, you need to change the `Stereo` parameter to `true`, as well as `DebugMode` to `true`. Create a file `autoexec.cfg` in the folder `\Half-Life Alyx\game\hlvr\cfg\` and add the parameter `vr_companion_window_zoom_level 0.55` & `vr_spectator_show_left_eye 1` to it or Set "renderWidth" and "renderHeight" to `1920` (for high performance PCs). To disable the cursor, use the "HideCursor" utility (in the archive with the driver). You will also have to load the game in the "Headset Window". In the latest versions of the game, for some reason the camera does not rotate after the hands, maybe this can be fixed with commands, if someone knows how to fix it, write to me and I will add it to the instructions.

Also study the descriptions of the buttons, if you cannot pass at some point, read the instructions and see the walkthrough manuals on [youtube](https://www.youtube.com/channel/UCcuoRRWRvb7xUuMzrEqCZ5w).


If you are running SteamVR Home, then go into SteamVR settings and disable its autostart when SteamVR starts.



Start SteamVR first and only then the game.

## Old method: TrueOpenVR (not recommended, outdated)
1. Unzip [TrueOpenVR](https://github.com/TrueOpenVR/TrueOpenVR-Core/releases) into the folder "C:\Program File\TrueOpenVR"
2. Unzip the driver ["HalfLifeAlyx64.dll"](https://github.com/r57zone/Half-Life-Alyx-novr/releases) into the folder "C:\Program File\TrueOpenVR\Settings\Drivers".
3. Run "TrueOpenVR Settings.exe"(C:\Program File\TrueOpenVR\Settings), select"HalfLifeAlyx64.dll"there and click apply.
4. Install [SteamVR](https://store.steampowered.com/app/250820/SteamVR/).
5. Unzip the driver ["TrueOpenVR.SteamVR.manual.zip"](https://github.com/TrueOpenVR/SteamVR-TrueOpenVR/releases) for SteamVR (bridge driver), in the folder "...\Steam\steamapps\common\SteamVR\drivers".
6. Change the settings in the configuration file "...\Steam\steamapps\common\SteamVR\drivers\tovr\resources\settings\default.vrsettings", "renderWidth" width, and "renderHeight" the height of the game resolution (for example, for a game in 1920 by 1080, you need to set "renderWidth" 1920 and "renderHeight" 1080 (1280 can be used to reduce FOV or increasing "ZoomWidth" to 0.8 and "ZoomHeight" to 0.8, also this will reduce "black corners"), if the game slows down, then these parameters can be made smaller, for example, 1280 and 720). Also, "windowWidth" is the width of the window, and "windowHeight" is the height, these parameters must strictly correspond to the resolution of your monitor (see the resolution in the settings). You must also change the "Stereo" parameter from true to false. If you want to play 3D on a mobile VR headset from a smartphone, you can leave true, but renderWidth will have to be changed back to 1080. For PSVR helmets and other HDMI helmets working in the extended screen mode, you can change the "windowX" parameter to 1920 (resolution width the first monitor) and the window will be displayed on the second monitor.
7. Start SteamVR and in the SteamVR status window, click -> Room Settings -> Small Room -> Calibration -> 170 cm. The SteamVR tutorial can be closed, and SteamVR Home is disabled in the SteamVR settings.
8. For Steam, go to the game properties, click on the launch options and enter `+vr_gg_gesture_pull 0`. For a regular shortcut, go to the shortcut properties and add the same: `"C:\Games\Half-Life Alyx\bin\win64\hlvr.exe" +vr_gg_gesture_pull 0`. This is necessary so that the objects [pulled up on one button](https://youtu.be/RWQbwlXjtjI).
9. Run the game, select the game and click on it, then press ALT + TAB and switch to the "Headset Window" and click. The SteamVR status window and SteamVR messages can be hidden.
10. In the game settings select "Movement: Sliding" (for smooth walking as in regular games) and turn off the rotation (so that the inscription uses the rotation does not interfere).
11. (Optional) To play in spectator mode, you need to increase the FOV in the configuration file "default.vrsettings" by 110. Return the "Stereo" parameter to true. Set "renderWidth" and "renderHeight" - 1920. To disable the cursor, use the "HideCursor" utility (in the archive with the driver). You will also have to load the game into the "Headset Window".


