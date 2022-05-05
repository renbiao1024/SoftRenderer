#pragma once
#include <vector>
#include <string>
#include <fstream>
#include "Mesh.h"
//using namespace std;

void StringSplit(string s, char splitchar, vector<string>& vec);//·ÖÁÑ×Ö·û´®

void ReadObjFile(string path, Mesh* obj);