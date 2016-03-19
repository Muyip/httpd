#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "sql_connect.h" //sql_connect.h
using namespace std;

//remote database
const string _host = "127.0.0.1";
const string _user = "common";
const string _passwd = "common";
const string _db   = "study";

string get_insert_data(char *data, int content_length)
{
	char id[64];
	char name[64];
	char age[64];
	char habby[64];
	bzero(id, sizeof(id));
	bzero(name, sizeof(name));
	bzero(age, sizeof(age));
	bzero(habby, sizeof(habby));

	for(int i = 0; i < content_length; ++i) //id=0&name=common&age=10&habby=listen
	{
		if(data[i]=='=' || data[i]=='&')
			data[i] = ' ';
	}
	sscanf(data,"%*s %s %*s %s %*s %s %*s %s",id,name,age,habby);  //****** important  //id 0 name common age 10 habby listen
		
	string insert_data = "";
	insert_data += id;
	insert_data += ",'";
	insert_data += name;
	insert_data += "',";
	insert_data += age;
	insert_data += ",'";
	insert_data += habby;
	insert_data += "'";
	//insert data == "0,'common',10,'listen'"

	return insert_data;
}

int main()
{
	int content_length = -1;
	char method[1024];       //store request method 
	char query_string[1024]; //store query_string
	char post_data[4096];
	bzero(method, sizeof(method));
	bzero(query_string, sizeof(query_string));
	bzero(post_data, sizeof(post_data));

	cout<<"<html>"<<endl;
	cout<<"<head>show student</head>"<<endl;
	cout<<"<body>"<<endl;

	sql_connecter conn(_host, _user, _passwd, _db);
   	conn.begin_connect();  //connect to Database Mysql

	strcpy(method, getenv("REQUEST_METHOD"));

	if( strcasecmp("GET", method) == 0)
	{
		strcpy(query_string, getenv("QUERY_STRING"));
		string insert_data = get_insert_data(query_string, strlen(query_string)); //get the insert data
		conn.insert_sql(insert_data); //insert the data to the database
	}
	else if( strcasecmp("POST", method) == 0 )
	{
		content_length = atoi(getenv("CONTENT_LENGTH"));
		int i = 0; 
		for(; i < content_length; i++ ){
			read(0, &post_data[i], 1);
		}
		post_data[i] = '\0';
		
		string insert_data = get_insert_data(post_data, content_length);  //get the insert data
		conn.insert_sql(insert_data); //insert the data to the database
	}
	else
	{}

	cout<<"</body>"<<endl;
	cout<<"</html>"<<endl;

	return 0;
}
