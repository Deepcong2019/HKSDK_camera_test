#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <direct.h>
#include "HK_Camera.h"
#include <iostream>
#include <opencv2/core/utils/logger.hpp>

using namespace std;


int main(int argc, char* argv[])
{
    //cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);//不再输出日志
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_ERROR);//只输出错误日志
    HK_Camera camera;
    char sIpAddress[] = "172.16.51.90";
    char sUserID[] = "admin";
    char sPassword[] = "jiean300845";
    WORD wPort = 8000;

    if (camera.Init())
    {
        cout << "Init Success" << endl;
        if (camera.Login(sIpAddress, sUserID, sPassword, wPort))
        {
            cout << "Login Success" << endl;
            camera.ShowImg(true, "test.avi");

        }
        else
        {
            cout << "Login Fail" << endl;
        }
    }
    else
    {
        cout << "Init Fail" << endl;
    }
    system("pause");
    return 0;
}