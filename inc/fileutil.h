// boost filesystem
#define BOOST_FILESYSTEM_NO_DEPRECATED // For new code, defining "BOOST_FILESYSTEM_NO_DEPRECATED" before including filesystem headers is strongly recommended. 
                                       // This prevents inadvertent use of old features, particularly legacy function names, 
                                       // that have been replaced and are going to go away in the future.
#include <boost/filesystem/operations.hpp>
#include <opencv2/core/core.hpp>
#include "opencv2/opencv.hpp"
#include <iostream>               // for std::cout
#include <string>
#include <vector>
#include <fstream>
using namespace std;
namespace bf = boost::filesystem;
using namespace cv;
#define Read_FileStorage 1
#define Write_FileStorage 0

// to create a directory
// returns true if successful;
// otherwise, returns false;
bool MakeDir(const std::string & dir_path);


bool find_file( const bf::path & dir_path,     // in this directory,
                const std::string & file_name, // search for this name,
                bf::path & path_found          // placing path here if found
);

void GetDirList(const std::string & directory, std::vector<std::string> * dirlist);

// to get the names of all the image files in the current path
void GetFileList(const std::string & directory, std::vector<std::string> * filelist);

// overload + 1
void GetFileList(const string& directory, 
	const string & fileextension, // e.g., = ".txt"
	vector<string>* filelist);

// -------------- Read/Write Files --------------
void readDoubleDataFromBinaryFile(string filename, // "E:\\ComputerVisionCCJ\\Epitome\\eMean.bin"
	std::vector<double> & v_dataFromFile);

void readDoubleDataFromTxtFile(string filename, // e.g. "eMean.txt"
	std::vector<double> & v_dataFromFile);

// -------------- File Storage --------------
void writedoubleMatToTxtFile(cv::Mat1d & m,
	string filename // e.g. "eMean.txt"
	);

// save the index to txt files
void writeIdxVectorDataToTxtFile(vector<vector<vector<int>>> & v, string filename);

void fileStorageHelp();

void writeDataToFileStorage(vector<vector<vector<int>>> & v,
	string & filename, // e.g. filename = "test.yml"
	string & fileNodeName // e.g. "imageIdx"
	);



void readDataFromFileStorage(vector<vector<vector<int>>> & v,
	string & filename, // e.g. filename = "test.yml"
	string & fileNodeName // e.g. "imageIdx"
	);


void read_Write_FileStorage(vector<vector<vector<int>>> & v,
	string & filename, // e.g. filename = "test.yml"
	string & filenodename,
	int flag  // #define Read_FileStorage 1; #define Write_FileStorage 0
	);