CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

VulkanTest: main.cpp triangleApp.hpp triangleApp.cpp vertex.hpp window.hpp window.hpp device.cpp device.hpp
	g++ $(CFLAGS) -o VulkanTest main.cpp triangleApp.cpp device.cpp window.cpp $(LDFLAGS)

.PHONY: test clean

test: VulkanTest
	./VulkanTest

clean:
	rm -f VulkanTest resources/shaders/*.spv
