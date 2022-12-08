#include "../inc/fileutil.h"

// to create a directory
// returns true if successful;
// otherwise, returns false;
bool MakeDir(const string & dir_path){
	bf::path p(dir_path); 
	if (bf::exists(p) && is_directory(p)){ // if directory p actually exist;
		std::cout << p << " already exists.\n";
		return true;
	}
	else // p actually does not exist;
		return bf::create_directory(p);
}


bool find_file( const bf::path & dir_path,     // in this directory,
                const std::string & file_name, // search for this name,
                bf::path & path_found          // placing path here if found
){
  if ( !bf::exists( dir_path ) ) return false;
  bf::directory_iterator end_itr; // default construction yields past-the-end
  for ( bf::directory_iterator itr( dir_path );
        itr != end_itr;
        ++itr ){
    if ( bf::is_directory(itr->status())){
      if ( find_file( itr->path(), file_name, path_found ) ) return true;
    }
	else if ( itr->path().filename() == file_name ){ // see below
      path_found = itr->path();
      return true;
    }
  }
  return false;
}


/*
// see http://www.cplusplus.com/forum/general/61834/
As follows: - from boost path.hpp
#   ifdef BOOST_WINDOWS_API
    const std::string string() const { return string(codecvt()); } 
    const std::string string(const codecvt_type& cvt) const
    { 
      std::string tmp;
      if (!m_pathname.empty())
        path_traits::convert(&*m_pathname.begin(), &*m_pathname.begin()+m_pathname.size(),
          tmp, cvt);
      return tmp;
    }
    
    //  string_type is std::wstring, so there is no conversion
    const std::wstring&  wstring() const { return m_pathname; }
    const std::wstring&  wstring(const codecvt_type&) const { return m_pathname; }
	*/


void GetDirList(const string & directory, vector<string> * dirlist){
	 bf::path p(directory);
  if (bf::is_directory(p)){
    for (bf::directory_iterator itr(p); itr!=bf::directory_iterator(); ++itr){
		if (bf::is_directory(itr->status()))
			dirlist -> push_back(itr->path().filename().string());
    }
  }
  else 
	  cout <<  p << " is not a directory!\n";
}


// to get the names of all the image files in the current path
void GetFileList(const string& directory, vector<string>* filelist){
	bf::path p(directory);
  if (bf::is_directory(p)){
    for (bf::directory_iterator itr(p); itr!=bf::directory_iterator(); ++itr){
		if (bf::is_regular_file(itr->status()))
			filelist -> push_back(itr->path().filename().string());
    }
  }
  else 
	  cout <<  p << " is not a directory!\n";
}

// to get the names of all the image files in the current path
void GetFileList(const string& directory, 
	const string & fileExtension, // e.g., = ".txt", including the dot ".";
	vector<string>* filelist
){
	bf::path p(directory);
  if (bf::is_directory(p)){
    for (bf::directory_iterator itr(p); itr!=bf::directory_iterator(); ++itr){
		if ((bf::is_regular_file(itr->status())) && (itr->path().extension().string() == fileExtension))
			filelist -> push_back(itr->path().filename().string());
    }
  }
  else 
	  cout <<  p << " is not a directory!\n";
}

void readDoubleDataFromBinaryFile(string filename, // "E:\\ComputerVisionCCJ\\Epitome\\eMean.bin"
	std::vector<double> & v_dataFromFile){
	long length;
	ifstream readFile;
	//	string filename = "E:\\ComputerVisionCCJ\\Epitome\\eMean.bin";
	readFile.open(filename, ios::binary | ios::in);

	// get length of file:
	// "seekg" is used to move the position to the end of the file, and then back to the beginning.
	readFile.seekg(0, ios::end);
	length = readFile.tellg();
	readFile.seekg(0, ios::beg);
	if (!readFile){
		cerr << "Open error!" << endl;
		//		exit(1);
	}

	if (abs(length) > (v_dataFromFile.size() * sizeof(double))){
		cout << "sizeof(double) = " << sizeof(double) << endl;
		cerr << "The vector is not big enough to save all the data from the binary file.\n";
		//		exit(1);
	}

	// read data as a block:
	readFile.read(reinterpret_cast<char*>(&v_dataFromFile[0]), length);
	readFile.close();
	cout << " The binary file is of length " << length << " bytes, i.e., " << length / sizeof(double) << " doubles\n";
	cout << "The output vector data: its size is " << v_dataFromFile.size() << endl;
	cout << "The first two elements: " << v_dataFromFile[0] << " and  " << v_dataFromFile[1] << endl;
	cout << "The last two elements: " << v_dataFromFile[length / sizeof(double) - 2] << " and " << v_dataFromFile[length / sizeof(double) - 1] << endl;
}

void readDoubleDataFromTxtFile(string filename, // e.g. "eMean.txt"
	std::vector<double> & v_dataFromFile){
	// open file    
	ifstream inputFile(filename);

	// test file open   
	if (inputFile) {
		double value;
		// read the elements in the file into a vector  
		while (inputFile >> value) {
			v_dataFromFile.push_back(value);
		}
	}

	inputFile.close();// close the file
}


void writedoubleMatToTxtFile(cv::Mat1d & m,
	string filename // e.g. "eMean.txt"
	){

	ofstream fout(filename);

	if (!fout){
		cout << "File Not Opened" << endl;  return;
	}

	for (int i = 0, rows = m.rows; i < rows; i++){
		for (int j = 0, cols = m.cols; j < cols; j++){
			fout << m.at<double>(i, j) << " ";
		}
		fout << endl;
	}
	fout.close();
}


