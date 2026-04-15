# Qt C++ Multithreading Demo

Покрокова демонстрація багатопоточності на C++/Qt —
від рівня Trainee до Senior+. Кожен рівень будується
на попередньому, додаючи нові концепції та вирішуючи
реальні проблеми попереднього підходу.

## Рівні складності

| Рівень | Підхід | Ключова концепція |
|--------|--------|-------------------|
| Trainee | `QThread` subclass | `run()`, `atomic<bool>` |
| Junior | Worker + `moveToThread` | `QMutex`, `QWaitCondition`, `QAbstractListModel` |
| Middle | `QThreadPool` + `QRunnable` | Thread pool, `QFuture`, `QtConcurrent` |
| Senior | `std::future` + `promise` | `shared_mutex`, RAII, lock-free |
| Senior+ | Custom priority pool | `condition_variable`, cancellation token |

## Стек

- **C++17**
- **Qt 6.x** (Core, Gui, Quick, Concurrent)
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
├── CMakeLists.txt
├── threadcontroller.h/cpp   ← C++/QML bridge (єдина точка входу для UI)
├── worker_thread.h/cpp      ← Trainee: QThread subclass
├── task_queue.h/cpp         ← Junior: mutex + wait condition
├── task_processor.h/cpp     ← Middle: QThreadPool
├── advanced_processor.h/cpp ← Senior: std::future + cache
├── thread_pool.h/cpp        ← Senior+: custom priority pool
└── qml/                     ← весь UI
    ├── main.qml
    ├── TaskCard.qml
    ├── ThreadPoolView.qml
    ├── CacheStats.qml
    └── PriorityQueue.qml
```

## Ліцензія

MIT