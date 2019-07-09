#include "led-matrix.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <signal.h>

using rgb_matrix::GPIO;
using rgb_matrix::RGBMatrix;

uint8_t PANEL_HEIGHT = 16;
uint8_t PANEL_WIDTH  = 32;

volatile bool interrupt_received = false;

static void InterruptHandler(int signo) {
	interrupt_received = true;
}

void draw_canvas(RGBMatrix::Canvas* canvas) {
	uint8_t r, g, b;
	uint32_t n, max = 0xFFFFFF;
	while(interrupt_received == false) {
		/*canvas->SetPixel(0, 0, 255, 0, 0);
		canvas->SetPixel(0, 1, 0, 255, 0);
		canvas->SetPixel(0, 2, 0, 0, 255);*/
		/*for (n = 0; n < max; n++) {
			r = ((n >> 16) & 0xFF);
			g = ((n >> 8) & 0xFF);
			b = n & 0xFF;

			if (interrupt_received) break;
			canvas->SetPixel(0, 0, r, g, b);
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		}*/
		canvas->SetPixel(0, 0, 0, 255, 0);
	}
}

int main(int argc, char *argv[]) {
	RGBMatrix::Options defaults;

	defaults.hardware_mapping = "regular";
	defaults.rows = PANEL_HEIGHT;
	defaults.cols = PANEL_WIDTH;
	defaults.chain_length = 1;
	defaults.multiplexing = 3;

	RGBMatrix::Canvas* canvas = rgb_matrix::CreateMatrixFromFlags(&argc, &argv, &defaults);
	if (canvas == NULL) return 1;
	std::cout << "It's connected!" << std::endl;

	signal(SIGTERM, InterruptHandler);
	signal(SIGINT, InterruptHandler);

	draw_canvas(canvas);
	delete canvas;
	return 0;
}
