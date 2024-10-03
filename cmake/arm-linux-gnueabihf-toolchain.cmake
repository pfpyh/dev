# Target operating system
set(CMAKE_SYSTEM_NAME Linux)

# Target architecture
set(CMAKE_SYSTEM_PROCESSOR arm)

# Use the cross compiler installed via apt
set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)

# Specify the target architecture and float ABI (use hardware floating point)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=armv7-a -mfloat-abi=hard -mfpu=vfpv3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=armv7-a -mfloat-abi=hard -mfpu=vfpv3")