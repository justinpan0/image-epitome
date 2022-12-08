#include "pch.h"

string DatabaseDir;
string EpitomeResultDir;
string ReconsCompresDir;
int newPatchSpacingNum;
int newPatchSpacing;
string fileNameforRowCol_Idx_YML;
int save_img_num;
int nthreads;
int max_omp_for_idx;
int epitomeWidth;
int epitomeHeight;
int patchSideLength;
int patchSpacing;
int numIteration;
string nameDifference;
string imgEpi_EncodeType;
int Epi_overheadKind_Num;

inline static string STR_BOOL(bool flag)
{
    return flag ? "1" : "0";
}