// save the index to txt files
void writeIdxVectorDataToTxtFile(vector<vector<vector<int>>> & v, string filename){

	ofstream fout(filename);
	if (!fout){
		std::cout << "File Not Opened" << endl;  return;
	}

	for (int i = 0, size1 = v.size(); i < size1; i++){

		for (int j = 0; j < 5; j++){
			switch (j){
			case 0: {
				fout << "image height & width for image" << i + 1 << "/" << size1 << ":\n";
				for (int k = 0, length = v[i][j].size(); k < length; k++){
					fout << v[i][j][k] << " ";
				}
				fout << endl;
				break;
			}
			case 1: {
				fout << "row indices with random shifting for image" << i + 1 << "/" << size1 << ":\n";
				for (int k = 0, length = v[i][j].size(); k < length; k++){
					fout << v[i][j][k] << " ";
				}
				fout << endl;
				break;
			}
			case 2: {
				fout << "column indices with random shifting for image" << i + 1 << "/" << size1 << ":\n";
				for (int k = 0, length = v[i][j].size(); k < length; k++){
					fout << v[i][j][k] << " ";
				}
				fout << endl;
				break;
			}
			case 3: {
				fout << "row indices of maximum posterior element for image" << i + 1 << "/" << size1 << ":\n";
				for (int k = 0, length = v[i][j].size(); k < length; k++){
					fout << v[i][j][k] << " ";
				}
				fout << endl;
				break;
			}

			default: {
				fout << "column indices of maximum posterior element for image" << i + 1 << "/" << size1 << ":\n";
				for (int k = 0, length = v[i][j].size(); k < length; k++){
					fout << v[i][j][k] << " ";
				}
				fout << endl;
			}
			}
		} // end of j
		fout << endl;
		fout << endl;
	}// end of i

	fout.close();
}

void fileStorageHelp()
{
	cout << "\nfilestorage_sample demonstrate the usage of the OpenCV serialization functionality.\n"
		<< "usage:\n"
		<< "output file above can have many different extensions, see below."
		<< "\nThis program demonstrates the use of FileStorage for serialization, that is use << and >>  in OpenCV\n"
		<< "For example, how to create a class and have it serialize, but also how to use it to read and write matrices.\n"
		<< "FileStorage allows you to serialize to various formats specified by the file end type."
		<< "\nYou should try using different file extensions.(e.g. yaml yml xml xml.gz yaml.gz etc...)\n" << endl;
}

void writeDataToFileStorage(vector<vector<vector<int>>> & v,
	string & filename, // e.g. filename = "test.yml"
	string & fileNodeName // e.g. "imageIdx"
	){
	FileStorage fs_in(filename, FileStorage::WRITE);
	if (!fs_in.isOpened())
	{
		cerr << "failed to open " << filename << endl;
		fileStorageHelp();
	}

	fs_in << fileNodeName << "[";
	for (int i = 0, size1 = v.size(); i < size1; i++){
		// 0: "image height & width for image"
		// 1:  "row indices with random shifting for image"
		// 2: "column indices with random shifting for image" 
		// 3: "row indices of maximum posterior element for image"
		// 4:  "column indices of maximum posterior element for image"
		fs_in << "{:" << "size" << v[i][0] << "row_Idx" << v[i][1]
			<< "col_Idx" << v[i][2] << "maxPost_row_Idx" << v[i][3]
			<< "maxPost_col_Idx" << v[i][4] << "}";
	}
	fs_in << "]";
	fs_in.release();
}



void readDataFromFileStorage(vector<vector<vector<int>>> & v,
	string & filename, // e.g. filename = "test.yml"
	string & fileNodeName // e.g. "imageIdx"
	){
	FileStorage fs_out(filename, FileStorage::READ);
	if (!fs_out.isOpened())
	{
		cerr << "failed to open " << filename << endl;
		fileStorageHelp();
	}

	for (int i = 0, size1 = v.size(); i < size1; i++){
		FileNode forImages = fs_out[fileNodeName];
		FileNodeIterator it = forImages.begin(), // iterator pointing to the first node element.
			it_end = forImages.end(); // iterator pointing to the element following the last node element.

		// iterate through a sequence using FileNOdeIterator
		for (; it != it_end; it++){
			// 0: "size" = "image height & width for image"
			// 1:  "row_Idx" = "row indices with random shifting for image"
			// 2: "col_Idx" = "column indices with random shifting for image" 
			// 3:  "maxPost_row_Idx" = "row indices of maximum posterior element for image"
			// 4:   "maxPost_col_Idx" = "column indices of maximum posterior element for image"
			(*it)["size"] >> v[i][0];
			(*it)["row_Idx"] >> v[i][1];
			(*it)["col_Idx"] >> v[i][2];
			(*it)["maxPost_row_Idx"] >> v[i][3];
			(*it)["maxPost_col_Idx"] >> v[i][4];
		}
	}
	fs_out.release();
}


void read_Write_FileStorage(vector<vector<vector<int>>> & v,
	string & filename, // e.g. filename = "test.yml"
	string & filenodename,
	int flag  // #define Read_FileStorage 1; #define Write_FileStorage 0
	)
{
	switch (flag){

	case  Write_FileStorage: { //write date to file storage
		writeDataToFileStorage(v, filename, filenodename);
		break;

	}

	case Read_FileStorage: {//read data from file storage
		readDataFromFileStorage(v, filename, filenodename);
		break;
	}

	default:{
		fileStorageHelp();
		cout << "Please input integer 0 for writing or 1 for reading."
			<< "\nother numbers are invalid.\n"; }
	}
}