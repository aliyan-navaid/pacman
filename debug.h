#ifndef DEBUG_H
#define DEBUG_H

#ifdef DEBUG
    #include <iostream>
    #define DEBUG_PRINT(x) std::cout << "[DEBUG] " << x << std::endl
#else
    #define DEBUG_PRINT(x)
#endif

#endif