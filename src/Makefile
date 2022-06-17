CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

VulkanTest: main.cpp device.o window.o buffer.o swapchain.o application.hpp application.cpp vertex.hpp model.o pipeline.o renderer.o
	g++ $(CFLAGS) -o VulkanTest main.cpp device.o window.o buffer.o swapchain.o model.o pipeline.o renderer.o application.cpp $(LDFLAGS)

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

pipeline.o: pipeline.cpp pipeline.hpp device.o
	g++ $(CFLAGS) -c pipeline.cpp device.o $(LDFLAGS)

renderer.o: renderer.cpp renderer.hpp device.o swapchain.o pipeline.o model.o
	g++ $(CFLAGS) -c renderer.cpp device.o swapchain.o pipeline.o model.o $(LDFLAGS)


.PHONY: test clean

test: VulkanTest
	./VulkanTest

clean:
	rm -f VulkanTest *.o resources/shaders/*.spv
