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

int main()
{
	int content_length = -1;
	char method[1024];
	char query_string[1024];
	char post_data[4096];
	memset(method, '\0', sizeof(method));
	memset(query_string, '\0', sizeof(query_string));
	memset(post_data, '\0', sizeof(post_data));

	cout<<"<html>"<<endl;
	cout<<"<head>show student</head>"<<endl;
	cout<<"<body>"<<endl;

	strcpy(method, getenv("REQUEST_METHOD"));

   	sql_connecter conn(_host, _user, _passwd, _db);
   	conn.begin_connect(); //connect to the Database Mysql

   	string _sql_data[1024][4];  //store the data in the database
   	string header[4];  //store the head of a table
   	int curr_row = -1; //store the num of rows

	//for(int i = 0; i < 1024; ++i)
	//	bzero(_sql_data[i], sizeof(int)*4);    // _sql_data type is string 
	//bzero(header, sizeof(header));

	if( strcasecmp("GET", method) == 0 )
	{
	}
	else if( strcasecmp("POST", method) == 0 )
	{
	}
	else
	{
	}

	//get the data stored in database
   	conn.select_sql(header, _sql_data, curr_row);

	cout<<"<table border=\"1\">"<<endl;
	cout<<"<tr>"<<endl;
   	for(int i = 0; i<4; i++)
	{
   		cout<<"<th>"<<header[i]<<"</th>"<<endl;
   	}
	cout<<"</tr>"<<endl;
   
    for(int i = 0; i<curr_row; i++)
	{
		cout<<"<tr>"<<endl;
    	for(int j=0; j<4; j++)
		{
			cout<<"<td>"<<_sql_data[i][j]<<"</td>"<<endl;;
		}
		cout<<"</tr>"<<endl;
   	}
	cout<<"</table>"<<endl;
	cout<<"</body>"<<endl;
	cout<<"</html>"<<endl;
	return 0;
}
