#include "ascvert.hpp"
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <thread>

namespace ascvert {

Ascii_Gen::Ascii_Gen(const std::string& path, file_type type) :
    path_(path),
    type_(type) {}

void Ascii_Gen::display() {
    switch(type_) {
        case IMAGE: {
            cv::Mat img = cv::imread(path_, cv::IMREAD_COLOR);
            display(img);
            break;
        }
        case VIDEO: {
            cv::VideoCapture vid(path_);
            if(!vid.isOpened())
                throw std::runtime_error("Couldn't load video.");
            display(vid);
            break;
        }
    }
}

void Ascii_Gen::display(cv::Mat& img) {
    gen_lookup();
    ascii_gen(img);
}

void Ascii_Gen::display(cv::VideoCapture& vid) {
    gen_lookup();

    do {
        render_vid(vid);
    } while(loop_);

    system("clear");
}

void Ascii_Gen::set_charset(const std::string& charset) {
    if(charset.empty())
        charset_ = short_charset_;
    else
        charset_ = charset;

    if(charset.size() > lookup_size_)
        throw std::runtime_error("Charset is too big\n");

}

void Ascii_Gen::set_scale_factor(double scale_factor) { 
    if(scale_factor < 0 || scale_factor > 1)
        throw std::runtime_error("Invalid scale factor");

    scale_factor_ = scale_factor; 
}

void Ascii_Gen::set_loop()    { loop_ = true; }
void Ascii_Gen::set_color()   { color_ = true; }
void Ascii_Gen::set_inverse() { inverse_ = true; }

void Ascii_Gen::gen_lookup() {
    for(int i{}; i < lookup_size_; i++)
        charset_lookup_[i] = charset_[std::round((double)i / lookup_size_ * (charset_.length() ))];
}

std::string Ascii_Gen::get_color(const cv::Mat& img, int pixel_y, int pixel_x) {
    const uchar* pixel_row = img.ptr<uchar>(pixel_y);
    const int red_offset = 2;
    const int green_offset = 1;
    const int blue_offset = 0;
    return "\33[38;2;" + 
        std::to_string(int(pixel_row[pixel_x + red_offset])) + ";" + 
        std::to_string(int(pixel_row[pixel_x + green_offset])) + ";" + 
        std::to_string(int(pixel_row[pixel_x + blue_offset])) + "m";
}

void Ascii_Gen::ascii_gen(cv::Mat& img) {
    const double FONT_RATIO = 2.31;
    if(img.empty())
        throw std::runtime_error("Error: loading the image/frame failed\n");

    // Get terminal size
    winsize terminal_size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &terminal_size);

    if(scale_factor_)
        // If a scale factor is supplied then we need to multiply x by the FONT_RATIO
        // We need to do that so the image preserves shape, since characeters are not perfect squares unlike pixels
        cv::resize(img, img, cv::Size(), FONT_RATIO * scale_factor_, scale_factor_);
    else
        cv::resize(img, img, cv::Size(terminal_size.ws_col, terminal_size.ws_row));

    cv::Mat img_grayscale = img.clone();
    cv::cvtColor(img_grayscale, img_grayscale, cv::COLOR_BGR2GRAY);
    std::string ascii_img;
    ascii_img.reserve(img.size().area());

    int rows { img.rows };
    int original_cols { img.cols };
    int continuous_cols { img.cols }; 

    // The time complexity will be linear if the image is stored in continuous adress space
    if(img_grayscale.isContinuous()) {
        continuous_cols *= rows;
        rows = 1;
    }
    
    // Go pixel by pixel, for each find an appropriate character in charset_
    // Optionally display the color
    for(int i{} ; i < rows; i++) {
        uchar *pixel_row = img_grayscale.ptr<uchar>(i);

        for(int j{} ; j < continuous_cols; j++) {
            // Because we're mainly operationg on the grayscale image to get the color from the original we need to adjust for the presence of colors
            const int color_multiplayer = 3; 
            if(color_)
                ascii_img.append(get_color(img, i, j * color_multiplayer));

            if(inverse_)
                ascii_img.push_back(charset_lookup_.at(lookup_size_ - pixel_row[j] - 1));
            else
                ascii_img.push_back(charset_lookup_.at(pixel_row[j]));

            if((j+1) % original_cols == 0)
                ascii_img.push_back('\n');
        }
    }

    std::cout << ascii_img;
}

void Ascii_Gen::render_vid(cv::VideoCapture& vid) {
    double fps = vid.get(cv::CAP_PROP_FPS);
    int frame_duration_ms = 1000.0 / fps;

    cv::Mat frame;
    while(true) {
        // We start a new thread that will wait for the desired delay and join it when all the processing is done so that we don't need to factor in processing time
        std::thread wait_fps_duration([&]{
            std::this_thread::sleep_for(std::chrono::milliseconds(frame_duration_ms));
        });
        vid >> frame;
        if(frame.empty()) {
            wait_fps_duration.join();
            break;
        }

        // Reset the cursor to the base position
        // When the cursor is reset the already displayed characters will be 'overwritten' by the ones from a new frame
        std::cout << "\33[H";
        
        ascii_gen(frame);
        // Sync with frame_duration
        wait_fps_duration.join();
    }
}

}; // namespace