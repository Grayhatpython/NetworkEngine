# Build (Ubuntu)

```bash
sudo apt update
sudo apt install -y build-essential cmake ninja-build gdb

### Optional: Monitoring Server Dependencies

> The following packages are required **only for the monitoring server**
> that uses ImGui with OpenGL.
> They are **not required for server and client deployments**.

```bash
sudo apt install -y libgl1-mesa-dev
sudo apt install -y libx11-dev libxrandr-dev libxi-dev libxinerama-dev libxcursor-dev


cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build


./build/Server/Server
./build/DummyClient/DummyClient
./build/MonitoringServer/MonitoringServer

