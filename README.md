# Philosophers Project

![Philosophers](/philo.jpg)

## Description

The **Philosophers** project is part of **42 School's curriculum**, designed to introduce students to the fundamentals of **multithreading, mutexes, and deadlock prevention** in C. It is based on the **dining philosophers problem**, a well-known **synchronization challenge** that involves a group of philosophers alternating between **thinking, eating, and sleeping**, while ensuring shared resources (forks) are accessed in a controlled manner.

The project requires implementing an **efficient solution** that prevents **deadlocks** and **race conditions** while ensuring philosophers do not starve. Using **pthreads, mutexes, and precise timing**, the program simulates philosophers' interactions while adhering to strict **synchronization and fairness** rules.

---

## Features

- Implements the **Dining Philosophers** problem using a **multithreaded approach**.
- Uses **mutexes** to ensure safe access to shared resources (forks).
- Prevents **deadlocks** and **race conditions** by managing execution order.
- Handles **precise timing** to regulate philosophers' eating and sleeping cycles.
- Supports **command-line parameters** to adjust execution settings dynamically.
- Ensures **efficient resource management** and avoids **memory leaks**.
- Provides **logging and debugging support** for easy issue identification.

---

## Usage

### Compilation

To compile the program, use the **make** command:

```sh
make
```

This generates an executable named **philo**.

### Running the Program

The program accepts **command-line arguments** for configuration:

```sh
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

#### Arguments:

- `number_of_philosophers`: Specifies the number of philosophers at the table.
- `time_to_die`: Maximum time (in milliseconds) a philosopher can go without eating.
- `time_to_eat`: Time (in milliseconds) required for a philosopher to eat.
- `time_to_sleep`: Time (in milliseconds) a philosopher spends sleeping before thinking.
- `[number_of_times_each_philosopher_must_eat]` *(optional)*: If provided, the simulation stops after each philosopher has eaten this many times.

### Example

```sh
./philo 5 800 200 200
```

This runs the simulation with **5 philosophers**, where:

- They will **die if they do not eat within 800ms**.
- Eating takes **200ms**.
- Sleeping lasts **200ms**.

---

## Implementation Details

### Thread-based Simulation
Each philosopher is represented as a **thread**, managing their own execution cycle.

### Mutex Synchronization
Ensures safe **access to shared resources (forks)** and prevents conflicts.

### Precise Timing Control
Uses **system time functions** to regulate **thinking, eating, and sleeping cycles**.

### Deadlock Avoidance
Implements **strategies to prevent starvation** and **cyclic waiting**.

### Efficient Resource Management
Ensures **threads exit cleanly** without memory leaks.

---

## How It Works

Each philosopher follows this cycle:

1. **Thinks** for a while.
2. **Attempts to pick up forks** (mutex-protected resources).
3. **Eats** when both forks are acquired.
4. **Sleeps** for a designated period before repeating the cycle.

---

## Edge Cases Handled

- **Single Philosopher Scenario**: A single philosopher cannot eat and will starve.
- **Minimum Timing Constraints**: Ensures the timing values are valid.
- **Rapid Execution**: Properly handles cases where actions complete too quickly.
- **Large Number of Philosophers**: Efficiently manages **many concurrent threads**.

---

## Learning Outcomes

- Understanding **multithreading** concepts and **thread synchronization**.
- Gaining hands-on experience with **mutexes** and their role in preventing **deadlocks**.
- Learning to **efficiently manage shared resources** in concurrent systems.
- Developing a **robust approach** to handling **race conditions** and **resource contention**.
- Applying **system timing functions** to regulate **concurrent execution**.

---

## Error Handling

- **Invalid Arguments**: Displays an **error message** if input parameters are incorrect.
- **Thread Management Failures**: Ensures proper **error detection** for thread creation.
- **Mutex Failures**: Detects and handles **errors related to mutex initialization**.
- **Memory Leaks**: Ensures **allocated resources** are freed appropriately.


---

### ⭐ If you found this project helpful, consider giving it a star on GitHub!
