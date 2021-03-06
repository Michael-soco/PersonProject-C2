#include"stdafx.h"
#include<iostream>
#include<fstream>
#include<cassert>
#include<string>
#include<algorithm>
#include<map>
#include<vector>
#include<ctime>
#define CLK_TCK CLOCKS_PER_SEC

using namespace std;

clock_t Begin, End;
double duration;

int Char = 0, Word = 0, Line = 0;

vector<pair<string, int>> vec;
ifstream infile;
map<string, int> mp;

void readTxt(string fname)
{
	infile.open(fname, ifstream::in);
	if (!infile.is_open())
	{
		cerr << "open error!" << endl;
		exit(1);
	}

	infile >> noskipws;   //不忽略空白符，也不忽略每行最后的'\n'
}

void charCount(ifstream &inf)		//inf.eof()多循环一次
{
	char c;
	while (inf.peek() != EOF)
	{
		inf >> c;
		Char++;
	}
}

bool isWord(string w)
{
	int l = w.length();
	if (l < 4) return false;
	for (int i = 0;i < 4;i++)
	{
		if (!(w[i] >= 97 && w[i] <= 122)) return false;
	}
	for (int i = 4;i < l;i++)
	{
		if (!((w[i] >= 97 && w[i] <= 122) || (w[i] >= 48 && w[i] <= 57)))
			return false;
	}
	return true;
}

int cmp(const pair<string, int> &x, const pair<string, int> &y)	//从大到小排序
{
	return x.second > y.second;
}

void wordCount(ifstream &inf)
{
	char c;

	inf.clear();
	inf.seekg(0);

	while (inf.peek() != EOF)
	{
		string w;
		inf >> c;
		while ((c >= 48 && c <= 57) || (c >= 65 && c <= 90) || (c >= 97 && c <= 122))
		{
			if (c >= 65 && c <= 90)   //大写转小写
				c = c + 32;

			w = w + c;				//越界
			inf >> c;
		}

		if (isWord(w))  //w是单词，将其加入map
		{
			Word++;
			if (mp.find(w) != mp.end())
			{
				mp[w]++;
			}
			else
			{
				mp[w] = 1;
			}
		}
	}

	for (map<string, int>::iterator it = mp.begin();it != mp.end();++it)
	{
		vec.push_back(make_pair(it->first, it->second));
	}
	sort(vec.begin(), vec.end(), cmp);
	
}

void Delete(string &s, const string &mark)
{
	size_t nSize = mark.size();
	while (1)
	{
		size_t pos = s.find(mark);
		if (pos == string::npos)
		{
			return;
		}
		s.erase(pos, nSize);
	}
}

void lineCount(ifstream &inf)		//空白字符处理
{
	string s;

	inf.clear();
	inf.seekg(0);

	while (inf)
	{
		getline(inf, s);
		Delete(s, " ");
		Delete(s, "\t");

		if (!s.empty()) Line++;
	}
}

void writeTxt()
{
	int i = 0;
	ofstream outfile;
	outfile.open("result.txt", std::ios::out | std::ios::ate);
	if (!outfile.is_open())
		cout << "open error!" << endl;

	outfile << "characters:" << Char << endl;
	outfile << "words:" << Word << endl;
	outfile << "lines:" << Line << endl;

	for (vector<pair<string, int>>::iterator it = vec.begin();it != vec.end() && i<10;++it, i++)
	{
		outfile <<'<'<< it->first <<">:" << it->second << endl;
	}
	
	outfile.close();
}

int main(int argc, const char* argv[])
{

	for (int i = 1;i < argc;i++)
	{
//		Begin = clock();

		string fname = argv[i];
		readTxt(fname);
		charCount(infile);
		wordCount(infile);
		lineCount(infile);
		writeTxt();

/*		End = clock();
		duration = (End - Begin) / CLK_TCK;
		cout << "tick:" << End - Begin << endl;
		cout << CLK_TCK << endl;
		cout << "duration:" << duration << endl;
*/
	}
	return 0;

}