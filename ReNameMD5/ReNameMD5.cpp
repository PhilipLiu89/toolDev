#include "stdafx.h"
#include <iostream>  
#include <string>  
#include <io.h>  
#include "Crypto.h"
using namespace std;

bool bAll = false;

//遍历当前目录下的文件夹和文件,默认是按字母顺序遍历  
bool TraverseFiles(string path, int &file_num)
{
	_finddata_t file_info;
	string current_path = path + "/*.*"; //可以定义后面的后缀为*.exe，*.txt等来查找特定后缀的文件，*.*是通配符，匹配所有类型,路径连接符最好是左斜杠/，可跨平台  
	//打开文件查找句柄  
	int handle = _findfirst(current_path.c_str(), &file_info);
	//返回值为-1则查找失败  
	if (-1 == handle)
		return false;
	do
	{
		//判断是否子目录  
		string attribute;
		if (file_info.attrib == _A_SUBDIR) //是目录  
			attribute = "dir";
		else
			attribute = "file";
		//输出文件信息并计数,文件名(带后缀)、文件最后修改时间、文件字节数(文件夹显示0)、文件是否目录  
		cout << file_info.name << ' ' << file_info.time_write << ' ' << file_info.size << ' ' << attribute << endl; //获得的最后修改时间是time_t格式的长整型，需要用其他方法转成正常时间显示  
		file_num++;

	} while (!_findnext(handle, &file_info));  //返回0则遍历完  
	//关闭文件句柄  
	_findclose(handle);
	return true;
}

//深度优先递归遍历当前目录下文件夹和文件及子文件夹和文件  
void DfsFolder(string path, int layer)
{
	_finddata_t file_info;
	string current_path = path + "/*.*"; //也可以用/*来匹配所有  
	int handle = _findfirst(current_path.c_str(), &file_info);
	//返回值为-1则查找失败  
	if (-1 == handle)
	{
		cout << "cannot match the path" << endl;
		return;
	}

	do
	{
		//判断是否子目录  
		if (file_info.attrib == _A_SUBDIR)
		{
			//递归遍历子目录  
			//打印记号反映出深度层次  
			for (int i = 0; i<layer; i++)
				cout << "--";
			cout << file_info.name << endl;
			int layer_tmp = layer;
			if (strcmp(file_info.name, "..") != 0 && strcmp(file_info.name, ".") != 0)  //.是当前目录，..是上层目录，必须排除掉这两种情况  
				DfsFolder(path + '/' + file_info.name, layer_tmp + 1); //再windows下可以用\\转义分隔符，不推荐  
		}
		else
		{
			//打印记号反映出深度层次  
			for (int i = 0; i<layer; i++)
				cout << "--";
			cout << file_info.name << endl;
			std::string str = file_info.name;
			auto pos = str.find_last_of(".");
			if (pos == -1)
			{
				continue;
			}
			string strhz = str.substr(pos, str.length() - pos);
			string strFile = str.substr(0, str.length() - 4);

			if (strhz == ".csb" || strhz == ".xml" || strhz == ".FBX")
			{

				auto strRet = Crypto::md5(file_info.name);
				string strold = path + '/' + file_info.name;
				string strnew = path + '/' + strRet;
				rename(strold.c_str(), strnew.c_str());
			}
			
		}
	} while (!_findnext(handle, &file_info));  //返回0则遍历完  
	//关闭文件句柄  
	_findclose(handle);
}

void listFiles(const char * dir)
{
	char dirNew[256];
	strcpy(dirNew, dir);
	strcat(dirNew, "\\*.*");    // 在目录后面加上"\\*.*"进行第一次搜索
	string strPath = dir;
	intptr_t handle;
	_finddata_t findData;

	handle = _findfirst(dirNew, &findData);
	if (handle == -1)        // 检查是否成功
		return;

	do
	{
		if (findData.attrib & _A_SUBDIR)
		{
			if (strcmp(findData.name, ".") == 0 || strcmp(findData.name, "..") == 0)
				continue;

			cout << findData.name << "\t<dir>\n";

			// 在目录后面加上"\\"和搜索到的目录名进行下一次搜索
			strcpy(dirNew, dir);
			strcat(dirNew, "\\");
			strcat(dirNew, findData.name);

			listFiles(dirNew);
		}
		else
		{
			std::string strFileName = findData.name;
			auto pos = strFileName.find_last_of(".");
			if (pos == -1)
			{
				continue;
			}
			string strhz = strFileName.substr(pos, strFileName.length() - pos);
			if (strhz == ".csb" || strhz == ".xml" || strhz == ".FBX")
			{

				auto strRet = Crypto::md5(strFileName);
				string strold = strPath + '\\' + strFileName;
				string strnew = strPath + '\\' + strRet;
				rename(strold.c_str(), strnew.c_str());
			}
		}

	} while (_findnext(handle, &findData) == 0);

	_findclose(handle);    // 关闭搜索句柄
}

int main(int argc, char *argv[])
{
	cout << "请输入要转换的md5文件名的目录(支持csb/xml/fbx)：\n";
	string str;
	getline(cin, str);

	//递归遍历文件夹  
	//DfsFolder(str.c_str(), 0);
	listFiles(str.c_str());

	system("pause");
	return 0;
}