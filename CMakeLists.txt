project(dfrobot)

set(PUBLIC_HEADERS
	"Motor.h"
  	"Smbus.h"
)

add_executable(dfrobot 
    Motor.c
)
