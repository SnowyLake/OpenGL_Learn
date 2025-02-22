/*
* @Module Name: builtin_obj_data.h
* @Auther: SnowyLake
* @Version: 0.3
* -------------------------------------------------------
* @Abstract:
*	This is the header file of my built-in objects' data.
*	This is just one of my attempts to implement unified
*	management of built-in objects.
*	This class is half-finished and ugly, and I'll be
*	refining and refactoring it later.
*/

#pragma once
#include <map>
#include <vector>
#include <string>

using BIOType = enum class BuiltInObjectType
{
    OBJ_CUBE,
    OBJ_PLANE,
};

using OTexPath_t = std::map<BIOType, const char*>;
using OVec_t = std::map<BIOType, std::vector<float>>;

using BIOData = class BuiltInObjectData
{
public:
    static OTexPath_t objTexturesPath;
    static OVec_t objVectices;

private:
    static std::string cubeTexPath;
    static std::string planeTexPath;

    static std::vector<float> cubeVertices;
    static std::vector<float> planeVertices;
};

