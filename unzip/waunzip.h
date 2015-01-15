#ifndef __WAUNZIP_H__
#define __WAUNZIP_H__

#define OPT_PASSWORD        'p'
#define OPT_INPUT           'i'
#define OPT_OUTPUT          'o'
#define OPT_COMPRESS_LEVEL  'l'
#define OPT_IGNORE_DOTFILE  'd'
#define OPT_VERBOSE         'v'

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <time.h>

#include "zlib.h"
#include "zconf.h"
#include "zip/unzip.h"

#define BUFFER_SIZE 8192

#ifndef __linux__
#ifndef __APPLE__
#define __WINDOWS__
#include <string>
#include <vector>
#include <Shlobj.h>
#define PATH_MAX 4096
void unescape_path(std::string &path);
void get_file_name(std::string &relativePath, std::string &filename);
void create_parent_folder(std::string &path);
void create_folder(std::string &path);
#endif
#endif

#endif