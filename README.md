# 🐧 epoll 기반 Linux Network Server (WIP)

본 프로젝트는 **Linux 환경에서 epoll 기반 네트워크 서버를 직접 설계·구현 중인 개인 학습 프로젝트**입니다.  
Windows IOCP 모델과 Linux epoll 모델의 차이를 비교·이해하고,  
**epoll 특성에 맞는 이벤트 디스패처와 서버 루프 구조를 단계적으로 구현**하는 것을 목표로 하고 있습니다.

> 🚧 **Work In Progress**  
> 현재 서버 코어 구조 및 epoll 이벤트 분기 로직을 중심으로 설계·구현을 진행 중입니다.

---

## 🎯 프로젝트 목적

- Linux의 **epoll 이벤트 모델 동작 방식**을 내부 구조 수준에서 이해
- IOCP 기반 서버 개념을 **epoll 기반 아키텍처로 재해석**
- 단순 예제가 아닌, **실제 서버 구조에 가까운 설계**를 목표로 단계적 구현
- 이벤트 처리 / 에러 처리 / 종료 처리 흐름을 명확히 분리

---

## 🧠 epoll 기반 서버 설계 개요 (진행 중)

본 서버는 **비동기 I/O 작업을 등록하는 방식(IOCP)** 이 아니라,  
**이벤트 관심사 등록 후 이벤트 발생 시 직접 I/O를 수행하는 epoll 모델**을 기준으로 설계하고 있습니다.

[epoll_ctl]
     ↓
[epoll_wait]
     ↓
[event dispatch]
     ↓
[recv / send]



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


-- 현재는 네트워크 코드 작성을 하고 있는 중이라 실행되지 않음 ;;
