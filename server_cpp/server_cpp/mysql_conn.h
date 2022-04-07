#include <stdlib.h>
#include <iostream>
#include <mysql.h>
#include <string>
#include "cJSON.h"
#include <typeinfo>
#include <string>
#include <locale.h>

using namespace std;

char query[60000];

string GBK_2_UTF8(string gbkStr)
{
	string outUtf8 = "";
	int n = MultiByteToWideChar(CP_ACP, 0, gbkStr.c_str(), -1, NULL, 0);
	WCHAR *str1 = new WCHAR[n];
	MultiByteToWideChar(CP_ACP, 0, gbkStr.c_str(), -1, str1, n);
	n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);
	char *str2 = new char[n];
	WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, n, NULL, NULL);
	outUtf8 = str2;
	delete[]str1;
	str1 = NULL;
	delete[]str2;
	str2 = NULL;
	return outUtf8;
}


string UTF8_2_GBK(string utf8Str)
{

	string outGBK = "";
	int n = MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, NULL, 0);
	WCHAR *str1 = new WCHAR[n];
	MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, str1, n);
	n = WideCharToMultiByte(CP_ACP, 0, str1, -1, NULL, 0, NULL, NULL);
	char *str2 = new char[n];
	WideCharToMultiByte(CP_ACP, 0, str1, -1, str2, n, NULL, NULL);
	outGBK = str2;
	delete[] str1;
	str1 = NULL;
	delete[] str2;
	str2 = NULL;
	return outGBK;
}
/*
————————————————
版权声明：本文为CSDN博主「9990002222」的原创文章，遵循CC 4.0 BY - SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https ://blog.csdn.net/zc1239044730/article/details/61920238
*/

void elimate_quotation(string &quote) {
	for (int i = 0; i < quote.length(); i++) {
		if (quote[i] == '\"') {
			quote.erase(i, 1);
		}
	}
}

void eliminate_sqr(string &quote) {
	for (int i = 0; i < quote.length(); i++) {
		if (quote[i] == '[' || quote[i] == ']') {
			quote.erase(i,1);
		}
	}
}

static void sql_init(MYSQL &mysql, const char* db) {
	if (mysql_real_connect(&mysql, "192.168.1.81", "root", "Root_123", db, 3306, NULL, 0) == NULL) {
		cout << "link database fail" << endl;
	}
}

//function to insert data
//insert into index_name_match
static bool index_insert(MYSQL &mysql, string ATOCK_INDEX, string CHINESE_NAME) {
	//创建db
	string create = "CREATE TABLE IF NOT EXISTS index_name_match(ATOCK_INDEX INT NOT NULL primary key,CHINESE_NAME varchar(200) not null); ";
	strcpy_s(query, create.c_str());

	if (mysql_query(&mysql, query))        //执行SQL语句  
	{
		cout << u8"Query failed " << mysql_error(&mysql) << endl;
		memset(query, 0, 1000);
		create = "";
	}
	else
	{
		//printf("Insert success\n");
		memset(query, 0, 1000);
		create = "";
	}
	
	//插入数据
	string values;
	values = "INSERT IGNORE INTO index_name_match values ("+ ATOCK_INDEX+","+ CHINESE_NAME+");";
	strcpy_s(query, values.c_str());


	if (mysql_query(&mysql, query))        //执行SQL语句  
	{
		cout << u8"Query failed "<< mysql_error(&mysql)<<endl;
		memset(query, 0, 1000);
		values = "";
		return false;
	}
	else
	{
		//printf("Insert success\n");
		memset(query, 0, 1000);
		values = "";
		return true;
	}


}

