@REM del /s /q "%CD%\..\Distribution"

@REM "D:\Applications\Epic Games\UE_4.27\Engine\Build\BatchFiles\RunUAT.bat" BuildPlugin -Plugin="%CD%\..\Plugins\Systems\Systems.uplugin" -Package="%CD%\..\Distribution\Systems-Architecture" -TargetPlatforms=Win64 -VS2019 -Rocket
@REM "D:\Applications\Epic Games\UE_4.27\Engine\Build\BatchFiles\RunUAT.bat" BuildPlugin -Plugin="%CD%\..\Plugins\SystemsQueryDebugger\SystemsQueryDebugger.uplugin" -Package="%CD%\..\Distribution\Systems-Query-Debugger" -TargetPlatforms=Win64 -VS2019 -Rocket
@REM "D:\Applications\Epic Games\UE_4.27\Engine\Build\BatchFiles\RunUAT.bat" BuildPlugin -Plugin="%CD%\..\Plugins\ReactiveUserInterfaceSystems\ReactiveUserInterfaceSystems.uplugin" -Package="%CD%\..\Distribution\Reactive-UI-Systems" -TargetPlatforms=Win64 -VS2019 -Rocket
