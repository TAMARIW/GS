# desktop

This file contains the instructions to operate the Tamariw satellites using the groundstation on the desktop setup. Please ensure you are familiar with all the steps documented here. If this is your first time or you have any confusion, please contact Atheel Redah or Saurav Paudel.

## Tamariw Satellite

If you are not well acquainted with the satellite hardware, please clarify all your questions with Saurav before proceeding. The following are general steps to keep in mind:

1. Ensure that the power supply is set to **7.2 V** before plugging the satellite into the power supply.
2. After connecting the satellite, a single unit should draw approximately 0.2–0.4 Amps. If it draws more current, turn off the power supply and consult Saurav.
3. If you are unsure whether the current firmware on the satellite is the latest, go to `C:\Users\Floatsat\Desktop\tamariw\Docking-STM` using the Command Prompt or the terminal in VS Code and run the command `git pull origin main` to update the software on the desktop from [the GitHub repository](https://github.com/tamariw/docking-stm). There are two ways to flash code into the STM32 of the Tamariw board:
   - Connect the ST-Link of the Discovery Board to the programming port of the satellite and run the command `make flash` to program the board.
   - Select the `.hex` file from the `CONNECT` tab of the ground station. The file path is `C:\Users\Floatsat\Desktop\tamariw\Docking-STM\build\main.hex`. After selecting the file, press `FLASH`.

## Raspberry Pi

1. Turn on the hotspot from the desktop. The default credentials are:
```
SSID: TAMARIWTestPi
Password: pi@tamariw
```
2. You should be able to see the device connected to the hotspot and its IP address by pressing the `Win` key, typing `Mobile hotspot`, and opening it. This IP address is used by the ground station to connect to the Tamariw satellite.

## Ground Station

1. Open Qt Creator.
2. Go to `File` → `Open File or Project`, and double-click the following file: `C:\Users\Floatsat\Desktop\tamariw\GS\CMakeLists.txt`.
3. Ensure the ground station code is up to date with the latest stable version on [GitHub](https://github.com/tamariw/gs). Open the `Terminal` in Qt Creator and run the command `git pull origin main` to update the code.
4. Press `Ctrl+R` to build the project. The ground station should launch.
5. Enter the IP address of the Tamariw satellite in the `SERVER IP` field under the `CONNECT` tab of the ground station and press the router icon underneath to connect. If the connection is successful, the WiFi icon (greyed out by default) should turn green.
6. Enjoy controlling Tamariw!
