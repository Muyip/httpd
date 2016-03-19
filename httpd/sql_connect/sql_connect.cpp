#include"sql_connect.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

sql_connecter::sql_connecter(const string &_host, const string &_user, const string &_passwd, const string &_db)
{
	mysql_base = mysql_init(NULL);
	res = NULL;
	host = _host;
	user = _user;
	passwd = _passwd;
	db = _db;
}

sql_connecter::~sql_connecter()
{
	close_connect();
	if(NULL != res)
		free(res);
}

bool sql_connecter::begin_connect()
{
	if(mysql_real_connect(mysql_base, host.c_str(), user.c_str(), passwd.c_str(), db.c_str(), 3306, NULL, 0) == NULL)
	{
		perror("mysql_real_connect");
		return false;
	}
	else
	{
		//cout<<"connect success :)"<<endl;
		return true;
	}
}

bool sql_connecter::select_sql(string _out_field_name[], string _out_data[][4], int &_out_row)
{	
	string sql = "SELECT * FROM stu_info;";	
	if(mysql_query(mysql_base, sql.c_str()) != 0)
	{
		perror("mysql_query");
	}
	else
	{
		//cout<<"query_success\n"<<endl;
	}

	res = mysql_store_result(mysql_base);
	
	int row_num = mysql_num_rows(res);
	int field_num = mysql_num_fields(res);
	
	_out_row = row_num;
	
	MYSQL_FIELD * fd = NULL;
	int i = 0;

	for(; fd = mysql_fetch_field(res); )
	{
		_out_field_name[i++] = fd->name;
	}
	for(int index=0; index < row_num; index++)
	{
		MYSQL_ROW _row = mysql_fetch_row(res);
		if(_row)
		{
			for(int start = 0; start < field_num; ++start)
			{
				_out_data[index][start] = _row[start];
			}
		}
	}	
	return true;
}

bool sql_connecter::insert_sql(const string &data)
{
	string insert_data = "INSERT INTO stu_info (id, name, age, habby) values ";
	insert_data += "(";
	insert_data += data;
	insert_data += ");";
	if(mysql_query(mysql_base, insert_data.c_str()) == 0)
	{
		cout<<"query success!"<<endl;
		return true;
	}
	else
	{	
		cout<<"query failed!"<<endl;
		return false;
	}
}

bool sql_connecter::delete_sql(string to_delete)
{
	string delete_item = "DELETE FROM stu_info WHERE(age=";
	delete_item += to_delete;
	delete_item += ");";
	if(mysql_query(mysql_base, delete_item.c_str()) != 0)
	{
		//cout<<"query success!"<<endl;
		perror("mysql_query");
		return false;
	}
	else
	{
		//cout<<"query success!"<<endl;
		return true;
	}
}	

bool sql_connecter::close_connect()
{
	mysql_close(mysql_base);
	return true;
}

bool sql_connecter::show_info()
{
	cout<<mysql_get_client_info();
	return true;
}

//#ifndef _DEBUG_
//#define _DEBUG_

//int main()
//{
//	const string _host = "127.0.0.1";
//	const string _user = "common";
//	const string _password = "common";
//	const string _db = "study";
//
//	string mysql_data[1024][4];
//	string mysql_head[4];
//	int data_num = 0;
//
//	sql_connecter conn(_host, _user, _password, _db);
//	conn.begin_connect();
//	//conn.show_info();
//
//	//cout<<"begin insert.."<<endl;
//	//const string insert_data = "0, 'yp', 22, 'read'";
//	//if(conn.insert_sql(insert_data))
//	//	cout<<"data : "<<insert_data<<"  has been inserted"<<endl<<"new data: "<<endl;
//
//	conn.select_sql(mysql_head, mysql_data, data_num);
//	for(int i = 0; i<4; ++i)
//	{
//		cout<<mysql_head[i]<<"\t";
//	}
//	cout<<endl;
//	for(int i = 0; i<data_num; ++i)
//	{
//		for(int j = 0; j<4; ++j)
//			cout<<mysql_data[i][j]<<"\t";
//		cout<<endl;
//	}
//

//	conn.close_connect();
//	conn.begin_connect();
//	
//	string delete_age = "12";
//	conn.delete_sql(delete_age);
//	conn.select_sql(mysql_head, mysql_data, data_num);
//	for(int i = 0; i<4; ++i)
//	{
//		cout<<mysql_head[i]<<"\t";
//	}
//	cout<<endl;
//
//	for(int i = 0; i<data_num; ++i)
//	{
//		for(int j = 0; j<4; ++j)
//			cout<<mysql_data[i][j]<<"\t";
//		cout<<endl;
//	}
//
//	return 0;
//}

//#endif
