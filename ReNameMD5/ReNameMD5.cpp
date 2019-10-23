#include "stdafx.h"
#include <iostream>  
#include <string>  
#include <io.h>  
#include "Crypto.h"
using namespace std;

bool bAll = false;

//������ǰĿ¼�µ��ļ��к��ļ�,Ĭ���ǰ���ĸ˳�����  
bool TraverseFiles(string path, int &file_num)
{
	_finddata_t file_info;
	string current_path = path + "/*.*"; //���Զ������ĺ�׺Ϊ*.exe��*.txt���������ض���׺���ļ���*.*��ͨ�����ƥ����������,·�����ӷ��������б��/���ɿ�ƽ̨  
	//���ļ����Ҿ��  
	int handle = _findfirst(current_path.c_str(), &file_info);
	//����ֵΪ-1�����ʧ��  
	if (-1 == handle)
		return false;
	do
	{
		//�ж��Ƿ���Ŀ¼  
		string attribute;
		if (file_info.attrib == _A_SUBDIR) //��Ŀ¼  
			attribute = "dir";
		else
			attribute = "file";
		//����ļ���Ϣ������,�ļ���(����׺)���ļ�����޸�ʱ�䡢�ļ��ֽ���(�ļ�����ʾ0)���ļ��Ƿ�Ŀ¼  
		cout << file_info.name << ' ' << file_info.time_write << ' ' << file_info.size << ' ' << attribute << endl; //��õ�����޸�ʱ����time_t��ʽ�ĳ����ͣ���Ҫ����������ת������ʱ����ʾ  
		file_num++;

	} while (!_findnext(handle, &file_info));  //����0�������  
	//�ر��ļ����  
	_findclose(handle);
	return true;
}

//������ȵݹ������ǰĿ¼���ļ��к��ļ������ļ��к��ļ�  
void DfsFolder(string path, int layer)
{
	_finddata_t file_info;
	string current_path = path + "/*.*"; //Ҳ������/*��ƥ������  
	int handle = _findfirst(current_path.c_str(), &file_info);
	//����ֵΪ-1�����ʧ��  
	if (-1 == handle)
	{
		cout << "cannot match the path" << endl;
		return;
	}

	do
	{
		//�ж��Ƿ���Ŀ¼  
		if (file_info.attrib == _A_SUBDIR)
		{
			//�ݹ������Ŀ¼  
			//��ӡ�Ǻŷ�ӳ����Ȳ��  
			for (int i = 0; i<layer; i++)
				cout << "--";
			cout << file_info.name << endl;
			int layer_tmp = layer;
			if (strcmp(file_info.name, "..") != 0 && strcmp(file_info.name, ".") != 0)  //.�ǵ�ǰĿ¼��..���ϲ�Ŀ¼�������ų������������  
				DfsFolder(path + '/' + file_info.name, layer_tmp + 1); //��windows�¿�����\\ת��ָ��������Ƽ�  
		}
		else
		{
			//��ӡ�Ǻŷ�ӳ����Ȳ��  
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
	} while (!_findnext(handle, &file_info));  //����0�������  
	//�ر��ļ����  
	_findclose(handle);
}

void listFiles(const char * dir)
{
	char dirNew[256];
	strcpy(dirNew, dir);
	strcat(dirNew, "\\*.*");    // ��Ŀ¼�������"\\*.*"���е�һ������
	string strPath = dir;
	intptr_t handle;
	_finddata_t findData;

	handle = _findfirst(dirNew, &findData);
	if (handle == -1)        // ����Ƿ�ɹ�
		return;

	do
	{
		if (findData.attrib & _A_SUBDIR)
		{
			if (strcmp(findData.name, ".") == 0 || strcmp(findData.name, "..") == 0)
				continue;

			cout << findData.name << "\t<dir>\n";

			// ��Ŀ¼�������"\\"����������Ŀ¼��������һ������
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

	_findclose(handle);    // �ر��������
}

int main(int argc, char *argv[])
{
	cout << "������Ҫת����md5�ļ�����Ŀ¼(֧��csb/xml/fbx)��\n";
	string str;
	getline(cin, str);

	//�ݹ�����ļ���  
	//DfsFolder(str.c_str(), 0);
	listFiles(str.c_str());

	system("pause");
	return 0;
}