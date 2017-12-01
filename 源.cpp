#include <iostream>
#include <string>
#include <curl/curl.h>
#include <regex>
#include <vector>
#include <sstream>
#include <string.h>
#pragma comment(lib, "urlmon.lib")
using namespace std;

int count=0;
int v = 0;
int page = 0;
int num;
string PackName;
vector<string>urlc;

size_t CurlWrite_CallbackFunc_StdString(void *contents, size_t size, size_t nmemb, std::string *s)
{
	size_t newLength = size*nmemb;
	size_t oldLength = s->size();
	s->resize(oldLength + newLength);
	std::copy((char*)contents, (char*)contents + newLength, s->begin() + oldLength);
	return size*nmemb;
}

void download(LPCSTR urls) {
	string fname = "./" + to_string(v) + ".jpg";
	HRESULT hr = URLDownloadToFile(0, urls,fname.c_str(),0,NULL);
	v++;
	cout << "OK"<<endl;
}

string ItoS(int num) {
	stringstream temp;
	string temp2;
	temp << num;
	temp >> temp2;
	return temp2;
}
void geturl() {
	regex pattern("\"thumbURL\":\"(.+?)\"");
	smatch r1;
	string payback;
	CURL *curl = nullptr;
		curl = curl_easy_init();
		if (curl != nullptr) {
			char* encode_url = curl_easy_escape(curl, PackName.c_str(), 0);
			string e_url = encode_url;
			string url2 = "https://image.baidu.com/search/flip?tn=baiduimage&word=" + e_url + "&pn=" + to_string(page)+"&ie=gb2312";
			cout << e_url << endl;
			curl_easy_setopt(curl, CURLOPT_URL, url2.c_str());
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlWrite_CallbackFunc_StdString);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &payback);
			curl_easy_perform(curl);
			//curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
			/*if (res != CURLE_OK) {
				fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
			}*/
			//cout << endl << "���Ƿֽ���" << endl <<payback<<endl;
			while(std::regex_search(payback, r1, pattern)) {
				// �����ʽ1
				for (auto i = 1; i != r1.size(); ++i) {
					//cout << r1[i] << endl;
					urlc.push_back(r1[i]);
					::count++;				
				}
				// �����ʽ2
				//for (auto it = r1.begin(); it != m.end(); ++it)
					//std::cout << *it << " ";
				//std::cout << std::end;
				payback = r1.suffix().str();  // ����ĩ�ˣ���Ϊ�µ������Ŀ�ʼ
			}
			curl_easy_cleanup(curl);
			curl_free(encode_url);
		}
		cout << "�˴�Ϊ�ֽ���"<<endl;
		for (int i = 0; i < 60; i++) {
			cout << urlc[i] << endl;
			download(urlc[i].c_str());
		}
		urlc.clear();
}

void main() {
	cout << "���������ص�����(ӦΪ60�ı���)" << endl;
	cin >> num;
	cout << "������ͼƬ����,����:�����ܼ������\n";
	cin >> PackName;
	for (; page<num;page += 60)
		geturl();
}