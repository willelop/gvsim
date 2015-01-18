#ifndef TYPEDEFS_H
#define TYPEDEFS_H
#include <inttypes.h>


#ifdef WIN32
    //#define max(a,b) (((a) > (b)) ? (a) : (b))
    //#define min(a,b) (((a) < (b)) ? (a) : (b))
    #undef min
    #undef max
    #undef ERROR
#else
    #include <stdlib.h>
    #include <math.h>
#endif

typedef char		int8;
typedef unsigned char	uint8;
typedef int16_t		Sint16;
/*!
 * \brief The LOG struct contains the diferents Log types
 */
struct LOG {
    enum TYPE {
        NORMAL,
        ERRORMSG,
        INFO
    };
};
/*!
 * \brief The inputs struct contains the enums to order the commands
 */
struct inputs {
    enum CONTROLAXES {
        ELEVATORAXE,
        AILERONAXE,
        RUDDERAXE,
        THROTTLEAXE
    };
    enum CONTROLKEYS {
        ELEVATORUP,
        ELEVATORDOWN,
        AILERONLEFT,
        AILERONRIGHT,
        RUDDERLEFT,
        RUDDERRIGHT,
        THROTTLEUP,
        THROTTLEDOWN,
        BREAKS,
        GEARTOGGLE
    };
};

#endif // TYPEDEFS_H
