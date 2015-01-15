#include "waunzip.h"

#include <iostream>
#include <fstream>
#ifndef __WINDOWS__ 
#include "boost/filesystem.hpp"
namespace fs = boost::filesystem;
namespace fs = boost::filesystem;
#endif

extern char *optarg;
extern int optind, opterr;

static void usage(void){
    fprintf(stderr, "usage: unzip [options]\n");
    fprintf(stderr, " -%c string : %s\n", OPT_PASSWORD , "password"); 
    fprintf(stderr, " -%c string : %s\n", OPT_INPUT , "input"); 
    fprintf(stderr, " -%c string : %s\n", OPT_OUTPUT , "output");     
    fprintf(stderr, " -%c : %s\n", OPT_IGNORE_DOTFILE , "ignore dotfile");  
    fprintf(stderr, " -%c : %s\n", OPT_VERBOSE , "verbose");
    
    exit(1);
}

#pragma mark -

int main(int argc, char *argv[])
{
    if(argc == 1) usage();
    
    int ch;
    
    std::string password;
    std::string input; 
    std::string output;
    
    unsigned int ignore_dot = 0;
    unsigned int verbose = 0;
    
    while ((ch = getopt(argc, argv, "p:i:o:dv")) != -1){
                
        switch (ch){
                case OPT_PASSWORD:
                    password = std::string(optarg);
                    break;
                case OPT_INPUT:
                    input = std::string(optarg);
                    break;
                case OPT_OUTPUT:
                    output = std::string(optarg);
                    break; 
                case OPT_IGNORE_DOTFILE:
                    ignore_dot = 1;
                    break;  
                case OPT_VERBOSE:
                    verbose = 1;
                break;                  
            default:
                usage();
        }
    } 
    
    if(!input.length() || !output.length()){
        usage();
    }

#ifdef __WINDOWS__
	unescape_path(input);
#endif

    unzFile hUnzip = unzOpen64(input.c_str());
       
    unsigned int result = 0; 
    
    if(hUnzip){
        
        unz_file_info64 fileInfo;
        std::vector<char> szConFilename(PATH_MAX);
        std::string relativePath, filename, absolutePath;
        
        if(output.at(output.length()-1) != '/'){
           output += "/"; 
        }

#ifdef __WINDOWS__
        create_parent_folder(output);
#else
		boost::system::error_code error;
        fs::path parent_path = (fs::path(output)).parent_path();
        
        if(!fs::is_directory(parent_path)){
            fs::create_directory(parent_path, error);
        }        
#endif        
        
        do{
            if(unzGetCurrentFileInfo64(hUnzip, &fileInfo, (char *)&szConFilename[0], PATH_MAX, NULL, 0, NULL, 0) != 0){
                std::cout << "unzGetCurrentFileInfo64:" << "\n";
                result = result | 1;
                break;
            }
            
            relativePath = std::string((const char *)&szConFilename[0]);
            absolutePath = output + relativePath;
            
#ifdef __WINDOWS__
            create_parent_folder(absolutePath);
            get_file_name(relativePath, filename);
#else
            parent_path = (fs::path(absolutePath)).parent_path();
            
            if(!fs::is_directory(parent_path)){
                fs::create_directory(parent_path, error);
            }   
            
            filename = std::string(fs::path(relativePath).filename().c_str());
#endif                
            
            if(!ignore_dot 
               || ((filename.at(0) != '.')
               && (relativePath.find("/.") == std::string::npos))){
            
                if(relativePath.length() > 1){
                    
                    if(relativePath.at(relativePath.length()-1) == '/'){
                        
#ifdef __WINDOWS__
						create_folder(absolutePath);
#else
						fs::create_directory(absolutePath, error);
#endif
                    }
                    
                    if(password.length()){
                        if(unzOpenCurrentFilePassword(hUnzip, (const char *)password.c_str()) != 0){
                            std::cout << "unzOpenCurrentFilePassword" << "\n";
                            result = result | 1;
                            break;
                        }
                    }else{
                        if(unzOpenCurrentFile(hUnzip) != 0){
                            std::cout << "unzOpenCurrentFile" << "\n";
                            result = result | 1;
                            break;
                        }
                    }
                    
                    std::ofstream ofs(absolutePath.c_str(), std::ios::out|std::ios::binary);
                    
                    if(ofs.is_open()){
                        std::vector<char> buf(BUFFER_SIZE);
                        std::streamsize size;
                        
                        while ((size = unzReadCurrentFile(hUnzip, &buf[0], BUFFER_SIZE)) > 0)
                            ofs.write((const char *)&buf[0], size);
                        
                        ofs.close(); 
                        
                        if(verbose){
                            std::cout << "path: " << relativePath << "\n";
                        }
                    }  
                }              
            }

            unzCloseCurrentFile(hUnzip);
            
        }while(unzGoToNextFile(hUnzip) != UNZ_END_OF_LIST_OF_FILE);
        
        unzClose(hUnzip);

    }

	return 0;
}
