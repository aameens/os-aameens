# Exam 1

## Date: Monday, October 7

### OS
* Virtualization
* Resource Management
* Access Control
* process
    * scheduling
        * Round Robin
        * Shortest Job First
    * kernel hidden at the top of a process
    * fork and exec to manipulate processes/threads
* multi-threading
    * "multiple cooks in the kitchen"
    * each thread has its own stack - tiny piece of the heap
        * threads share everything but their stack
    * threads need some amount of orchestration
        * #include \<pthreads>
        * mutex - manage shared resources
    * shared resource: any thing more than one thread could simultaneously access
    * critical section: sections of code that may simultaneously access/use some shared resource
    * **every shared resource should have 1 mutex**
        * (wait for availibility) -> obtain -> use -> release
* RAM is not linear
    * multiple layers, core -> L1 -> L2 -> L3 -> ram -> swap
    * security in transit / overflow / timing attacks
    * makes your computer much more **efficient**
        * sequential page access (4kb)
        * bus alignment

# Post-Exam
1. fork -> duplicates current process as a child. **exec -> replaces current process with new executable of it**
2. input to program exceeds a size limit that is not checked by the code, so the data corrupts memory nearby (above) the buffer. this changes the behavior of the program and allows for arbitrary code execution, which can be exploited by the attacker
3.  * **address space randomization** to offset the buffer's location, to make it difficult to accurately exploit a buffer overflow
    * **NX bit (prevent code from running on the stack/heap)**
    * **canary values (check for corruption before returning from a stack frame)**
    * ***NOT*** input validation, as it isn't handled by the OS
4. SJF runs process with shortest required time first. This can't be done because you can't know exactly how long a process will take ahead of time - this must be estimated. Can indefinitely delay (starve) slower processes so long as there are shorter jobs available
5. * **Geometric vs Arithmetic averages:**
        * **Geometric is O(1) space and time, just needs last average and next sample to reach next avg**
        * **Arithmetic is O(1) time and O(n) space, needs last n samples, last ang, next sample**
6. **Kernel Requests: You cannot directly call from the code to the Kernel (would be a massive vulnerability), use an ISR (interrupt service request) which enables specific entry points into the Kernel. Specifically a software interrupt can ask for a specific TSR to be called. A TSR is very similar to a subroutine call but -> kernel addresses are allotted (IVT; interrupt vector table)**
7. **Large in-memory database: Sequential records are stored sequentially, so they use the cache optimally. Random can't use the cache very effectively because it's random, making it much slower. Working against the caching system is slow**
8. * **Two main cases where Threads are useful:**
        * **Things that can be done in parallel on multiple cores will be wall-clocked faster (if speed is useful, that's a good reason to use threads)**
        * **When the problem is convenient to divide into multiple (simultaneous) actors. Ex: grammar checker, large print/draw, etc**
9. **Multiple actors have to interact. Messing it up can lead to corrupted data or cause logic errors when data is updated out of order. Can be addressed with properly used Mutex for shared resources,** but can still lead to bottlenecks / deadlocks
10. **deadlock caused when a thread that owns mutex for SR1 waits for the mutex for SR2 which is held by another thread that owns mutex for SR2, which is itself in CS2 with SR2 waiting for the SR1 mutex and CS1 to be available. Can be addressed by releasing a Mutex after a timeout, wait a random set of time and try again**