#pragma once
#pragma once

#include <HCNetSDK.h>
#include <plaympeg4.h>
//#include <PlayM4.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;

class HK_Camera
{
public:
    HK_Camera();
    ~HK_Camera();

public:
    bool Init(); //³õÊ¼»¯
    bool Login(char* sIpAddress, char* sUserName, char* sPassword, WORD wPort);//µÇÂ¼
    void ShowImg(bool bSaveVideo = false, string sfileName = "");


private:
    LONG lUserID;
};