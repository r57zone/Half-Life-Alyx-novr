# Настройка
## Новый метод
1. Установить [SteamVR](https://store.steampowered.com/app/250820/SteamVR/).
2. Распаковать драйвер ["HalfLifeAlyx.SteamVR.driver.zip"](https://github.com/r57zone/Half-Life-Alyx-novr/releases), в папку "...\Steam\steamapps\common\SteamVR\drivers".
3. Изменить параметры в файле конфигурации "...\Steam\steamapps\common\SteamVR\drivers\hlalyx\resources\settings\default.vrsettings", "renderWidth" ширина, а "renderHeight" высота разрешения игры (например, для игры в 1920 на 1080, нужно выставить "renderWidth" 1920, а "renderHeight" 1080 (для уменьшения FOV можно поставить "ZoomWidth" до 0.8 и "ZoomHeight" до 0.8, так это поможет уменьшить "черные углы"), если игра тормозит, то эти параметры можно сделать меньше, например, 1280 и 720). 
4. Запускаем SteamVR и в окне состояния SteamVR жмем -> Настройка комнаты -> Маленькая комната -> Калибровка -> `170` см. SteamVR tutorial можно закрыть, а SteamVR Home отключается в настройках SteamVR.
5. Для Steam переходим в свойствах игры, там жмем параметры запуска и вводим `+vr_gg_gesture_pull 0`. Для обычного ярлыка заходим в свойства ярлыка и добавляем тоже самое: `"C:\Games\Half-Life Alyx\bin\win64\hlvr.exe" +vr_gg_gesture_pull 0`. Это нужно чтобы предметы [подтягивались по одной кнопке](https://youtu.be/RWQbwlXjtjI). Также если вы используете пиратскую версию игры, то нужно добавить ` -vr -noasserts -nopassiveasserts +map startup`.
6. Запускаем игру, выбираем игру и кликаем по ней, далее жмем ALT + TAB и переключаемся на "Headset Window" и кликаем. Окно состояния SteamVR и сообщения SteamVR можно скрыть.
7. В настройках игры нужно выбрать "Передвижение: Скольжение" (для плавного хождения как в обычных играх) и отключите поворот (чтобы надпись используйте поворот не мешала).
8. (Необязательно) Для игры в режиме зрителя нужно изменить параметр `Stereo` на `true`, а также `DebugMode` на `true`. Создать файл `autoexec.cfg` в папке `\Half-Life Alyx\game\hlvr\cfg\` и добавить в него параметр `vr_companion_window_zoom_level 0.5625` и `vr_spectator_show_left_eye 1` или выставить "renderWidth" и "renderHeight" - `1920` (для высокопроизводительных ПК). Для отключения курсора используем утилиту "HideCursor" (в архиве с драйвером). Также придется загружать игру в "Headset Window". Про включение debugmode для исправления режима камеры и зум подсказал tooulg.

Также изучите описания кнопок, если не удается пройти какой-то момент читайте инструкции и смотрите руководства прохождения на [youtube](https://www.youtube.com/channel/UCcuoRRWRvb7xUuMzrEqCZ5w).


Если у вас запускается SteamVR Home, то войдите в настройки SteamVR и отключите её автозапуск при старте SteamVR.



Сначала запускайте SteamVR, а только потом игру.

## Старый метод: TrueOpenVR (не рекомендуется, устарел)
1. Распаковать [TrueOpenVR](https://github.com/TrueOpenVR/TrueOpenVR-Core/releases) в папку "C:\Program File\TrueOpenVR"
2. Распаковать драйвер ["HalfLifeAlyx64.dll"](https://github.com/r57zone/Half-Life-Alyx-novr/releases) в папку "C:\Program File\TrueOpenVR\Settings\Drivers".
3. Запустить "TrueOpenVR Settings.exe" (C:\Program File\TrueOpenVR\Setttings), выбрать там "HalfLifeAlyx64.dll" и нажать применить.
4. Установить [SteamVR](https://store.steampowered.com/app/250820/SteamVR/).
5. Распаковать драйвер ["TrueOpenVR.SteamVR.manual.zip"](https://github.com/TrueOpenVR/SteamVR-TrueOpenVR/releases) для SteamVR (мост драйвер), в папку "...\Steam\steamapps\common\SteamVR\drivers".
6. Изменить параметры в файле конфигурации "...\Steam\steamapps\common\SteamVR\drivers\tovr\resources\settings\default.vrsettings", "renderWidth" ширина, а "renderHeight" высота разрешения игры (например, для игры в 1920 на 1080, нужно выставить "renderWidth" 1920, а "renderHeight" 1080 (для уменьшения FOV можно поставить 1280 или увеличив "ZoomWidth" до 0.8 и "ZoomHeight" до 0.8, так это поможет уменьшить "черные углы"), если игра тормозит, то эти параметры можно сделать меньше, например, 1280 и 720). Также "windowWidth" это ширина окна, а "windowHeight" высота, эти параметры должны строго соответствовать разрешению вашего монитора (смотрим разрешение в настройках). Также необходимо изменить параметр "Stereo" с true на false. Если вы хотите играть в 3D на мобильном VR шлеме из смартфона, то можно оставить true, но renderWidth придется изменить обратно на 1080. Для PSVR шлемов и других HDMI шлемов, работающих в режиме расширенного экрана, можно изменить параметр "windowX" на 1920 (ширина разрешения первого монитора) и окно будет выводится на второй монитор.
7. Запускам SteamVR и в окне состояния SteamVR жмем -> Настройка комнаты -> Маленькая комната -> Калибровка -> 170 см. SteamVR tutorial можно закрыть, а SteamVR Home отключается в настройках SteamVR.
8. Для Steam переходим в свойствах игры, там жмем параметры запуска и вводим `+vr_gg_gesture_pull 0`. Для обычного ярлыка заходим в свойства ярлыка и добавляем тоже самое: `"C:\Games\Half-Life Alyx\bin\win64\hlvr.exe" +vr_gg_gesture_pull 0`. Это нужно чтобы предметы [подтягивались по одной кнопке](https://youtu.be/RWQbwlXjtjI).
9. Запускаем игру, выбираем игру и кликаем по ней, далее жмем ALT + TAB и переключаемся на "Headset Window" и кликаем. Окно состояния SteamVR и сообщения SteamVR можно скрыть.
10. В настройках играх выбрать "Передвижение: Скольжение" (для плавного хождения как в обычных играх) и отключите поворот (чтобы надпись используйте поворот не мешала).
11. (Необязательно) Для игры в режиме зрителя нужно увеличить FOV в конфигурационном файле "default.vrsettings" на 110. Вернуть обратно параметр "Stereo" на true. Выставить "renderWidth" и "renderHeight" - 1920. Для отключения курсора используем утилиту "HideCursor" (в архиве с драйвером). Также придется загружать игру в "Headset Window".


