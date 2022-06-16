CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

VulkanTest: main.cpp application.hpp application.cpp vertex.hpp window.hpp window.hpp swapchain.hpp swapchain.cpp device.cpp device.hpp buffer.cpp buffer.hpp model.cpp model.hpp
	g++ $(CFLAGS) -o VulkanTest main.cpp application.cpp swapchain.cpp device.cpp buffer.cpp model.cpp window.cpp $(LDFLAGS)

.PHONY: test clean

test: VulkanTest
	./VulkanTest

clean:
	rm -f VulkanTest resources/shaders/*.spv
