# Qt C++ Multithreading Demo

Покрокова демонстрація багатопоточності на C++/Qt —
від рівня Trainee до Senior+. Кожен рівень будується
на попередньому, додаючи нові концепції та вирішуючи
реальні проблеми попереднього підходу.

## Рівні складності

| Рівень | Підхід | Ключова концепція | Готовність |
|--------|--------|-------------------|------------|
| Trainee | `QThread` subclass | `run()`, `atomic<bool>` |✅|
| Junior | Worker + `moveToThread` | `QMutex`, `QWaitCondition`, `QAbstractListModel` |✅|
| Middle | `QThreadPool` + `QRunnable` | Thread pool, `QFuture`, `QtConcurrent` |☐|
| Senior | `std::future` + `promise` | `shared_mutex`, RAII, lock-free |☐|
| Senior+ | Custom priority pool | `condition_variable`, cancellation token |☐|

## Стек

- **C++17**
- **Qt 6.x** (Core, Gui, Quick, Concurrent, Qml)
- **QML** для UI
- **CMake 3.16+**

## Збірка

```bash

# Залежності (Ubuntu/Debian)
sudo apt install qt6-base-dev qt6-declarative-dev cmake

# macOS
brew install qt6 cmake

# Збірка
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=$(brew --prefix qt6)  # тільки macOS
cmake --build . -j$(nproc)
./threading_demo

```

## Структура проєкту

```
├── test.txt                     ← файл з рандомними даними для тестових цілей
├── CMakeLists.txt
├── controller/
│   ├── CMakeLists.txt
│   └── threadcontroller.h/cpp   ← C++/QML bridge (єдина точка входу для UI)
├── worker/   
│   ├── CMakeLists.txt
│   └── worker_thread.h/cpp      ← Trainee: QThread subclassc
├── task_queue/
│   ├── CMakeLists.txt
│   └── task_queue.h/cpp         ← Junior: mutex + wait condition
├── task_processor/
│   ├── CMakeLists.txt
│   └── task_processor.h/cpp     ← Middle: QThreadPool (TBD)
├── advanced_processor/
│   ├── CMakeLists.txt
│   └── advanced_processor.h/cpp ← Senior: std::future + cache (TBD)
├── thread_pool/
│   ├── CMakeLists.txt
│   └── thread_pool.h/cpp        ← Senior+: custom priority pool (TBD)
└── ui/                          ← весь UI
    ├── Main.qml
    ├── TaskCard.qml
    ├── ThreadPoolView.qml (TBD)
    ├── CacheStats.qml (TBD)
    └── PriorityQueue.qml (TBD)
```

## Ліцензія

MIT
