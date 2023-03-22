#include<opencv2\opencv.hpp>
#include<algorithm>
#include<iostream>
#include<fstream>
#include <io.h>
#include <string>
#include <vector>

using namespace std;
using namespace cv;

Mat src, Imglabels, Imgstats, Imgcentroids;
int Imglabelnum;

void getFiles(string path, vector<string>& files) {
	//文件句柄  
	intptr_t hFile = 0;
	//文件信息，声明一个存储文件信息的结构体  
	struct _finddata_t fileinfo;
	string p;  //字符串，存放路径
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)//若查找成功，则进入
	{
		do
		{
			//如果是目录,迭代之（即文件夹内还有文件夹）  
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				//文件名不等于"."&&文件名不等于".."
				//.表示当前目录
				//..表示当前目录的父目录
				//判断时，两者都要忽略，不然就无限递归跳不出去了！
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			//如果不是,加入列表  
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		//_findclose函数结束查找
		_findclose(hFile);
	}
}

void write_file(int labelnum,string filename)
{
	int i, j = 1;
	ofstream  ofs;
	ofs.open(filename, ios::out);
	ofs << labelnum << '\n';
	for (i = 1; i < Imglabelnum; i++)
	{
		if (Imgstats.at<int>(i, CC_STAT_AREA) > 0)
		{
			ofs << Imgstats.at<int>(i, CC_STAT_LEFT) << ' ';
			ofs << Imgstats.at<int>(i, CC_STAT_TOP) << ' ';
			ofs << Imgstats.at<int>(i, CC_STAT_WIDTH) << ' ';
			ofs << Imgstats.at<int>(i, CC_STAT_HEIGHT) << ' ';
			ofs << "object" << j++ << '\n';
		}
	}
	ofs.close();
}

void connect_components(string filename,string Targetpath,int len_bef_photo)
{
	
	int labelnum = 0;
	//printf("%s\n", filename);
	src = imread(filename, 0);
	Imglabelnum = connectedComponentsWithStats(src, Imglabels, Imgstats, Imgcentroids);
	for (int i = 1; i < Imglabelnum; i++)
	{
		if (Imgstats.at<int>(i, CC_STAT_AREA) > 0)
		{
			labelnum++;
		}
	}
	
	string str = filename;
	str = str.erase(0, len_bef_photo);
	//cout << str << endl;
	//string str = filename.substr(filename.length() - 7,filename.length());
	//printf("%s\n", str);
	
	//filename.erase(filename.length() - 13);
	//printf("%s\n", filename);
	//filename += "bound\\";
	str.erase(str.length() - 3);
	str += "txt";
	Targetpath += str;
	//printf("%s\n", str);
	write_file(labelnum,Targetpath);
}

int main(int agrc, char* argv[])
{
	//string FilePath = "D:\\Users\\86152\\Desktop\\U2Netmaster\\U2Netmaster\\test_data\\u2net_results";//自己设置目录    
	//string Targetpath = "D:\\Users\\86152\\Desktop\\Connected_domain\\Connected_domain\\bound";//TXT文档生成路径
	string FilePath = argv[1]; //int agrc,char *argv[]
	string Targetpath = argv[2];
	vector<string> FileName_List;
	int len_bef_photo = FilePath.length();
	getFiles(FilePath, FileName_List);
	for (int i = 0; i < FileName_List.size(); i++)
	{ 
		connect_components(FileName_List[i].c_str(), Targetpath,len_bef_photo);
		//printf("%s\n", FileName_List[i].c_str());
		//cout << FileName_List[i].c_str() << endl;
	}

	//imshow("src", src);
	//waitKey(0);
	return 0;
}