#pragma once
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

namespace ascvert {

class Ascii_Gen {
public:
    enum file_type {
        IMAGE,
        VIDEO
    };
    // Supply a path to either a video or an image
    Ascii_Gen(const std::string& path, file_type type);
    // Will display either a video ar an image based on what was supplied in the constructor
    void display();
    void set_scale_factor(double scale_factor);
    void set_charset(const std::string& charset = "");
    void set_loop();
    void set_color();
    void set_inverse();

private:
    static const int lookup_size_ = 256;
    const std::string short_charset_ = " .:-=+*#%@";
    std::string charset_ = " .'`^\",:;Il!i><~+_-?][}{1)(|/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";
    std::string path_;
    std::array<char, lookup_size_> charset_lookup_;
    double scale_factor_{};
    file_type type_;
    bool inverse_{};
    bool color_{};
    bool loop_{};

    // Generates a lookup table that is later used for finding an appropriate charceter from the charset
    void gen_lookup();

    // Returns an escape sequence that corresponds to a color of a pixel
    std::string get_color(const cv::Mat& img, int pixel_y, int pixel_x);

    // Handles rendering the video, apropriatly pacing frames
    void render_vid(cv::VideoCapture& vid);

    // Generates an ascii art frame/image
    void ascii_gen(cv::Mat& img);

    // Separate implementations for displaying an image or a video
    // Yes it is possible to handle an image like a video but
    // in the cv::VideoCapture implementation the screens gets cleared because it would be stupid to have a last frame of the video stil displayed
    void display(cv::Mat& img);
    void display(cv::VideoCapture& vid);
};

}; // namespace