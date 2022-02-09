del /s /q "%CD%\..\Distribution"
"C:\Program Files\Epic Games\UE_4.27\Engine\Build\BatchFiles\RunUAT.bat" BuildPlugin -Plugin="%CD%\..\Plugins\Systems\Systems.uplugin" -Package="%CD%\..\Distribution" -TargetPlatforms=Win64 -VS2019 -Rocket
