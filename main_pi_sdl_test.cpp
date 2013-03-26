//
//	main_pi_sdl_test.cpp - OpenCV & SDL sample program for Raspberry Pi
//
//	How to Build
//		$ sudo apt-get install libopencv-dev
//		$ sudo apt-get install libsdl1.2-dev
//		$ sudo apt-get install cmake
//		$ cmake .
//		$ make
//

#include <SDL/SDL.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <sys/time.h>

const int WINDOW_WIDTH  = 656;
const int WINDOW_HEIGHT = 416;

SDL_Surface* screen = NULL;
SDL_Surface* result_img_surface = NULL;

cv::VideoCapture capture;
cv::Mat capture_img;
cv::Mat result_img;

bool quit_flag = false;

bool init_opencv()
{
	if (!capture.open(0)) {
		std::cerr << "error: capture.open() failed..." << std::endl;
		exit(-1);
	}
	capture.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

	result_img.create(cv::Size(640, 480), CV_8UC3);

	return true;
}

void finish_opencv()
{
	capture.release();
	capture_img.release();
}

void process_capture()
{
	capture >> capture_img;
}

inline double get_time()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	double t = tv.tv_sec + 1e-6 * tv.tv_usec;
	return t;
}

void process_image()
{
	static int count = 0;
	static double st = get_time();

	// dummy
	cv::resize(capture_img, result_img, result_img.size());

	count ++;
	if (count == 100) {
		double t = (get_time() - st) / 100;
		double fps = 1.0 / t;
		std::cout << "fps=" << fps << ", t=" << t << std::endl;
		st = get_time();
		count = 0;
	}
}

bool init_sdl()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr <<"error: SDL_Init() failed..." << std::endl;
		return false;
	}
	SDL_ShowCursor(SDL_DISABLE);

	screen = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 24, SDL_FULLSCREEN|SDL_DOUBLEBUF|SDL_HWSURFACE);
	if(screen == NULL) {
		std::cerr << "error: SDL_SetVideoMode() failed..." << std::endl;
		exit(EXIT_FAILURE);
	}

	result_img_surface = SDL_CreateRGBSurfaceFrom((void*)result_img.data,
			result_img.cols,
			result_img.rows,
			24,
			result_img.cols * 3,
			0xff0000, 0x00ff00, 0x0000ff, 0);

	if (result_img_surface == NULL) {
		std::cerr << "error: SDL_CreateRGBSurfaceFrom() failed..." << std::endl;
	}

	return true;
}

void finish_sdl()
{
	SDL_FreeSurface(result_img_surface);
	SDL_Quit();
}

void draw()
{
	SDL_Rect src_rect, dst_rect;

	src_rect.x = 0;
	src_rect.y = 0;
	src_rect.w = result_img_surface->w;
	src_rect.h = result_img_surface->h;

	dst_rect.x = 0;
	dst_rect.y = 0;
	dst_rect.w = result_img_surface->w;
	dst_rect.h = result_img_surface->h;

	SDL_BlitSurface(result_img_surface, &src_rect, screen, &dst_rect);

	SDL_Flip(screen);
}

void process_event()
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				quit_flag = true;
				break;
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym) {
					case SDLK_ESCAPE:
						quit_flag = true;
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}
	}
}

int main(int argc, char **argv)
{
	if (!init_opencv()) {
		return EXIT_FAILURE;
	}

	if (!init_sdl()) {
		return EXIT_FAILURE;
	}

	while(!quit_flag) {
		process_capture();
		process_image();
		draw();
		process_event();
	} 

	finish_sdl();
	finish_opencv();

	return EXIT_SUCCESS;
}
