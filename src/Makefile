CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

VulkanTest: main.cpp device.o window.o buffer.o swapchain.o application.hpp application.cpp vertex.hpp model.o
	g++ $(CFLAGS) -o VulkanTest main.cpp device.o window.o buffer.o swapchain.o model.o application.cpp $(LDFLAGS)

device.o: device.cpp device.hpp
	g++ $(CFLAGS) -c device.cpp $(LDFLAGS)

window.o: window.cpp window.hpp
	g++ $(CFLAGS) -c window.cpp $(LDFLAGS)

swapchain.o: swapchain.cpp swapchain.hpp device.o window.o
	g++ $(CFLAGS) -c swapchain.cpp device.o window.o $(LDFLAGS)

buffer.o: buffer.cpp buffer.hpp device.o
	g++ $(CFLAGS) -c buffer.cpp device.o $(LDFLAGS)

model.o: model.cpp model.hpp buffer.o device.o
	g++ $(CFLAGS) -c model.cpp buffer.o device.o $(LDFLAGS)

.PHONY: test clean

test: VulkanTest
	./VulkanTest

clean:
	rm -f VulkanTest *.o resources/shaders/*.spv
