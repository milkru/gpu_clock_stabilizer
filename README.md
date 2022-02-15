## About
A simple *DirectX 12* based *GPU clock stabilizer* used for consistent *GPU* profiling on *Windows*. It can be used for profiling applications made with other graphics APIs as well. It utilizes *DirectX 12's* `SetStablePowerState` in order to achieve this.

## Installation
The *DirectX 12 SDK* comes included with the *Windows 10 SDK* which is part of the *Visual Studio* installation. Just make sure you install the *Game Development with C++* in the *Visual Studio Installer*.

## How it works
If not specified in the command line, the first available *GPU* will be chosen. You can also specifiy the GPU explicitly by passing the name of the preferred GPU, for example "*NVIDIA GeForce RTX 3070*". Names of your *GPUs* can be found in *Device Manager*, *Display Adapters* tab. This way the application can work on *multi GPU system*. In order for *GPU clock* to remain stable, the application should remain running at all times. After exiting the application, clock stabilization will stop working. For testing the *GPU clock* I recommend using the [TechPowerUp GPU-Z](https://www.techpowerup.com/download/techpowerup-gpu-z/). Observe the *Sensors/GPU Core Clock's* behavior with and without the application running.

## Troubleshooting
If `SetStablePowerState` is failing, please refer to the [SetStablePowerState Microsoft Documentation](https://docs.microsoft.com/en-us/windows/win32/api/d3d12/nf-d3d12-id3d12device-setstablepowerstate) for troubleshooting.
