project(dfrobot)

set(PUBLIC_HEADERS
	"include/Motor.h"
  "include/Smbus.h"
)

add_executable(dfrobot 
    src/Motor.c
)
