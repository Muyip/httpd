#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<error.h>
#include<string.h>
#include"mysql.h"
using namespace std;

class sql_connecter
{
	public:
		sql_connecter(const string &_host, const string &_user, const string &_passwd, const string &_db);
		~sql_connecter();
		bool begin_connect();
		bool insert_sql(const string &data);
		bool select_sql(string _out_field_name[], string _out_data[][4], int &_out_row); 
		bool close_connect();
		bool show_info();	

		////bool update_sql();
		bool delete_sql(string to_delete);
		////bool create_table();
		////bool delete_table();
	private:
		MYSQL_RES *res;
		MYSQL *mysql_base;
		string host;
		string user;
		string passwd;
		string db;
};