static void data_insert_day(MYSQL &mysql, string ATOCK_INDEX, cJSON* myinsert) {
	//create db
	string create = "CREATE TABLE IF NOT EXISTS stock_day_data(ATOCK_INDEX INT NOT NULL,DAY_DATE varchar(15) not null,DAY_STATRT varchar(15) not null,DAY_END varchar(15) not null,DAY_MAX varchar(15) not null,DAY_MIN varchar(15) not null,DAY_TRAYNING_VALUME varchar(20) not null,primary key(ATOCK_INDEX, DAY_DATE)); ";
	strcpy_s(query, create.c_str());

	if (mysql_query(&mysql, query))        //执行SQL语句  
	{
		cout << u8"Query failed " << mysql_error(&mysql) << endl;
		memset(query, 0, 1000);
		create = "";
	}
	else
	{
		//printf("Insert success\n");
		memset(query, 0, 1000);
		create = "";
	}


	//插入数据
	string values="insert ignore into stock_day_data () values";
	int n = cJSON_GetArraySize(myinsert);
	for (int i = 0; i < n; i++) {
		int temp = cJSON_GetArraySize(cJSON_GetArrayItem(myinsert, i));
		for (int j = 6; j < temp; j++) {
			cJSON_DeleteItemFromArray(cJSON_GetArrayItem(myinsert, i), j);
		}
		string data = cJSON_Print(cJSON_GetArrayItem(myinsert, i));
		eliminate_sqr(data);
		values = values + "(" + ATOCK_INDEX+","+ data + "),";
		data = "";
	}
	values[values.size() - 1] = ';';

	strcpy_s(query, values.c_str());
	//cout << query << endl;
	if (mysql_query(&mysql, query))        //执行SQL语句  
	{
		cout << u8"Query failed " << mysql_error(&mysql) << endl;
		memset(query, 0, 1000);
		values = "";
	}
	else
	{
		//printf("Insert success\n");
		memset(query, 0, 1000);
		values = "";
	}
}
void createDB_min(MYSQL &mysql, string DB_name);

void data_insert_min(MYSQL &mysql, string ATOCK_INDEX, cJSON* myinsert) {
	elimate_quotation(ATOCK_INDEX);
	string DB_name = "stock_min_" + ATOCK_INDEX;
	createDB_min(mysql, DB_name);

	//insert
	string values = "insert ignore into " + DB_name + " () values";
	int n = cJSON_GetArraySize(myinsert);
	for (int i = 0; i < n; i++) {
		int temp = cJSON_GetArraySize(cJSON_GetArrayItem(myinsert, i));
		
		//avoid exception
		for (int j = 8; j < temp; j++) {
			cJSON_DeleteItemFromArray(cJSON_GetArrayItem(myinsert, i), j);
		}

		//delete empty one
		cJSON_DeleteItemFromArray(cJSON_GetArrayItem(myinsert, i), 6);

		string data = cJSON_Print(cJSON_GetArrayItem(myinsert, i));
		eliminate_sqr(data);
		values = values+"(" + data + "),";
		data = "";
	}
	values[values.size()-1] = ';';


	//执行
	strcpy_s(query, values.c_str());
	//cout << query << endl;
	if (mysql_query(&mysql, query))        //执行SQL语句  
	{
		cout << u8"Query failed " << mysql_error(&mysql) << endl;
		memset(query, 0, 1000);
		values = "";
	}
	else
	{
		//printf("Insert success\n");
		memset(query, 0, 1000);
		values = "";
	}
}

void createDB_min(MYSQL &mysql, string DB_name) {
	string myq = "CREATE TABLE IF NOT EXISTS "+ DB_name + " (DATEplusMIN_MIN varchar(15) NOT NULL,STATRT_MIN varchar(15) not null,END_MIN varchar(15) not null,MAX_MIN varchar(15) not null,MIN_MIN varchar(15) not null,TRANING_TOTAL_MIN varchar(15) not null,EXCHANGE_RATIO varchar(20) not null,primary key(DATEplusMIN_MIN))"; 

	strcpy_s(query, myq.c_str());

	if (mysql_query(&mysql, query))        //执行SQL语句  
	{
		cout << u8"database create failed " << mysql_error(&mysql) << endl;
		memset(query, 0, 1000);
		myq = "";
	}
	else
	{
		//printf("Insert success\n");
		memset(query, 0, 1000);
		myq = "";
	}
}


static char* sql_fetch(char fetch[256]) {
	char res[256] = "select * from test.tb_test;";

	return res;
}

static char* myinsert_query(cJSON* data) {
	char res[200] = { "INSERT INTO " };
	for (int i = 0; i <= 6; i++) {
		if (i != 6) {
			cJSON_GetArrayItem(data, i)->valuestring;
		}
		else {
			cJSON_GetArrayItem(data, +1)->valuestring;
		}
	}


	return res;
}

