#include <iostream>
#include "p5.hpp"
#include <vector>
#include <cmath>
#include <thread>
#include <mutex>

P5& p5 = P5::getInstance();

constexpr int cell_size = 5;
int w, h;
std::vector<float> grid;
std::vector<float> grid_diff;
float r_a = 11.0f;
float b_1 = 0.278f;
float b_2 = 0.365f;
float d_1 = 0.267f; 
float d_2 = 0.445f;
float dt = 0.1f;
float alpha_n = 0.028f, alpha_m = 0.147f;
std::mutex grid_mutex;

int emod(int a, int mod) {
    return (a % mod + mod) % mod;
}

float sig1(float x, float a) {
    return 1.0f / (1.0f + expf(-(x - a) * 4 / alpha_n));
}

float sig2(float x, float a, float b) {
    return sig1(x, a) * (1 - sig1(x, b));
}

float sig_m(float x, float y, float m) {
    return x * (1 - sig1(m, 0.5f)) + y * sig1(m, 0.5f);
}

float s(float n, float m) {
    return sig2(n, sig_m(b_1, d_1, m), sig_m(b_2, d_2, m));
}

void clamp(float* x, float l, float h) {
    if (*x < l) *x = l;
    if (*x > h) *x = h;
}

void compute_diff_region(int start_y, int end_y) {
    for (int y = start_y; y < end_y; y++) {
        for (int x = 0; x < w; x++) {
            float n = 0, m = 0, N = 0, M = 0;
            float r_i = r_a / 3.0;

            for (int i = -(r_a - 1); i < r_a; i++) {
                for (int j = -(r_a - 1); j < r_a; j++) {
                    int cur_y = emod(y + j, h);
                    int cur_x = emod(x + i, w);

                    if (i * i + j * j <= r_i * r_i) {
                        m += grid[cur_y * w + cur_x];
                        M += 1;
                    } else if (i * i + j * j <= r_a * r_a) {
                        n += grid[cur_y * w + cur_x];
                        N += 1;
                    }
                }
            }
            n /= N;
            m /= M;

            float diff_temp = s(n, m);
            grid_diff[y * w + x] = 2 * diff_temp - 1;
        }
    }
}

void apply_diff_region(int start_y, int end_y) {
    for (int y = start_y; y < end_y; y++) {
        for (int x = 0; x < w; x++) {
            grid_mutex.lock();
            grid[y * w + x] += dt * grid_diff[y * w + x];
            clamp(&grid[y * w + x], 0, 1);
            grid_mutex.unlock();
        }
    }
}

void compute_and_apply_diff(int start_y, int end_y) {
    compute_diff_region(start_y, end_y);
    apply_diff_region(start_y, end_y);
}

void parallel_compute_and_apply_diff() {
    int num_threads = std::thread::hardware_concurrency();
    int region_height = h / num_threads;
    std::vector<std::thread> threads;
    for (int i = 0; i < num_threads; i++) {
        int start_y = i * region_height;
        int end_y = (i == num_threads - 1) ? h : (i + 1) * region_height;
        threads.emplace_back(compute_and_apply_diff, start_y, end_y);
    }
    for (auto& thread : threads) {
        thread.join();
    }
}

void initialize_grid() {
    size_t wi = w / 3;
    size_t he = h / 3;
    for (size_t dy = 0; dy < he; ++dy) {
        for (size_t dx = 0; dx < wi; ++dx) {
            size_t x = dx + w / 2 - wi / 2;
            size_t y = dy + h / 2 - he / 2;
            grid[y * w + x] = p5.random();
        }
    }
    // for(int i = 0; i < w * h; i++) {
    //     grid[i] = p5.random();
    // }
}

void setup() {
    p5.createCanvas(1200, 800);
    w = p5.width / cell_size;
    h = p5.height / cell_size;
    grid.resize(w * h);
    grid_diff.resize(w * h);
    p5.noStroke();
    initialize_grid();
    p5.frameRate(120);
}

void draw() {
    p5.background(0);

    // for (int i = 0; i < 2; i++) {
        parallel_compute_and_apply_diff();
    // }

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            p5.fill(255 * grid[i * w + j]);
            p5.rect(j * cell_size, i * cell_size, cell_size, cell_size);
        }
    }
}

void mousePressed() {
    initialize_grid();
}

int main() {
    p5.registerMethod("setup", setup);
    p5.registerMethod("draw", draw);
    p5.registerMethod("mousePressed", mousePressed);

    p5.run();

    return 0;
}
