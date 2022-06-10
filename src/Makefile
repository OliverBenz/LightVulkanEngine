CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

VulkanTest: main.cpp triangleApp.hpp triangleApp.cpp vertex.hpp lib/stb/stb_image.h lib/tol/tiny_obj_loader.h
	g++ $(CFLAGS) -o VulkanTest main.cpp triangleApp.cpp $(LDFLAGS)

.PHONY: test clean

test: VulkanTest
	./VulkanTest

clean:
	rm -f VulkanTest resources/shaders/*.spv
