# 🧠 Dining Philosophers

> A multithreaded simulation of the classic concurrency problem, implemented in C as part of the 1337 curriculum.

📌 Description
---------------
The Dining Philosophers problem illustrates synchronization issues and solutions in concurrent programming.
This project simulates philosophers seated around a table who alternately think, eat, and sleep — using shared forks (resources) — while preventing deadlocks, starvation, and data races.

Each philosopher is represented by a thread, and all shared resource access is protected using POSIX mutexes.

⚙️ Usage
--------

✅ Compilation:
$ make

▶️ Execution:
$ ./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]

📘 Example:
$ ./philo 5 800 200 200

- 5 philosophers
- A philosopher dies if they don't eat within 800ms
- Each philosopher eats for 200ms and sleeps for 200ms

📁 Project Structure
---------------------
philo.c         → Entry point, parsing arguments  
init.c          → Initialization of threads, mutexes, and shared structures  
philosopher.c   → Philosopher routines and state machine  
monitor.c       → Death detection and stop conditions  
utils.c         → Timing helpers and safe output  
Makefile        → Compilation rules

🧵 Features
-----------
- POSIX threads
- Accurate millisecond timing
- Thread-safe printing
- Deadlock prevention
- Optional: stops when all philosophers eat X times

📖 Rules
---------
- Each philosopher needs two forks to eat
- They alternate between eating, sleeping, and thinking
- If one doesn’t eat within time_to_die → they die, and the simulation stops
- Optionally: simulation ends when all eat a specified number of times

🧠 Concepts
------------
- Thread synchronization with mutexes
- Deadlock and starvation handling
- Race condition avoidance
- Accurate sleep and timestamp logic
- Clean thread termination

🚀 Bonus (if implemented)
--------------------------
- Processes instead of threads
- Forks and output control via semaphores
- Inter-process synchronization with shared memory

🛠️ Requirements
----------------
- OS: Unix/Linux
- Language: C
- Libraries: pthread, stdio, unistd, stdlib, sys/time

📚 References
--------------
- https://en.wikipedia.org/wiki/Dining_philosophers_problem
- 1337 / 42 Network curriculum

👨‍💻 Author
-----------
Hamza Rami  
Student at 1337 Khouribga 