static char* mysql(char fetch[]) {
	MYSQL mysql;
	MYSQL_RES* res=nullptr;
	MYSQL_ROW next_row;
	int ret;
	//query statement

	//char partquery[256];

	//store result
	static char *buf = NULL;

	//初始化
	mysql_init(&mysql);

	//编码格式
	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "utf8");
	//cout << fetch << endl;
	cJSON* myJSON = cJSON_Parse(fetch);
	if (myJSON == nullptr) {
		cout << "parse wrong"<<endl;
		return nullptr;
	}
	//myJSON = cJSON_Parse(fetch);
	//cout << cJSON_Print(myJSON) << endl;

	//get aim
	string aim = cJSON_Print(cJSON_GetObjectItem(cJSON_GetObjectItem(myJSON, "data"), "aim"));
	elimate_quotation(aim);

	//get type
	string type = cJSON_Print(cJSON_GetObjectItem(cJSON_GetObjectItem(cJSON_GetObjectItem(myJSON, "data"), "details"), "type"));
	elimate_quotation(type);
	cout <<"type: "<< type << endl;

	//link to correspond database
	if (type == "m1" || type == "m5" || type == "m15" || type == "m30" || type == "m60") {
		//create link
		sql_init(mysql, "stock_min");

	}
	else if (type == "day1" || type == "day7" || type == "day30" || type == "day365") {
		//create link
		sql_init(mysql, "stock_day");
	}
	else {
		static char err[] = "type wrong";
		return err;
	}

	
	//implement insert
	if (aim == "insert") {
		string index = cJSON_Print(cJSON_GetObjectItem(cJSON_GetObjectItem(cJSON_GetObjectItem(myJSON, "data"), "details"), "index"));
		string cname = cJSON_Print(cJSON_GetObjectItem(cJSON_GetObjectItem(cJSON_GetObjectItem(myJSON, "data"), "details"), "name"));
		cout <<index <<"--"<<cname<<endl;

		//insert index_name_match
		index_insert(mysql, index, cname);

		//data insert
		cJSON* point_data = cJSON_GetObjectItem(cJSON_GetObjectItem(myJSON, "data"), "points");
		if (type == "day1" || type == "day7" || type == "day30" || type == "day365") {
			data_insert_day(mysql, index, point_data);//bug here
		}
		else if (type == "m1" || type == "m5" || type == "m15" || type == "m30" || type == "m60") {
			data_insert_min(mysql, index, point_data);
		}

		cout << "insert done" << endl;
		cJSON_Delete(myJSON);
	}
	//implement fetch
	else if (aim == "fetch") {
		/*
		//生成fetch_min query
		strcpy_s(query, partquery);

		//query
		ret = mysql_query(&mysql, query);
		if (ret) {
			printf(("query failed %s\n"), mysql_error(&mysql));
		}

		//get result
		if (res = mysql_store_result(&mysql)) {
			cout << "fetch success" << endl;
		}

		//transfer to json
		cJSON* json_final = cJSON_CreateArray();


		while (next_row = mysql_fetch_row(res)) {
			cJSON* rows;
			rows = new cJSON;
			rows = cJSON_CreateArray();
			for (int i = 0; i < mysql_num_rows(res); i++) {
				if (i == 0) {
					cJSON_AddItemToArray(rows, cJSON_CreateString(next_row[i]));
				}
				else {
					//double dp = atof(next_row[i]);
					cJSON_AddItemToArray(rows, cJSON_CreateNumber(atof(next_row[i])));
				}
			}
			//cout << endl;
			cJSON_AddItemToArray(json_final, rows);
		}

		//print test
		buf = cJSON_Print(json_final);
		//printf("%s\n", buf = cJSON_Print(json_final));


		cJSON_Delete(json_final);*/

		cout << "fetch done" << endl;


	}
	else {
		
		cout<<"damn"<<endl;
	}
	/*
	//create query statement 
		//生成insert query
		cJSON* data_list = cJSON_GetObjectItem(myJSON, "data");
		int num = cJSON_GetArraySize(data_list);
		for (int i = 0; i < num; i++) {
			//partquery = myinsert_query(cJSON_GetArrayItem(data_list, i));
		}

	}

		/*
		//JSON parse
		char myjson[] = "[\"123\", 1, 2, 3, 4, 5]";
		cJSON *json = cJSON_Parse(myjson);
		char *buf = NULL;
		printf("data:%s\n", buf = cJSON_Print(json));
		*/

	//}


	
	//free
	if(res!=nullptr)
		mysql_free_result(res);
	mysql_close(&mysql);
	return buf;
}
