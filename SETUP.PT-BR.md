# Instalação
## Novo método
1. Instale [SteamVR] (https://store.steampowered.com/app/250820/SteamVR/).
2. Descompacte o driver ["HalfLifeAlyx.SteamVR.driver.zip"](https://github.com/r57zone/Half-Life-Alyx-novr/releases), na pasta "...\Steam\steamapps\common\SteamVR\drivers" .
3. Altere as configurações no arquivo de configuração "...\Steam\steamapps\common\SteamVR\drivers\hlalyx\resources\settings\default.vrsettings", "renderWidth" largura, e "renderHeight" a altura da resolução do jogo (por exemplo, para um jogo em 1920 por 1080, você precisa definir "renderWidth" para 1920, e "renderHeight" para 1080 (para reduzir o FOV, você pode definir "ZoomWidth" para 0.8 e "ZoomHeight" para 0.8, isso ajudará a reduzir as bordas pretas), se o jogo ficar lento, esses parâmetros podem ser reduzidos, por exemplo, 1280 e 720).
4. Inicio o SteamVR e na janela de status clique em -> Preparação de cômodo -> Apenas em pé -> Calibração -> `170` cm. Você pode fechar o tutorial do SteamVR, e a página inicial do SteamVR é desabilitada nas configurações do SteamVR.
5. Para Steam, acesse as propriedades do jogo, clique nas opções de inicialização e digite `+ vr_gg_gesture_pull 0`. Para um atalho, vá para as propriedades do atalho e adicione o mesmo: `"C:\Games\Half-Life Alyx\bin\win64\hlvr.exe " +vr_gg_gesture_pull 0`. Isso é necessário para que os objetos [sejam puxados por uma tecla](https://youtu.be/RWQbwlXjtjI). Além disso, se você estiver usando uma versão pirata do jogo, precisará adicionar ` -vr -noasserts -nopassiveasserts +map startup`.
6. Execute o jogo, então pressione ALT + TAB e mude para a "Headset Window". A janela de status do SteamVR e as mensagens do SteamVR podem ser ocultadas.
7. Nas configurações do jogo, selecione "Locomoção: contínua" (para uma caminhada suave) e desligue a rotação (para que a inscrição não interfira na rotação).
8. (Opcional) Para jogar no modo espectador, você precisa alterar o parâmetro "Stereo" para true. Defina "renderWidth" e "renderHeight" - `1920` (para PCs de alto desempenho) ou crie um arquivo `autoexec.cfg` na pasta `\Half-Life Alyx\game\hlvr\cfg\` e adicione o parâmetro `vr_companion_window_zoom_level 0.55` a ele. Para desabilitar o cursor, use o "HideCursor" (no arquivo do driver). Você também terá que carregar o jogo na "Headset Window".

Estude também as descrições das teclas, se você não conseguir passar em algum momento, leia as instruções e consulte os manuais [youtube](https://www.youtube.com/channel/UCcuoRRWRvb7xUuMzrEqCZ5w).

Se você estiver executando o SteamVR Home, vá até as configurações do SteamVR e desative sua inicialização automática quando o SteamVR iniciar.

Inicie o SteamVR primeiro e só então o jogo.

## Método antigo: TrueOpenVR (não recomendado, desatualizado)
1. Descompacte [TrueOpenVR](https://github.com/TrueOpenVR/TrueOpenVR-Core/releases) dentro da pasta "C:\Program File\TrueOpenVR"
2. Descompacte o driver ["HalfLifeAlyx64.dll"](https://github.com/r57zone/Half-Life-Alyx-novr/releases) na pasta "C:\Program File\TrueOpenVR\Settings\Drivers".
3. Execute "TrueOpenVR Settings.exe"(C:\Program File\TrueOpenVR\Settings), selecione "HalfLifeAlyx64.dll" e clique em aplicar.
4. Instale [SteamVR](https://store.steampowered.com/app/250820/SteamVR/).
5. Descompacte o driver ["TrueOpenVR.SteamVR.manual.zip"](https://github.com/TrueOpenVR/SteamVR-TrueOpenVR/releases) para SteamVR (bridge driver), na pasta "...\Steam\steamapps\common\SteamVR\drivers".
6. Altere as configurações no arquivo de configuração "...\Steam\steamapps\common\SteamVR\drivers\hlalyx\resources\settings\default.vrsettings", "renderWidth" largura, e "renderHeight" a altura da resolução do jogo (por exemplo, para um jogo em 1920 por 1080, você precisa definir "renderWidth" 1920 e "renderHeight" 1080 (1280 pode ser usado para reduzir o FOV ou aumentando "ZoomWidth" para 0.8 e "ZoomHeight" para 0.8, isso também vai reduzir as bordas pretas), se o jogo ficar lento, esses parâmetros podem ser reduzidos, por exemplo, 1280 e 720). Também, "windowWidth" é a largura da janela, e "windowHeight" é a altura, esses parâmetros devem corresponder estritamente à resolução do seu monitor (veja a resolução nas configurações). Você também deve mudar o parâmetro "Stereo" de true para false. Se você quiser jogar em 3D com um óculos VR de smartphone, pode deixar verdadeiro, mas renderWidth terá que ser alterado de volta para 1080. Para óculos PSVR e outros óculos HDMI funcionam no modo de tela estendida, você pode alterar o parâmetro "windowX" para 1920 (resolução da largura do primeiro monitor) e a janela será exibida no segundo monitor.
7. Inicie SteamVR e na janela de status do SteamVR, clique em -> Preparação de cômodo -> Apenas em pé -> Calibração -> 170 cm. Você pode fechar o tutorial do SteamVR, e a página inicial do SteamVR é desabilitada nas configurações do SteamVR.
8. Para Steam, acesse as propriedades do jogo, clique nas opções de inicialização e digite `+ vr_gg_gesture_pull 0`. Para um atalho, vá para as propriedades do atalho e adicione o mesmo: `"C:\Games\Half-Life Alyx\bin\win64\hlvr.exe " +vr_gg_gesture_pull 0`. Isso é necessário para que os objetos [sejam puxados por uma tecla](https://youtu.be/RWQbwlXjtjI).
9. Execute o jogo, então pressione ALT + TAB e mude para a "Headset Window". A janela de status do SteamVR e as mensagens do SteamVR podem ser ocultadas.
10. Nas configurações do jogo, selecione "Locomoção: contínua" (para uma caminhada suave) e desligue a rotação (para que a inscrição não interfira na rotação).
11. (Opcional) Para jogar no modo espectador, você precisa alterar o parâmetro "Stereo" para true. Defina "renderWidth" e "renderHeight" - 1920. Para desabilitar o cursor, use o "HideCursor" (no arquivo do driver). Você também terá que carregar o jogo na "Headset Window".

