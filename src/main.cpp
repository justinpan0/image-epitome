#include "../inc/main.h"

int main(int argc, char * argv[])
{
	clock_t startTime = clock();

    string configFile = argv[1];
    bool verbose = argv[2] == "true";
    bool isgrayScale = argv[3] == "true";
	bool randomshifting = argv[4] == "true"; // make a random shift form the regular patches' positions
    string whatkindImgs = argv[5];
    // e.g., = 1, means only learn epitome;
	// e.g., = 2, means learn and reconstruction; 
	// e.g., = 3, means processing of row-column indices and direct images reconstruction;
	string s_flag_epi_learn_recon = argv[6];
    cout << (argv[2] == "true") << " " << verbose << " " << isgrayScale << " " << randomshifting << endl;

    Config cfg;
    try
    {
        cfg.readFile(configFile);
        DatabaseDir = cfg.lookup("DatabaseDir").operator string();
        EpitomeResultDir = cfg.lookup("EpitomeResultDir").operator string();
        ReconsCompresDir = cfg.lookup("ReconsCompresDir").operator string();
        newPatchSpacingNum = cfg.lookup("newPatchSpacingNum");
        newPatchSpacing = cfg.lookup("newPatchSpacing");
        fileNameforRowCol_Idx_YML = cfg.lookup("fileNameforRowCol_Idx_YML").operator string();
        save_img_num = cfg.lookup("save_img_num");
        nthreads = cfg.lookup("nthreads");
        max_omp_for_idx = cfg.lookup("max_omp_for_idx");
        epitomeWidth = cfg.lookup("epitomeWidth");
        epitomeHeight = cfg.lookup("epitomeHeight");
        patchSideLength = cfg.lookup("patchSideLength");
        patchSpacing = cfg.lookup("patchSpacing");
        numIteration = cfg.lookup("numIteration");
        nameDifference = cfg.lookup("nameDifference").operator string();
        imgEpi_EncodeType = cfg.lookup("imgEpi_EncodeType").operator string();
        Epi_overheadKind_Num = cfg.lookup("Epi_overheadKind_Num");
    }
    catch(const FileIOException &fioex)
    {
        cerr << "I/O error while reading file." << "\n";
        return(EXIT_FAILURE);
    }
    catch(const ParseException &pex)
    {
        cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
                << " - " << pex.getError() << "\n";
        return(EXIT_FAILURE);
    }
    catch(const SettingNotFoundException &nfex)
    {
        cerr << "Missing setting at " << nfex.getPath() << "\n";
    }
    catch(const SettingTypeException &tex)
    {
        cerr << "Wrong setting type at " << tex.getPath() << "\n";
    }

    vector<string> inputImgsList;
    GetFileList(DatabaseDir, &inputImgsList);
    int newPatchSpacing = 0;

    if (s_flag_epi_learn_recon == "1"){ // only learn epitome
        // Initialize GrayImgEpitome Object
        // 1. make some necessary directories 
        // 2. epitome reconstruction dir
        cout << " s_flag_epi_learn_recon == 1, that is only learn epitome, without image reconstruction.\n";
        MakeDir(EpitomeResultDir);
        EpitomeResultDir = EpitomeResultDir + "/" + whatkindImgs + "-Epi_" + STR_BOOL(randomshifting) + "_" + std::to_string(static_cast<long long>(epitomeWidth)) + "_"
			+ std::to_string(static_cast<long long>(patchSideLength)) + "_" + std::to_string(static_cast<long long>(patchSpacing)) + "_"
			+ std::to_string(static_cast<long long>(numIteration)) + "-baseline";
        MakeDir(EpitomeResultDir);

        ImgEpitome* img1 = new ImgEpitome(
            epitomeWidth,
            epitomeHeight,
            inputImgsList.size(),
            patchSideLength,
            patchSpacing,
            numIteration,
            isgrayScale,
            verbose,
            randomshifting,
            DatabaseDir,
            EpitomeResultDir,
            ReconsCompresDir,
            whatkindImgs,
            nameDifference,
            imgEpi_EncodeType,
            Epi_overheadKind_Num,
            nthreads,
            max_omp_for_idx);

        bool IsConstantValueInitial = false;
        double constantVal = 0.8;
        double weight_for_sigma = 0.02;
        img1->initialImgEpitome(inputImgsList, weight_for_sigma, IsConstantValueInitial, constantVal);

        // Learn Epitome
        img1->learnEpitome();

        // Cleanup
        img1->clearImgEpitome();
        delete img1;
    }

	return 0;
}
