#include <iostream>
#include <argparse/argparse.hpp>
#include "ascvert.hpp"

namespace ac = ascvert;

std::unique_ptr<ac::Ascii_Gen> parse_args(int argc, char* argv[]);

int main(int argc, char* argv[]){
    try {
	    std::unique_ptr<ac::Ascii_Gen> ascii_gen_p = std::move(parse_args(argc, argv));
        ascii_gen_p->display();
    } catch(std::exception &e) {
        std::cerr << e.what();
        return 1;
    }

    return 0;
}

std::unique_ptr<ac::Ascii_Gen> parse_args(int argc, char* argv[]) {
    std::unique_ptr<ac::Ascii_Gen> ascii_gen_p;

    argparse::ArgumentParser arguments("ascvert", "1.0");

    auto& path_group = arguments.add_mutually_exclusive_group(true);
    path_group.add_argument("-i", "--image")
        .help("Path to an image");
    
    path_group.add_argument("-v", "--video")
        .help("Path to a video");

    arguments.add_argument("-f", "--factor")
        .help("Use a custom scale factor")
        .scan<'g', double>();
    
    auto& charset_group = arguments.add_mutually_exclusive_group();
    charset_group.add_argument("-s", "--short")
        .help("Use a shorter charset. Improves sometimes contrast.")
        .flag();

    charset_group.add_argument("-c", "--custom")
        .help("Use a custom character set");
        
    arguments.add_argument("-r", "--reverse")
        .help("Reverse the charset")
        .flag();

    arguments.add_argument("-l", "--loop")
        .help("Loop a video")
        .flag();
    
    arguments.add_argument("-C", "--color")
        .help("Use colors")
        .flag();
    
    arguments.parse_args(argc, argv);

    if(auto img_opt = arguments.present("-i")) {
        ascii_gen_p = std::make_unique<ac::Ascii_Gen>(*img_opt, ac::Ascii_Gen::VIDEO);
        if(arguments["-l"] == true)
            std::runtime_error("Can't loop an image\n");
    }

    if(auto video_opt = arguments.present("-v")) {
        if(arguments["-l"] == true)
            ascii_gen_p->set_loop();
        ascii_gen_p = std::make_unique<ac::Ascii_Gen>(*video_opt, ac::Ascii_Gen::VIDEO);
    }

    if(auto factor = arguments.present<double>("-f"))
        ascii_gen_p->set_scale_factor(*factor);

    if(arguments["-s"] == true)
        ascii_gen_p->set_charset();

    if(auto custom_opt = arguments.present("-c")) 
        ascii_gen_p->set_charset(*custom_opt);

    if(arguments["-r"] == true)
        ascii_gen_p->set_inverse();
    
    if(arguments["-C"] == true)
        ascii_gen_p->set_color();

    return ascii_gen_p;
}