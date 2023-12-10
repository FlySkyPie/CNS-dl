#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string> 
#include <cstring>
#include <vector>
#include <iterator>
#include <iomanip>
#include "rapidxml.hpp"
using namespace std;

using namespace rapidxml;

template <typename T>
  string NumberToString ( T Number )
  {
     ostringstream ss;
     ss << Number;
     return ss.str();
  }

template<typename Out>
void split(const std::string &s, char delim, Out result) 
{
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) 
	{
        *(result++) = item;
    }
}

std::vector<std::string> split(const std::string &s, char delim) 
{
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

string padTo(int num)
{
	ostringstream ss;
	ss << setw(5) << setfill('0') << num ;
	string s2(ss.str());
	return s2;
}

int main()
{
	cout << "請輸入CNS編號：";
	string id="";
	cin >> id;

	cout << "請輸入總頁數：";
	int total_page = 0;
	cin >> total_page;

	for(int i=1;i<=total_page;i++)
	{	string page = NumberToString(i);
		string get_token,get_image;
		string cmd = "";

		cmd = "curl --data \"generalno=" + id + "&version=zh_TW&pageNum=" + page + "\" http://www.cnsonline.com.tw/preview/GetData > temp";
		system(cmd.c_str());

		//read file
		xml_document<> doc;
		ifstream file("temp");
		stringstream buffer;
		buffer << file.rdbuf();
		file.close();
		string content(buffer.str());
		doc.parse<0>(&content[0]);
		
		xml_node<> *pRoot = doc.first_node();
		xml_node<> * beer_node = pRoot->first_node("Message");

		vector<string> token = split(beer_node->value(), ',');

		cmd = "wget -nc -O " +  padTo(i) +".jpg http://www.cnsonline.com.tw/preview/GenerateImage?generalno=" + id + "\\&version=zh_TW\\&pageNum=" + page +"\\&checksum=" + token[1] ;
		cout << cmd << endl;
		system(cmd.c_str());			
	}
	system("find . -size  0 -print0 |xargs -0 rm");
	string cmd = "convert *.jpg CNS-" + NumberToString(id) +".pdf";
	system(cmd.c_str());
	cmd = "rm -f *.jpg";
	system(cmd.c_str());
	return 0;
}