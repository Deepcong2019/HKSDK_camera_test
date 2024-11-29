//#include "stdafx.h"
#include "HK_Camera.h"
#include <iostream>
#include <opencv2\imgproc\imgproc.hpp>

//ȫ�ֱ���
LONG g_nPort;
cv::Mat g_BGRImage;
cv::VideoWriter g_VideoWriter;


//���ݽ���ص�����
//���ܣ���YV_12��ʽ����Ƶ������ת��Ϊ�ɹ�opencv�����BGR���͵�ͼƬ���ݣ���ʵʱ��ʾ
void CALLBACK DecCBFun(long nPort, char* pBuf, long nSize, FRAME_INFO* pFrameInfo,
    long nUser, long nReserved2)
{
    if (pFrameInfo->nType == T_YV12)
    {
        //cout << "Frame type is T_YV12" << endl;
        if (g_BGRImage.empty())
        {
            g_BGRImage.create(pFrameInfo->nHeight, pFrameInfo->nWidth, CV_8UC3);
        }
        cv::Mat YUVImage(pFrameInfo->nHeight + pFrameInfo->nHeight / 2,
            pFrameInfo->nWidth, CV_8UC1, (unsigned char*)pBuf);
        cv::cvtColor(YUVImage, g_BGRImage, cv::COLOR_YUV2BGR_YV12);
        //cv::resize(g_BGRImage, g_BGRImage, cv::Size(g_BGRImage.cols / 2, g_BGRImage.rows / 2));
        cv::imshow("RGBImage", g_BGRImage);
        if (g_VideoWriter.isOpened())
        {
            g_VideoWriter.write(g_BGRImage);
        }
        cv::waitKey(1);
        YUVImage.~Mat();
    }
}




void fRealDataCallBack(LONG lRealHandle, DWORD dwDataType, BYTE* pBuffer, DWORD dwBufSize, void* pUser)
{
    if (dwDataType == NET_DVR_STREAMDATA)//��������
    {
        if (dwBufSize > 0 && g_nPort != -1)
        {
            if (PlayM4_InputData(g_nPort, pBuffer, dwBufSize))
            {
                //cout << "Success input data " << endl;

            }
            else
            {
                cout << "Fail input data " << endl;
            }
        }
    }
}


HK_Camera::HK_Camera()
{

}

HK_Camera::~HK_Camera()
{
    if (g_VideoWriter.isOpened())
    {
        g_VideoWriter.release();
    }
}

//��ʼ��
bool HK_Camera::Init()
{
    if (NET_DVR_Init())
    {
        return true;
    }
    else
    {
        return false;
    }
}

//��¼
bool HK_Camera::Login(char* sIpAddress, char* sUserName, char* sPassword, WORD wPort)
{
    NET_DVR_DEVICEINFO_V30 devInfo;
    this->lUserID = NET_DVR_Login_V30(sIpAddress, wPort, sUserName, sPassword, &devInfo);
    if (this->lUserID < 0)
    {
        cout << "Login Failed!" << endl;
        return false;
    }
    else
    {
        cout << "Login Success!" << endl;
        return true;
    }

}

//��ʾͼƬ
void HK_Camera::ShowImg(bool bSaveVideo, string sfileName)
{
    if (bSaveVideo == true)
    {
        double fps = 25.0;
        g_VideoWriter.open(sfileName, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, cv::Size(1920, 1080), true);
    }
    if (PlayM4_GetPort(&g_nPort)) //��ȡ���ſ�ͨ����
    {
        if (PlayM4_SetStreamOpenMode(g_nPort, STREAME_REALTIME)) //������ģʽ
        {
            if (PlayM4_OpenStream(g_nPort, NULL, 0, 1024 * 1024))  // ����
            {
                if (PlayM4_SetDecCallBackExMend(g_nPort, DecCBFun, NULL, 0, NULL))
                {
                    if (PlayM4_Play(g_nPort, NULL))
                    {
                        cout << "Success to set play mode" << endl;
                    }
                    else
                    {
                        cout << "Fail to set play mode" << endl;
                    }
                }
                else
                {
                    cout << "Fail to set dec callback " << endl;
                }
            }
            else
            {
                cout << "Fail to open stream" << endl;
            }
        }
        else
        {
            cout << "Fail to set stream open mode" << endl;
        }
    }
    else
    {
        cout << "Fail to get port" << endl;
    }

    //����Ԥ�������ûص�������

    NET_DVR_CLIENTINFO struClientInfo;
    struClientInfo.lChannel = 1; // Channel Number �豸ͨ��
    struClientInfo.hPlayWnd = NULL;//����Ϊ�գ��豸SDK������ֻȡ��
    struClientInfo.lLinkMode = 0;//0��TCP��ʽ,1��UDP��ʽ,2���ಥ��ʽ,3 - RTP��ʽ��4-RTP/RTSP,5-RSTP/HTTP
    struClientInfo.sMultiCastIP = NULL; ;//�ಥ���ַ
    if (NET_DVR_RealPlay_V30(this->lUserID, &struClientInfo, fRealDataCallBack, NULL, TRUE))
    {

        cv::namedWindow("RgbImg");
    }
}